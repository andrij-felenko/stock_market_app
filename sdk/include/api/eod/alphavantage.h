#ifndef API_ALPHA_VANTAGE_H
#define API_ALPHA_VANTAGE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"
#include "instrument/ticker.h"
#include "services/market.h"

namespace api { class AlphaVantage; }

class api::AlphaVantage : public API {
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
    using api::API::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Reply* reply) override;

    void handleInfo    (Reply* reply);
    void handleCandle  (Reply* reply);
    void handleTag     (Reply* reply);
    void handleDividend(Reply* reply);
};

#endif
