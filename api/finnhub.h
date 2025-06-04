#ifndef API_FINNHUB_H
#define API_FINNHUB_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class FinnHub; }

class api::FinnHub : public API {
    Q_OBJECT
public:
    explicit FinnHub(QObject* parent = nullptr);
    void set_api_key(const QString& key);

    virtual bool request(Request type, QString name, StringMap keys = {}) override;

signals:
    void error_occurred(const QString& message);

private:
    QNetworkAccessManager m_manager;
    QString m_api_key;
    QNetworkReply* m_reply = nullptr;
    // void _ready_read();

    virtual void handler_answer(Request type, QByteArray data, QString name) override;
};

#endif
