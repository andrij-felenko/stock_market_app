#ifndef API_EOD_H
#define API_EOD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include "sdk_def.h"
#include "core/symbol.h"
#include "api/transport/provider.h"

class sdk::api::Eod final : public Provider {
    Q_OBJECT
public:
    static Eod* instance();

    // void set_api_key(const QString& key);

    static void getAllTag(const QString& exchange);
    static void getAllExchangeTag();

    // void fetch_ticker_data(const QString& ticker);

    static void fundamental(const sdk::Symbol& tag);
    static void historicalYear(const sdk::Symbol& tag, int8_t year = -1, char period = 'd');

signals:
    void dataReady(const QJsonObject& data);

private:
    explicit Eod(QObject* parent = nullptr);

    using api::Provider::request;
    friend class sdk::Market;
    friend class sdk::Instrument;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& symbol,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Call* reply) override;
    virtual void handlerError(Call* reply, QNetworkReply::NetworkError error) override;

    void _handleExchange(Call* reply);
    void _handleCandle  (Call* reply);
    void _handleInfo    (Call* reply);

    void _handleInfoGeneral           (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoHighlights        (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoValuation         (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoSharesStats       (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoSplitsDividends   (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoAnalystRatings    (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoOutstandingShares (const sdk::Finder& finded, const QJsonObject& obj);

    void _handleInfoEarnings          (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoEarningsHistory   (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoEarningsTrend     (const sdk::Finder& finded, const QJsonObject& obj);

    void _handleInfoFinancials        (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoFinancialsBalance (const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoFinancialsCashFlow(const sdk::Finder& finded, const QJsonObject& obj);
    void _handleInfoFinancialsIncome  (const sdk::Finder& finded, const QJsonObject& obj);
};

#endif
