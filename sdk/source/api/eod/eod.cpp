#include "api/eod/eod.h"
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonArray>
#include "core/security/instrument.h"
#include "loader.h"
#include "common/json.h"

sdk::api::Eod::Eod(QObject* parent) : Provider(QUrl("https://eodhd.com/api"), parent)
{
    //
}

sdk::api::Eod* sdk::api::Eod::instance()
{
    static Eod* _instance = nullptr;
    if (_instance == nullptr)
        _instance = new Eod(qApp);

    return _instance;
}

void sdk::api::Eod::getAllTag(const QString& exchange)
{
    Eod* data = Eod::instance();
    data->request(Request::Exchange, exchange);
}

void sdk::api::Eod::getAllExchangeTag()
{
    qDebug() << Q_FUNC_INFO;
    Eod* data = Eod::instance();
    QStringList list = sdk::exchange::all_exchange_venue();
    for (const auto& it : std::as_const(list))
        data->request(Request::Exchange, it);
}

void sdk::api::Eod::fundamental(const sdk::Symbol& tag)
{
    Eod::instance()->request(Request::Info, tag);
}

void sdk::api::Eod::historicalYear(const sdk::Symbol& tag, int8_t year, char period)
{
    qDebug() << "EOD ADD BY TAG" << tag;
    if (year >= 30 || year < 1)
        year = 30;

    QDate date = QDate::currentDate();

    api::StringMap params = {
        { "period", QString(period) },
        { "from", QDate(date.year() - year, date.month(), date.day()).toString("yyyy-MM-dd") },
        { "to", date.toString("yyyy-MM-dd") },
        { "tag", tag.full() }
    };
    Eod::instance()->request(Request::Candle, tag, params);
}

bool sdk::api::Eod::request(Request type, const QString& name,
                            const sdk::Symbol& symbol, StringMap keys)
{
    qDebug() << Q_FUNC_INFO << name << symbol << keys;
    Call* post = add(type);

    switch (type){
        case api::Request::Text:

        case api::Request::MetricAll:
        case api::Request::MetricPrice:
        case api::Request::MetricMargin:
        case api::Request::MetricValuation:

        case api::Request::Peers:
        case api::Request::Quote:
        case api::Request::Tag:
        case api::Request::Dividend:
        case api::Request::Earnings:
        case api::Request::Logo:
        case api::Request::Reported: return false;
        case api::Request::Exchange: post->suburl = "/exchange-symbol-list/" + name;   break;
        case api::Request::Candle:   post->suburl = "/eod/" + keys["tag"];             break;
        case api::Request::Info:     post->suburl = "/fundamentals/" + symbol.full();  break;
    }

    switch (type){
        case api::Request::Exchange:
        case api::Request::Info:
        {
            post->addQueryItem("api_token", apiEndPoints->key_eod());
            post->addQueryItem("fmt", "json");
            break;
        }
        case api::Request::Candle: {
            // https://eodhd.com/api/eod/MCD.US?api_token=683ebb8bc59b60.11043967&fmt=json
            post->addQueryItem("api_token", apiEndPoints->key_eod());
            post->addQueryItem("period", keys.value("period", "d"));
            post->addQueryItem("fmt", "json");
            post->addQueryItem("from", keys.value("from"));
            post->addQueryItem("to",   keys.value("to"));
            break;
        }
        default:;
    }

    switch (type){
        case api::Request::Exchange: post->name = name; break;
        case api::Request::Candle:
        case api::Request::Info: post->symbol = symbol; break;
        default:;
    }

    post->prepare();
    return true;
}

void sdk::api::Eod::handlerAnswer(Call* reply)
{
    qDebug() << "handler answer eod";

    switch (reply->type()){
        case api::Request::Exchange: { _handleExchange(reply); break; }
        case api::Request::Candle:   { _handleCandle  (reply); break; }
        case api::Request::Info:     { _handleInfo    (reply); break; }
        default:;
    }
}

void sdk::api::Eod::handlerError(Call* reply, QNetworkReply::NetworkError error)
{
    qDebug() << Q_FUNC_INFO << static_cast <uint32_t> (reply->type()) << error
             << reply->name << reply->symbol;

    switch (reply->type()){
    case api::Request::Exchange: {
        break;
    }
    case api::Request::Candle: {
        break;
    }
    case api::Request::Info: {
        if (error == QNetworkReply::ContentNotFoundError){
            qDebug() << "Info";
            auto ticker = Nexus->market()->findTicker(reply->symbol);
            if (not ticker.exist()) {
                qDebug() << reply->symbol << "in info eod not found";
                return;
            }
            sdk::Instrument* in = ticker->instrument();
            bool after = false;
            for (const auto& it : in->tickers()){
                qDebug() << after << it;
                if (after){
                    fundamental(it);
                    break;
                }
                else {
                    if (it == reply->symbol)
                        after = true;
                }
            }
        }
        break;
    }
    default:;
    }
}

