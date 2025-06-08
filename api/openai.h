#ifndef API_OPENAI_H
#define API_OPENAI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

namespace api { class OpenAI; }

class api::OpenAI : public QObject {
    Q_OBJECT

public:
    static OpenAI* instance();

    void set_api_key(const QString& key);
    void send_chat_request(const QString& prompt,
                           const QString& model = "gpt-4.1-nano",
                           bool stream = false,
                           int max_chars = 50);

signals:
    void reply_chunk(const QString& chunk);
    void reply_complete(const QString& full_text);
    void error_occurred(const QString& message);

private:
    explicit OpenAI(QObject* parent = nullptr);

    void readyRead();
    void finished();

    QNetworkAccessManager m_manager;
    QString m_api_key;
    QNetworkReply* m_reply = nullptr;
    QString m_collected_text;
};

#endif // API_OPENAI_H
