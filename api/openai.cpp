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

api::OpenAI::OpenAI(QObject* parent)
    : API(parent),
      _model("gpt-4.1-nano")
{
    // set_api_key("sk-proj-vccDzzJrmHvKungJmFIz_U5X_yZI3wvadiKedhBomYzXUNv"
    //             "4XVU7nP4l84VqJZ9TlMeVhQLmLXT3BlbkFJ21RmjsV5oOpor1XvYbNa"
    //             "c2aJjNBNoz-S7SF8C84Lx4fqZHxEODILJk6bo8PyzBPCfXjDuQcJQA");
}

// void api::OpenAI::set_api_key(const QString& key) { _api_key = key; }

void api::OpenAI::set_model(QString new_model) { _model = new_model; }

bool api::OpenAI::_request(Request type, QString text, StringMap keys) {
    QUrl url("https://api.openai.com/v1/chat/completions");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + settings::network()->key_oai().toUtf8());

    int max_chars = keys.value("max", "50").toInt();
    bool stream = keys.value("stream", "false") == "true";

    QJsonArray messages;
    if (max_chars > 0) {
        messages.append(QJsonObject{
            {"role", "system"},
            {"content", "Відповідай дуже стисло, лише суть, максимум " +
                        QString::number(max_chars) +
                        " символів. Не повторюй питання."}
        });
    }
    messages.append(QJsonObject {{"role", "user"}, {"content", text}});

    QJsonObject payload {
        { "model", _model },
        { "temperature", 0.2 },
        { "stream", stream },
        { "messages", messages }
    };

    static auto reader = [](QByteArray chunk) -> QByteArray {
        QByteArray result;
        QList<QByteArray> lines = chunk.split('\n');

        for (const QByteArray& line : std::as_const(lines)) {
            if (!line.startsWith("data: ")) continue;

            QByteArray json = line.mid(6).trimmed();
            if (json == "[DONE]") break;

            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(json, &err);
            if (err.error != QJsonParseError::NoError) continue;

            QJsonObject delta = doc["choices"]
                                   .toArray()[0]
                                   .toObject()["delta"]
                                   .toObject();

            QString content = delta["content"].toString();
            if (!content.isEmpty())
                result += content.toUtf8();
        }

        return result;
    };


    QByteArray data = QJsonDocument(payload).toJson();
    if (stream)
        API::_add_reply(type, _netmanager.post(request, data), text, reader);
    else
        API::_add_reply(type, _netmanager.post(request, data), text);

    return true;
}

void api::OpenAI::recheck_tag(QString tag)
{
    _request(Request::Tag,
        "Знайди компанію за частковою або повною назвою, або категорією, або за відомим брендом, "
        "підрозділом чи пошуковим запитом, який повʼязаний із компанією. "
        "Наприклад, якщо користувач запитує \"Google\" — має бути знайдена Alphabet Inc. "
        "Якщо \"Windows\" — Microsoft. Якщо \"YouTube\" — знову Alphabet Inc. "
        "Знайти по фрагменту: \"" + tag + "\", "
        "яка публічно торгується на біржах. "
        "Виведи декілька компаній."
        "По кожній вкажи повну назву компанії та список усіх бірж, де вона присутня. "
        "Використовуй формат тега: SYMBOL.REGION (наприклад: NEST.FI, NEST.US, BMW.DE). "
        "Особливо зверни увагу на присутність у США (US), Європі (EURONEXT наприклад, .EU), Азії (.HK, .JP) та національних біржах."
        "Регіон — це код регіону біржі: US, FI, DE, JP, IN, CA тощо. "
        "Обов’язково додай всі варіанти розміщення, "
        "навіть якщо це лише ADR або вторинне лістування. "
        "Виведи результат як масив JSON-обʼєктів: "
        "[{\"all_tags\": [\"SYMBOL.REGION\", ...], \"tag\": \"Основний тег\", "
        "\"title\": \"Повна назва компанії\"}]. "
        "Не додавай пояснень. Тільки JSON. "
        "Ось приклад:\n"
        "[{\"all_tags\": [\"OR.EPA\", \"LRLCY.US\"], \"tag\": \"OR.EPA\", "
        "\"title\": \"L'Oréal S.A.\"}]"
    );

}

void api::OpenAI::_handler_answer(Request type, QByteArray data, QString name, bool stream)
{
    if (stream){
        emit reply_complete(data);
        qDebug() << "OPENAI result" << data;
    }
    else {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QString text = doc["choices"]
                           .toArray()[0]
                           .toObject()["message"]
                           .toObject()["content"]
                           .toString();
        emit reply_complete(text);
        qDebug() << "OPENAI result" << text;
    }
}