void sdk::api::Eod::_handleExchange(Call* reply)
{
    static uint32_t count = 0;
    QJsonDocument doc = QJsonDocument::fromJson(reply->receiveData());
    auto market = Nexus->market();
    QJsonArray root = doc.array();
    count += root.size();
    for (const auto& it : std::as_const(root)){
        QJsonObject obj = it.toObject();
        Nexus->market()->addTicker(sdk::Symbol(obj["Code"].toString(), reply->name),
                                   obj["Isin"].toString().toLatin1(),
                                   obj["Name"].toString(),
                                   sdk::instype::from_string(obj["Type"].toString()));
    }

    // handle exchange queue list
    // TODO found main ticker function need to call
    // if (not _queue_contains(Request::Exchange))
        // Nexus.market()->detect_main_ticker();

    qDebug() << "TOTAL CPUNT -----------------------------------------------------" << root.size()
             << "--------" << count << reply->request()->url();
    market->saveMeta();
}

void sdk::api::Eod::_handleCandle(Call* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->receiveData());
    auto ticker = Nexus->market()->findTicker(reply->symbol);
    if (ticker.ensure() == false){
        qDebug() << Q_FUNC_INFO << reply->symbol << "FALSE";
        return;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue& v : std::as_const(array)){
        QJsonObject obj = v.toObject();
        QString dtime   = obj[  "date"].toString();
        float open      = obj[  "open"].toDouble();
        float close     = obj[ "close"].toDouble();
        float high      = obj[  "high"].toDouble();
        float low       = obj[   "low"].toDouble();
        uint64_t volume = obj["volume"].toDouble();

        QDate date = QDate::fromString(dtime, "yyyy-MM-dd");
        ticker->quotes.setData(date, open, close, high, low, volume);
    }

    ticker->save();
}

void sdk::api::Eod::_handleInfo(Call* reply)
{
    qDebug() << Q_FUNC_INFO;
    auto finded = Nexus->market()->findTicker(reply->symbol);
    if (finded.ensure() == false) {
        qDebug() << reply->symbol << "not found";
        return;
    }

    QJsonObject root = QJsonDocument::fromJson(reply->receiveData()).object();
    _handleInfoGeneral(finded, root);
    _handleInfoHighlights(finded, root);
    _handleInfoValuation(finded, root);
    _handleInfoSharesStats(finded, root);

    // ----------------------- Technicals ---------------------------------------------------------
    // NOTE already have it in other fields
    // ============================================================================================

    _handleInfoSplitsDividends(finded, root);
    _handleInfoAnalystRatings(finded, root);

    // ----------------------- Holders ------------------------------------------------------------
    // TODO not save for now
    // ============================================================================================

    // ----------------------- InsiderTransactions ------------------------------------------------
    // TODO not save for now
    // ============================================================================================

    // ----------------------- ESGScores ----------------------------------------------------------
    // NOTE i`ts trash block
    // ============================================================================================

    _handleInfoOutstandingShares(finded, root);
    _handleInfoEarnings(finded, root);
    _handleInfoFinancials(finded, root);

    finded->save();
}

