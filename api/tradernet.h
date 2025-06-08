#ifndef API_TRADERNET_H
#define API_TRADERNET_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class TraderNet; }

class api::TraderNet : public API {
    Q_OBJECT
public:
    explicit TraderNet(QObject* parent = nullptr);
    void set_api_key(const QString& key);
    void set_secret_key(const QString& key);

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;

signals:
    void error_occurred(const QString& message);

private:
    QNetworkAccessManager m_manager;
    QString _api_key;
    QString _secret_key;

    QString _sign(const QString& msg) const;

    virtual void _handler_answer(Request type, QByteArray data, QString name) override;
};

#endif // API_TRADERNET_H
