#ifndef API_FINNHUB_H
#define API_FINNHUB_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/transport/provider.h"
#include "api/transport/call.h"


class sdk::api::FinnHub : public Provider {
    Q_OBJECT
public:
    static FinnHub* instance();

    // void set_api_key(const QString& key);

    static void updateInfoByTag(QString tag);

private:
    explicit FinnHub(QObject* parent = nullptr);
    using sdk::api::Provider::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Call* reply) override;
};

#endif