void sdk::api::Eod::_handleInfoGeneral(const sdk::market::Finder& finded, const QJsonObject& obj)
{
    auto g = json::object(obj, "General");
    if (not g)
        return;

    sdk::Legal& legal = finded.instrument->data()->legal;
    sdk::Meta&  meta  = finded.instrument->data()->meta;
    using namespace json;

    // Legal
    if (auto v = string(*g, "OpenFigi"))             legal.setOpenFigi(*v);
    if (auto v = string(*g, "LEI"))                  legal.setLei(*v);
    if (auto v = string(*g, "CIK"))                  legal.setCik(*v);
    if (auto v = string(*g, "CUSIP"))                legal.setCusip(*v);
    if (auto v = string(*g, "EmployerIdNumber"))     legal.setEmployerIdNumber(*v);
    if (auto v = date   (*g, "IPODate"))             legal.setIpo(*v);
    if (auto v = boolean(*g, "IsDelisted"))          legal.setDelisted(*v);
    if (auto v = string (*g, "Address"))             legal.setAddress(*v);
    if (auto v = object (*g, "AddressData"))         legal.setAddress(*v);

    // FiscalYearEnd: мапимо англ. назву місяця у sdk::Month локально, без дод. функцій
    if (auto v = string(*g, "FiscalYearEnd")) legal.setFiscalYearEnd(sdk::month::from_string(*v));

    // Meta
    if (auto v = string(*g, "Sector"))               meta.setSector(*v);
    if (auto v = string(*g, "Industry"))             meta.setIndustry(*v);
    if (auto v = string(*g, "GicSector"))            meta.setGicSector(*v);
    if (auto v = string(*g, "GicGroup"))             meta.setGicGroup(*v);
    if (auto v = string(*g, "GicIndustry"))          meta.setGicIndustry(*v);
    if (auto v = string(*g, "GicSubIndustry"))       meta.setGicSubIndustry(*v);
    if (auto v = string(*g, "HomeCategory"))         meta.setHomeCategory(*v);
    if (auto v = string(*g, "Description"))          meta.setDescription(*v);
    if (auto v = object(*g,  "Officers"))            meta.setOfficers(*v);
    if (auto v = string(*g, "Phone"))                meta.setPhoneNumber(*v);
    if (auto v = string(*g, "WebURL"))               meta.setWebsite(QUrl(*v));
    if (auto v = string(*g, "LogoURL"))              meta.setLogoLink(QUrl(*v));
    if (auto v = integer(*g,"FullTimeEmployees"))    meta.setFulltimeEmployees(*v);

    // Listings (додаткові лістинги)
    if (auto Ls = object(*g, "Listings")) {
        for (auto it = Ls->begin(); it != Ls->end(); ++it) {
            if (!it->isObject()) continue;
            const QJsonObject li = it->toObject();
            auto code = string(li, "Code");
            auto ven  = string(li, "Exchange");
            if (!code || !ven) continue;
            finded.instrument->data()->addTicker(sdk::Symbol(*code, *ven));
        }
    }

    // NOTE Поля без прямих сеттерів тут пропускаємо свідомо:
    // ISIN / Name / Type / Code / Exchange / Currency* / PrimaryTicker / UpdatedAt
}

void sdk::api::Eod::_handleInfoHighlights(const sdk::market::Finder& finded,
                                          const QJsonObject& obj)
{
    auto h = json::object(obj, "Highlights");
    if (not h) return;

    sdk::Dividend& dividend = finded->dividend;
    sdk::Finance&  finance  = finded.instrument->data()->finance;
    sdk::Fundamental& fund = finance.fundamental;
    sdk::Estimate&    est  = finance.estimates;
    using namespace json;

    // Fundamental
    if (auto v = real(*h, "BookValue"))         fund.setBookValue(*v);
    if (auto v = integer(*h, "EBITDA"))         fund.setEbitda(*v);
    if (auto v = integer(*h, "RevenueTTM"))     fund.setRevenueTtm(*v);
    if (auto v = integer(*h, "GrossProfitTTM")) fund.setGrossProfitTtm(*v);

    // Dividend
    if (auto v = real(*h, "DividendShare")) dividend.setPerShare(*v);
    if (auto v = real(*h, "DividendYield")) dividend.setForwardAnnualYield(*v);

    // Estimates
    if (auto v = real(*h, "WallStreetTargetPrice"))     est.setWallStreetTargetPrice(*v);
    if (auto v = real(*h, "EPSEstimateCurrentYear"))    est.setEpsEstimateCurrentYear(*v);
    if (auto v = real(*h, "EPSEstimateNextYear"))       est.setEpsEstimateNextYear(*v);
    if (auto v = real(*h, "EPSEstimateNextQuarter"))    est.setEpsEstimateNextQuart(*v);
    if (auto v = real(*h, "EPSEstimateCurrentQuarter")) est.setEpsEstimateCurrentQuart(*v);
    if (auto v = date(*h, "MostRecentQuarter"))         est.setRecentQuart(*v);

    // --- TODO (свідомо не чіпаємо тут) ---
    // MarketCapitalization / MarketCapitalizationMln, PERatio,
    // PEGRatio → Valuation (коли зафіксуємо API)
    // RevenuePerShareTTM, ProfitMargin, OperatingMarginTTM,
    // ROA/ROE, QoQ/YoY growth → або Derived/Trend
    // DilutedEpsTTM / EarningsShare → додамо,
    // коли підтвердимо ім'я сеттера у Fundamental/Derived
}

