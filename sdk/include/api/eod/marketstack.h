#ifndef API_MARKETSTACK_H
#define API_MARKETSTACK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class MarketStack; }

class api::MarketStack : public API {
    Q_OBJECT
public:
    static MarketStack* instance();

    static void updateInfoByTag(const sdk::Symbol& tag);
    static void updateQuotesByTag(sdk::SymbolList tags);

private:
    explicit MarketStack(QObject* parent = nullptr);
    using api::API::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Reply* reply) override;
};

#endif // API_MARKETSTACK_H
