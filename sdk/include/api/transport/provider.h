#ifndef SDK_API_TRANSPORT_PROVIDER_H
#define SDK_API_TRANSPORT_PROVIDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <deque>

#include "sdk_def.h"
#include "call.h"
#include "api/connectors/endpoints.h" // IWYU pragma: keep
#include "core/symbol.h"

namespace sdk::api {
    using StringMap = QMap <QString, QString>;
}

class sdk::api::Provider : public QObject {
    Q_OBJECT
public:
    explicit Provider(QUrl url, QObject* parent = nullptr);

protected:
    virtual bool request(Request type, const QString& name, StringMap keys = {}) final;
    virtual bool request(Request type, const sdk::Symbol& symbol, StringMap keys = {}) final;
    virtual bool request(Request type, const QString& name,
                         const sdk::Symbol& symbol, StringMap keys = {}) = 0;

    virtual void handlerAnswer(Call* reply) = 0;
    virtual void handlerError (Call* reply, QNetworkReply::NetworkError error);

    virtual bool lock() const final { return _lock; }
    virtual void finish(QNetworkReply* reply) final;

    int shift_ms;
    virtual bool queueContains(Request r) const final;
    virtual Call* add(api::Request type) final;

private:
    QUrl url;
    bool _lock;
    std::deque <Call*> _queue;
    QNetworkAccessManager _netmanager;
    friend class sdk::api::Call;

    virtual void _next() final;

signals:
    void errorOccurred(QString error);
    void errorReply(QNetworkReply* reply);
};

#endif // SDK_API_TRANSPORT_PROVIDER_H