void sdk::api::Eod::_handleInfoValuation(const sdk::market::Finder& finded, const QJsonObject& obj)
{
    auto v = json::object(obj, "Valuation");
    if (not v) return;

    // if (auto x = json::real(*v, "TrailingPE"))            val.setTrailingPe(*x);
    // if (auto x = json::real(*v, "ForwardPE"))             val.setForwardPe(*x);
    // if (auto x = json::real(*v, "PriceSalesTTM"))         val.setPriceSalesTtm(*x);
    // if (auto x = json::real(*v, "PriceBookMRQ"))          val.setPriceBookMrq(*x);
    // if (auto x = json::real(*v,"EnterpriseValue"))        val.setEnterpriseValue(*x);
    // if (auto x = json::real(*v, "EnterpriseValueRevenue"))val.setEnterpriseValueRevenue(*x);
    // if (auto x = json::real(*v, "EnterpriseValueEbitda")) val.setEnterpriseValueEbitda(*x);
}

void sdk::api::Eod::_handleInfoSharesStats(const sdk::market::Finder& finded,
                                           const QJsonObject& obj)
{
    auto s = json::object(obj, "SharesStats");
    if (not s) return;

    sdk::Capital&         capital = finded.instrument->data()->finance.capital;
    sdk::ShortInterest&   shorts  = finded->short_interst;

    if (auto x = json::real(*s, "SharesOutstanding"))   capital.setSharesOutstanding(*x);
    if (auto x = json::real(*s, "SharesFloat"))         capital.setSharesFloat(*x);
    if (auto x = json::real(*s, "PercentInsiders"))     capital.setPercentOfInsiders(*x);
    if (auto x = json::real(*s, "PercentInstitutions")) capital.setPercentOfInsitution(*x);

    if (auto x = json::integer(*s, "SharesShort"))           shorts.setShortInterestShares(*x);
    if (auto x = json::integer(*s, "SharesShortPriorMonth")) shorts.setSharesPriorMonth(*x);
}

void sdk::api::Eod::_handleInfoSplitsDividends(const sdk::market::Finder& finded,
                                               const QJsonObject& obj)
{
    auto sd = json::object(obj, "SplitsDividends");
    if (not sd) return;

    sdk::Dividend&        dividend = finded->dividend;
    sdk::CorporateAction& c_action = finded->corp_action;
    using namespace json;

    // Dividend basics
    if (auto v = real(*sd, "ForwardAnnualDividendRate"))  dividend.setPerShare(*v);
    if (auto v = real(*sd, "ForwardAnnualDividendYield")) dividend.setForwardAnnualYield(*v);
    if (auto v = real(*sd, "PayoutRatio"))                dividend.setPayoutRatio(*v);
    if (auto v = date(*sd, "DividendDate"))               dividend.setDate(*v);
    if (auto v = date(*sd, "ExDividendDate"))             dividend.setExDate(*v);

    // Corporate actions (splits)
    if (auto v = date  (*sd, "LastSplitDate")) c_action.setLastSplitDate(*v);
    if (auto v = string(*sd, "LastSplitFactor"))
        c_action.setLastSplitFactor((*v).split(":")[0].toInt(), (*v).split(":")[1].toInt());

    // TODO maybe add NumberDividendsByYear
    // if (auto obj = json::object(*sd, "NumberDividendsByYear")) {
    //     QMap<int,int> perYear;
    //     for (auto it = obj->begin(); it != obj->end(); ++it) {
    //         if (it->isObject()) {
    //             const QJsonObject o = it->toObject();
    //             auto y = json::integer(o, "Year");
    //             auto c = json::integer(o, "Count");
    //             if (y && c) perYear[*y] = *c;
    //         }
    //     }
    //     if (!perYear.isEmpty())
    //         div.setPerYear(perYear);
    // }
}

void sdk::api::Eod::_handleInfoAnalystRatings(const sdk::market::Finder& finded,
                                              const QJsonObject& obj)
{
    auto ar = json::object(obj, "AnalystRatings");
    if (not ar) return;

    sdk::Estimate& est = finded.instrument->data()->finance.estimates;

    if (auto v = json::real(*ar, "Rating"))      est.setAnalystConsensusRate(*v);
    if (auto v = json::real(*ar, "TargetPrice")) est.setWallStreetTargetPrice(*v);

    if (auto v = json::integer(*ar, "StrongBuy"))  est.setAnalystStrongBuy(*v);
    if (auto v = json::integer(*ar, "Buy"))        est.setAnalystBuy(*v);
    if (auto v = json::integer(*ar, "Hold"))       est.setAnalystHold(*v);
    if (auto v = json::integer(*ar, "Sell"))       est.setAnalystSell(*v);
    if (auto v = json::integer(*ar, "StrongSell")) est.setAnalystStrongSell(*v);

}

