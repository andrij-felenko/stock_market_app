#ifndef API_MARKETSTACK_H
#define API_MARKETSTACK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/transport/provider.h"
#include "api/transport/call.h"


class sdk::api::MarketStack : public Provider {
    Q_OBJECT
public:
    static MarketStack* instance();

    static void updateInfoByTag(const sdk::Symbol& tag);
    static void updateQuotesByTag(sdk::SymbolList tags);

private:
    explicit MarketStack(QObject* parent = nullptr);
    using sdk::api::Provider::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Call* reply) override;
};

#endif // API_MARKETSTACK_H
