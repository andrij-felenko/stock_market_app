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
    static FinnHub* instance();

    // void set_api_key(const QString& key);

    static void updateInfoByTag(QString tag);

private:
    explicit FinnHub(QObject* parent = nullptr);
    using api::API::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Reply* reply) override;
};

#endif