void sdk::api::Eod::_handleInfoOutstandingShares(const sdk::market::Finder& finded,
                                                 const QJsonObject& obj)
{
    auto os = json::object(obj, "outstandingShares");
    if (not os) return;

    auto q = json::object(*os, "quartely");
    if (not q) return;

    sdk::Capital& capital = finded.instrument->data()->finance.capital;

    QDate date;
    int64_t shares;
    for (auto it = q->begin(); it != q->end(); ++it){
        if (!it->isObject()) continue;
        auto d = json::date(it->toObject(), "dateFormatted");
        auto s = json::integer(it->toObject(), "shares");
        if (d && s)
            capital.setOutstandShare(*s, *d);
    }
}

void sdk::api::Eod::_handleInfoEarnings(const sdk::market::Finder& finded,
                                        const QJsonObject& obj)
{
    auto e = json::object(obj, "Earnings");
    if (not e) return;

    _handleInfoEarningsHistory(finded, *e);
    _handleInfoEarningsTrend(finded, *e);
}

void sdk::api::Eod::_handleInfoEarningsHistory(const sdk::market::Finder& finded,
                                               const QJsonObject& obj)
{
    auto h = json::object(obj, "History");
    if (not h) return;

    sdk::Finance& finance = finded.instrument->data()->finance;

    for (auto it = h->begin(); it != h->end(); ++it){
        if (!it->isObject()) continue;
        const QJsonObject o = it->toObject();
        auto periodEnd  = json::date(o, "date");
        if (not periodEnd) continue;

        auto q = finance.quartel(*periodEnd);
        if (auto v = json::date(o, "reportDate"))  q.earning.setReportDate(*v);
        if (auto v = json::real(o, "epsActual"))   q.earning.setEpsActual(*v);
        if (auto v = json::real(o, "epsEstimate")) q.earning.setEpsEstimate(*v);
        if (auto v = json::string(o, "beforeAfterMarket"))
            q.earning.setBeforeMarket(*v == "BeforeMarket"); // FIXME before
    }
}

void sdk::api::Eod::_handleInfoEarningsTrend(const sdk::market::Finder& finded,
                                             const QJsonObject& obj)
{
    auto t = json::object(obj, "Trend");
    if (not t) return;

    sdk::Finance& finance = finded.instrument->data()->finance;
    using namespace json;

    for (auto it = t->begin(); it != t->end(); ++it){
        if (!it->isObject()) continue;
        const QJsonObject o = it->toObject();
        auto periodEnd  = json::date(o, "date");
        if (not periodEnd) continue;

        auto t = finance.quartel(*periodEnd).trend;
        if (auto v = real(o, "earningsEstimateAvg"))        t.setEarningsAvg(*v);
        if (auto v = real(o, "earningsEstimateLow"))        t.setEarningsLow(*v);
        if (auto v = real(o, "earningsEstimateHigh"))       t.setEarningsHigh(*v);
        if (auto v = real(o, "earningsEstimateYearAgoEps")) t.setEarningsYearAgoEps(*v);
        if (auto v = real(o, "earningsEstimateNumberOfAnalysts"))
            t.setEarningsAnalystsCount(*v);

        if (auto v = real(o, "revenueEstimateAvg"))        t.setRevenueAvg(*v);
        if (auto v = real(o, "revenueEstimateLow"))        t.setRevenueLow(*v);
        if (auto v = real(o, "revenueEstimateHigh"))       t.setRevenueHigh(*v);
        if (auto v = real(o, "revenueEstimateYearAgoEps")) t.setRevenueYearAgoEps(*v);
        if (auto v = real(o, "revenueEstimateNumberOfAnalysts"))
            t.setRevenueAnalystsCount(*v);

        if (auto v = real(o, "epsTrendCurrent"))   t.setEpsTrendCurrent(*v);
        if (auto v = real(o, "epsTrend7daysAgo"))  t.setEpsTrend7dAgo(*v);
        if (auto v = real(o, "epsTrend30daysAgo")) t.setEpsTrend30dAgo(*v);
        if (auto v = real(o, "epsTrend60daysAgo")) t.setEpsTrend60dAgo(*v);
        if (auto v = real(o, "epsTrend90daysAgo")) t.setEpsTrend90dAgo(*v);

        if (auto v = real(o, "epsRevisionsUpLast7days"))   t.setEpsRevisionUpLast7d(*v);
        if (auto v = real(o, "epsRevisionsUpLast30days"))  t.setEpsRevisionUpLast30d(*v);
        if (auto v = real(o, "epsRevisionsDownLast7days")) t.setEpsRevisionDownLast7d(*v);
        if (auto v = real(o, "epsRevisionsDownLast30days"))t.setEpsRevisionDownLast30d(*v);
    }
}

