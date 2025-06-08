#include "openai.h"
#include <QJsonArray>
#include <QDebug>
#include <QtGui/QGuiApplication>

api::OpenAI* api::OpenAI::instance()
{
    static OpenAI* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new OpenAI(qApp);
    }
    return _instance;
}

api::OpenAI::OpenAI(QObject* parent) : QObject(parent)
{
    set_api_key("sk-proj-vccDzzJrmHvKungJmFIz_U5X_yZI3wvadiKedhBomYzXUNv"
                "4XVU7nP4l84VqJZ9TlMeVhQLmLXT3BlbkFJ21RmjsV5oOpor1XvYbNa"
                "c2aJjNBNoz-S7SF8C84Lx4fqZHxEODILJk6bo8PyzBPCfXjDuQcJQA");
}

void api::OpenAI::set_api_key(const QString& key) { m_api_key = key; }

void api::OpenAI::send_chat_request(const QString& prompt,
                                    const QString& model,
                                    bool stream,
                                    int max_chars) {
    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
    }

    QUrl url("https://api.openai.com/v1/chat/completions");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + m_api_key.toUtf8());

    QJsonArray messages;
    if (max_chars > 0) {
        messages.append(QJsonObject{
            {"role", "system"},
            {"content", "Відповідай дуже стисло, лише суть, максимум " + QString::number(max_chars) + " символів. Не повторюй питання."}
        });
    }
    messages.append(QJsonObject{{"role", "user"}, {"content", prompt}});

    QJsonObject payload{
        {"model", model},
        {"temperature", 0.2},
        {"stream", stream},
        {"messages", messages}
    };

    QByteArray data = QJsonDocument(payload).toJson();
    m_collected_text.clear();

    m_reply = m_manager.post(request, data);
    qDebug() << "SENT:" << QString::fromUtf8(data);

    if (stream)
        connect(m_reply, &QIODevice::readyRead, this, &OpenAI::readyRead);
    else
        connect(m_reply, &QNetworkReply::finished, this, &OpenAI::finished);
}

void api::OpenAI::readyRead()
{
    while (m_reply->canReadLine()) {
        QByteArray line = m_reply->readLine();
        if (line.startsWith("data: ")) {
            QByteArray json = line.mid(6).trimmed();
            if (json == "[DONE]") {
                emit reply_complete(m_collected_text);
                qDebug() << "reply data 0" << m_collected_text;
                return;
            }
            QJsonDocument doc = QJsonDocument::fromJson(json);
            QJsonObject delta = doc["choices"].toArray()[0].toObject()["delta"].toObject();
            QString content = delta["content"].toString();
            if (!content.isEmpty()) {
                m_collected_text += content;
                qDebug() << "reply data" << content;
                emit reply_chunk(content);
            }
        }
    }
}

void api::OpenAI::finished()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        emit error_occurred(m_reply->errorString());
        qDebug() << "reply error" << m_reply->errorString();
        m_reply->deleteLater();
        m_reply = nullptr;
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(m_reply->readAll());
    QString text = doc["choices"]
                       .toArray()[0]
                       .toObject()["message"]
                       .toObject()["content"]
                       .toString();
    emit reply_chunk(text);
    emit reply_complete(text);

    qDebug() << "reply chunk" << text;
    m_reply->deleteLater();
    m_reply = nullptr;
}
