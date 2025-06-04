#ifndef API_OPENAI_H
#define API_OPENAI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

class OpenAiApiClient : public QObject {
    Q_OBJECT

public:
    explicit OpenAiApiClient(QObject* parent = nullptr);

    void set_api_key(const QString& key);
    void send_chat_request(const QString& prompt,
                           const QString& model = "gpt-4o",
                           bool stream = false,
                           int max_chars = 50);

signals:
    void reply_chunk(const QString& chunk);
    void reply_complete(const QString& full_text);
    void error_occurred(const QString& message);

private:
    void readyRead();
    void finished();

    QNetworkAccessManager m_manager;
    QString m_api_key;
    QNetworkReply* m_reply = nullptr;
    QString m_collected_text;
};

#endif // API_OPENAI_H
