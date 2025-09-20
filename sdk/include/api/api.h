#ifndef API_API_H
#define API_API_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <deque>

#include "reply.h"
#include "settings/network.h" // IWYU pragma: keep
#include "instrument/symbol.h"

namespace api {
    class API;

    using StringMap = QMap <QString, QString>;
}

class api::API : public QObject {
    Q_OBJECT
public:
    explicit API(QUrl url, QObject* parent = nullptr);

protected:
    virtual bool request(Request type, const QString& name, StringMap keys = {}) final;
    virtual bool request(Request type, const sdk::Symbol& symbol, StringMap keys = {}) final;
    virtual bool request(Request type, const QString& name,
                         const sdk::Symbol& symbol, StringMap keys = {}) = 0;

    virtual void handlerAnswer(Reply* reply) = 0;
    virtual void handlerError (Reply* reply, QNetworkReply::NetworkError error);

    virtual bool lock() const final { return _lock; }
    virtual void finish(QNetworkReply* reply) final;

    int shift_ms;
    virtual bool queueContains(Request r) const final;
    virtual Reply* add(api::Request type) final;

private:
    QUrl url;
    bool _lock;
    std::deque <Reply*> _queue;
    QNetworkAccessManager _netmanager;
    friend class Reply;

    virtual void _next() final;

signals:
    void errorOccurred(QString error);
    void errorReply(QNetworkReply* reply);
};

#endif