void sdk::api::Eod::_handleInfoFinancials(const sdk::market::Finder& finded,
                                          const QJsonObject& obj)
{
    auto e = json::object(obj, "Financials");
    if (not e) return;

    _handleInfoFinancialsBalance (finded, *e);
    _handleInfoFinancialsCashFlow(finded, *e);
    _handleInfoFinancialsIncome  (finded, *e);
}

void sdk::api::Eod::_handleInfoFinancialsBalance(const sdk::market::Finder& finded,
                                                 const QJsonObject& obj)
{
    auto h = json::object(obj, "Balance_Sheet");
    if (not h)
        return;

    sdk::Finance& finance = finded.instrument->data()->finance;
    using namespace json;

    for (auto it = h->begin(); it != h->end(); ++it){
        if (!it->isObject()) continue;
        const QJsonObject o = it->toObject();
        auto periodEnd  = json::date(o, "date");
        if (not periodEnd) continue;

        auto b = finance.quartel(*periodEnd).balance;
        // meta
        if (auto v = date(o, "filing_date")) b.setFilingDate(*v);

        // assets
        if (auto v = real(o, "totalAssets"))            b.setTotalAssets(*v);
        if (auto v = real(o, "cash"))                   b.setCash(*v);
        if (auto v = real(o, "cashAndEquivalents"))     b.setCashAndCashEquivalents(*v);
        if (auto v = real(o, "shortTermInvestments"))   b.setShortTermInvestments(*v);
        if (auto v = real(o, "netReceivables"))         b.setNetReceivables(*v);
        if (auto v = real(o, "inventory"))              b.setInventory(*v);
        if (auto v = real(o, "otherCurrentAssets"))     b.setOtherCurrentAssets(*v);
        if (auto v = real(o, "totalCurrentAssets"))     b.setTotalCurrentAssets(*v);
        if (auto v = real(o, "longTermInvestments"))    b.setLongTermInvestments(*v);
        if (auto v = real(o, "propertyPlantEquipment")) b.setPropertyPlantAndEquipment(*v);
        if (auto v = real(o, "propertyPlantAndEquipmentGross"))
            b.setPropertyPlantAndEquipmentGross(*v);

        if (auto v = real(o, "propertyPlantAndEquipmentNet"))
            b.setPropertyPlantAndEquipmentNet(*v);

        if (auto v = real(o, "accumulatedDepreciation")) b.setAccumulatedDepreciation(*v);
        if (auto v = real(o, "intangibleAssets"))        b.setIntangibleAssets(*v);
        if (auto v = real(o, "goodWill"))                b.setGoodwill(*v);
        if (auto v = real(o, "otherAssets"))             b.setOtherNonCurrentAssets(*v);
        if (auto v = real(o, "nonCurrentAssetsOther"))   b.setNonCurrentAssetsOther(*v);
        if (auto v = real(o, "nonCurrentAssetsTotal"))   b.setNonCurrentAssetsTotal(*v);
        if (auto v = real(o, "accumulatedAmortization")) b.setAccumulatedAmortization(*v);
        if (auto v = real(o, "netTangibleAssets"))       b.setNetTangibleAssets(*v);
        if (auto v = real(o, "deferredLongTermAssetCharges"))
            b.setDeferredLongTermAssetCharges(*v);

        // liabilities
        if (auto v = real(o, "accountsPayable"))            b.setAccountsPayable(*v);
        if (auto v = real(o, "otherCurrentLiab"))           b.setOtherCurrentLiabilities(*v);
        if (auto v = real(o, "totalCurrentLiabilities"))    b.setTotalCurrentLiabilities(*v);
        if (auto v = real(o, "currentDeferredRevenue"))     b.setCurrentDeferredRevenue(*v);
        if (auto v = real(o, "shortTermDebt"))              b.setShortTermDebt(*v);
        if (auto v = real(o, "longTermDebt"))               b.setLongTermDebt(*v);
        if (auto v = real(o, "longTermDebtTotal"))          b.setLongTermDebtTotal(*v);
        if (auto v = real(o, "totalLiab"))                  b.setTotalLiabilities(*v);
        if (auto v = real(o, "otherLiab"))                  b.setOtherLiabilities(*v);
        if (auto v = real(o, "capitalLeaseObligations"))    b.setCapitalLeaseObligations(*v);
        if (auto v = real(o, "warrants"))                   b.setWarrants(*v);
        if (auto v = real(o, "preferredStockRedeemable"))   b.setRedeemablePreferredStock(*v);
        if (auto v = real(o, "negativeGoodwill"))           b.setNegativeGoodwill(*v);
        if (auto v = real(o, "deferredLongTermLiab"))       b.setDeferredLongTermLiabilities(*v);
        if (auto v = real(o, "shortLongTermDebt"))          b.setCurrentPortionOfLongTermDebt(*v);
        if (auto v = real(o, "nonCurrentLiabilitiesOther")) b.setNonCurrentLiabilitiesOther(*v);
        if (auto v = real(o, "nonCurrentLiabilitiesTotal")) b.setNonCurrentLiabilitiesTotal(*v);

        // equity
        if (auto v = real(o, "totalStockholderEquity"))   b.setTotalShareholdersEquity(*v);
        if (auto v = real(o, "liabilitiesAndStockholdersEquity"))
            b.setLiabilitiesAndShareholdersEquity(*v);

        if (auto v = real(o, "commonStock"))             b.setCommonStock(*v);
        if (auto v = real(o, "capitalStock"))            b.setCapitalStock(*v);
        if (auto v = real(o, "retainedEarnings"))        b.setRetainedEarnings(*v);
        if (auto v = real(o, "otherStockholderEquity"))  b.setOtherShareholdersEquity(*v);
        if (auto v = real(o, "commonStockTotalEquity"))  b.setCommonStockTotalEquity(*v);
        if (auto v = real(o, "treasuryStock"))           b.setTreasuryStock(*v);
        if (auto v = real(o, "additionalPaidInCapital")) b.setAdditionalPaidInCapital(*v);
        if (auto v = real(o, "totalPermanentEquity"))    b.setTotalPermanentEquity(*v);
        if (auto v = real(o, "preferredStockTotalEquity"))   b.setPreferredStockTotalEquity(*v);
        if (auto v = real(o, "retainedEarningsTotalEquity")) b.setRetainedEarningsTotalEquity(*v);
        if (auto v = real(o, "noncontrollingInterestInConsolidatedEntity"))
            b.setNoncontrollingInterest(*v);

        if (auto v = real(o, "temporaryEquityRedeemableNoncontrollingInterests"))
            b.setRedeemableNoncontrolInterests(*v);

        if (auto v = real(o, "accumulatedOtherComprehensiveIncome"))
            b.setOtherComprehensIncomeAccumulat(*v);
    }
}

