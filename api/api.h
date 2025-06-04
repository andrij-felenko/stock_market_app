#ifndef API_API_H
#define API_API_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>

#include "reply.h"

namespace api {
    class API;

    using StringMap = std::map <QString, QString>;
}

class api::API : public QObject {
    Q_OBJECT
public:
    explicit API(QObject* parent = nullptr);

protected:
    virtual void handler_answer(Request type, QByteArray data, QString name) = 0;
    virtual bool request(Request type, QString name, StringMap keys = {}) = 0;

    virtual void add_reply(api::Request type,
                           QNetworkReply* reply,
                           const QString& symbol) final;

private:
    std::vector <Reply*> _replies;

    void finish(QNetworkReply* reply);
    void have_answer(QNetworkReply* reply);
};

#endif
