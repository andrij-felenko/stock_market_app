#ifndef API_FIGI_H
#define API_FIGI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/transport/provider.h"
#include "api/transport/call.h"


class sdk::api::Figi : public Provider {
    Q_OBJECT
public:
    static Figi* instance();

    static void updateInfoByTag(const sdk::Symbol& tag);

private:
    explicit Figi(QObject* parent = nullptr);
    using sdk::api::Provider::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& symbol,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Call* reply) override;
};

#endif