void sdk::api::Eod::_handleInfoFinancialsCashFlow(const sdk::market::Finder& finded,
                                                  const QJsonObject& obj)
{
    auto cf = json::object(obj, "Cash_Flow");
    if (not cf)
        return;

    auto q = json::object(*cf, "quarterly");
    if (not q)
        return;

    sdk::Finance& finance = finded.instrument->data()->finance;
    using namespace json;

    for (auto it = q->begin(); it != q->end(); ++it) {
        if (!it->isObject()) continue;
        const QJsonObject o = it->toObject();

        auto periodEnd = json::date(o, "date");
        if (!periodEnd) continue;

        auto& c = finance.quartel(*periodEnd).cashflow;

        // meta
        if (auto v = json::date(o, "filing_date")) c.setFilingDate(*v);

        // investing
        if (auto v = integer(o, "investments")) c.setInverstinNetsInvestment(*v);
        if (auto v = integer(o, "totalCashflowsFromInvestingActivities")) c.setInvestingTotal(*v);
        if (auto v = integer(o, "capitalExpenditures")) c.setInvestingCapitalSpending(*v);
        if (auto v = integer(o, "otherCashflowsFromInvestingActivities")) c.setInvestingOther(*v);

        // operating
        if (auto v = integer(o, "depreciation")) c.setDepreciation(*v);
        if (auto v = integer(o, "netIncome"))    c.setNetIncome(*v);
        if (auto v = integer(o, "totalCashFromOperatingActivities")) c.setOperatingTotal(*v);
        if (auto v = integer(o, "stockBasedCompensation"))      c.setStockBasedCompensation(*v);
        if (auto v = integer(o, "changeToOperatingActivities")) c.setOperatingOtherAdjustments(*v);
        if (auto v = integer(o, "changeToLiabilities"))        c.setOperatingChangeLiabilities(*v);
        if (auto v = integer(o, "changeToInventory"))          c.setOperatingChangeInventory(*v);
        if (auto v = integer(o, "changeToAccountReceivables")) c.setOperatingChangeReceivables(*v);
        if (auto v = integer(o, "changeReceivables"))          c.setOperatingChangeReceivables(*v);
        if (auto v = integer(o, "cashFlowsOtherOperating")) c.setOperatingOther(*v);
        if (auto v = integer(o, "otherNonCashItems"))       c.setOperatingNoncashOther(*v);
        if (auto v = integer(o, "changeToNetincome"))       c.setOperatingNetIncomeAdjust(*v);

        // financing
        if (auto v = integer(o, "netBorrowings"))          c.setFinancingNetBorrowings(*v);
        if (auto v = integer(o, "salePurchaseOfStock"))    c.setFinancingBuybackNet(*v);
        if (auto v = integer(o, "dividendsPaid"))          c.setFinancingDividendsPaid(*v);
        if (auto v = integer(o, "issuanceOfCapitalStock")) c.setFinancingEquityIssuance(*v);

        if (auto v = integer(o, "otherCashflowsFromFinancingActivities")) c.setFinancingOther(*v);

        // derived / getters-only у CashFlow → НЕ сетимо:
        // - changeInCash / cashBegin / cashEnd / cashEquivalentsChange /
        // fxEffectOnCash / workingCapitalChange / financingTotal
        // :contentReference[oaicite:2]{index=2}

        // додатково (необов'язково): вхідні begin/end cash, FCF
        if (auto v = integer(o, "freeCashFlow")) c.setFreeCash(*v);
        // beginPeriodCashFlow / endPeriodCashFlow присутні в JSON,
        // але сеттерів немає (тільки гетери)  :contentReference[oaicite:3]{index=3}
    }
}

