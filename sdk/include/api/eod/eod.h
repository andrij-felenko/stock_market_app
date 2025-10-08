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

    void _handleInfoGeneral           (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoHighlights        (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoValuation         (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoSharesStats       (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoSplitsDividends   (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoAnalystRatings    (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoOutstandingShares (const market::Finder& finded, const QJsonObject& obj);

    void _handleInfoEarnings          (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoEarningsHistory   (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoEarningsTrend     (const market::Finder& finded, const QJsonObject& obj);

    void _handleInfoFinancials        (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoFinancialsBalance (const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoFinancialsCashFlow(const market::Finder& finded, const QJsonObject& obj);
    void _handleInfoFinancialsIncome  (const market::Finder& finded, const QJsonObject& obj);
};

namespace sdk { constexpr inline Singleton <api::Eod> apiEod {}; }

#endif
