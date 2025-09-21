#ifndef API_ALPHA_VANTAGE_H
#define API_ALPHA_VANTAGE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/transport/provider.h"
#include "api/transport/call.h"
#include "core/security/listing/ticker.h"
#include "service/market.h"

class sdk::api::AlphaVantage : public Provider {
    Q_OBJECT
public:
    static AlphaVantage* instance();

    static void updateInfoByTag (const sdk::Symbol& tag);
    static void dailyCandleByTag(const sdk::Symbol& tag);
    static void todayCandleByTag(const sdk::Symbol& tag);
    static void findTag(QString str);

public slots:
    void findSymbol(QString str);

private:
    explicit AlphaVantage(QObject* parent = nullptr);
    using sdk::api::Provider::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Call* reply) override;

    void handleInfo    (Call* reply);
    void handleCandle  (Call* reply);
    void handleTag     (Call* reply);
    void handleDividend(Call* reply);
};

#endif
