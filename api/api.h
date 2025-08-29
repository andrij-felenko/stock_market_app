#ifndef API_API_H
#define API_API_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <deque>
#include <vector>

#include "reply.h"
#include "settings/network.h" // IWYU pragma: keep

namespace api {
    class API;

    using StringMap = QMap <QString, QString>;
}

class api::API : public QObject {
    Q_OBJECT
public:
    explicit API(QObject* parent = nullptr);

protected:
    virtual bool _request(Request type, QString name, StringMap keys = {}) = 0;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) = 0;
    virtual void _handler_error(Request type, QNetworkReply::NetworkError error, QString name);

    virtual void _add_reply(api::Request type,
                            QNetworkReply* reply,
                            const QString& symbol,
                            std::function <QByteArray (QByteArray)> reader = nullptr) final;

    QNetworkAccessManager _netmanager;

    std::vector <Reply*> _replies;

    struct QueueItem {
        Request type;
        QString name;
        StringMap keys;

        QueueItem(Request type, QString name, StringMap keys = {})
            : type(type), name(name), keys(keys) { /* */ }
    };

    virtual void _send(Request type, QString name, StringMap keys = {}) final;
    virtual bool lock() const final { return _lock; }
    void _finish(QNetworkReply* reply);

    int shift_ms;
    virtual bool _queue_contains(Request r) const final;

private:
    bool _lock;
    std::deque <QueueItem> _queue;
    virtual void _next() final;

signals:
    void error_occurred(QString error);
    void error_reply(QNetworkReply* reply);
};

#endif