void sdk::api::Eod::_handleInfoFinancialsIncome(const sdk::market::Finder& finded,
                                                const QJsonObject& obj)
{
    auto is = json::object(obj, "Income_Statement");
    if (not is)
        return;

    auto q = json::object(*is, "quarterly");
    if (not q)
        return;

    sdk::Finance& finance = finded.instrument->data()->finance;
    using namespace json;

    for (auto it = q->begin(); it != q->end(); ++it) {
        if (!it->isObject()) continue;
        const QJsonObject o = it->toObject();

        auto periodEnd = json::date(o, "date");
        if (!periodEnd) continue;

        auto& inc = finance.quartel(*periodEnd).incomes;

        // meta
        if (auto v = json::date(o, "filing_date")) inc.setFilingDate(*v);

        // revenue / cogs / gross
        if (auto v = integer(o, "totalRevenue"))  inc.setTotalRevenue(*v);
        if (auto v = integer(o, "costOfRevenue")) inc.setCostOfRevenue(*v);

        // opex
        if (auto v = integer(o, "researchDevelopment"))          inc.setResearchDevelopment(*v);
        if (auto v = integer(o, "sellingGeneralAdministrative")) inc.setSellingGeneralAdmin(*v);
        if (auto v = integer(o, "sellingAndMarketingExpenses"))  inc.setSellingMarketingExpens(*v);
        if (auto v = integer(o, "otherOperatingExpenses"))       inc.setOtherOperatingExpenses(*v);

        // D&A
        if (auto v = integer(o, "depreciationAndAmortization")) inc.setDeprAmortization(*v);
        if (auto v = integer(o, "reconciledDepreciation"))      inc.setReconciledDepreciation(*v);

        // interest & other income/expense
        if (auto v = integer(o, "interestIncome"))  inc.setInterestIncome(*v);
        if (auto v = integer(o, "interestExpense")) inc.setInterestExpense(*v);

        if (auto v = integer(o, "totalOtherIncomeExpenseNet"))
            inc.setOtherIncomeExpenseNet(*v);
        else if (auto v = integer(o, "nonOperatingIncomeNetOther"))
            inc.setOtherIncomeExpenseNet(*v);

        // tax
        if (auto v = integer(o, "incomeTaxExpense"))  inc.setIncomeTaxExpense(*v);
        else if (auto v = integer(o, "taxProvision")) inc.setIncomeTaxExpense(*v);

        // bottom line
        if (auto v = integer(o, "netIncomeFromContinuingOps")) inc.setNetIncomeContOps(*v);
        if (auto v = integer(o, "discontinuedOperations"))     inc.setDiscontinuedOps(*v);
        if (auto v = integer(o, "minorityInterest"))           inc.setMinorityInterest(*v);
        if (auto v = integer(o, "netIncome"))                  inc.setNetIncome(*v);

        // adjustments
        if (auto v = integer(o, "preferredStockAndOtherAdjustments"))
            inc.setPreferredAdjustments(*v);

        if (auto v = integer(o, "effectOfAccountingCharges"))
            inc.setAccountingEffects(*v);
    }
}
