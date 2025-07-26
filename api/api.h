#ifndef API_API_H
#define API_API_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>

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

    virtual void _add_reply(api::Request type,
                            QNetworkReply* reply,
                            const QString& symbol,
                            std::function <QByteArray (QByteArray)> reader = nullptr) final;

    QNetworkAccessManager _netmanager;

signals:
    void error_occurred(QString error);
    void error_reply(QNetworkReply* reply);

protected:
    std::vector <Reply*> _replies;

    void _finish(QNetworkReply* reply);
};

#endif
