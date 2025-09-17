#include "api/eod.h"
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonArray>
#include "instrument/instrument.h"
#include "loader.h"

namespace json {
    std::optional <QString> string(const QJsonObject& obj, const char* key) {
        auto it = obj.find(key);
        if (it == obj.end() || !it->isString()) return std::nullopt;
        const QString v = it->toString().trimmed();
        return v.isEmpty() ? std::nullopt : std::optional<QString>(v);
    };

    std::optional <bool> boolean(const QJsonObject& obj, const char* key) {
        auto it = obj.find(key);
        if (it == obj.end()) return std::nullopt;
        if (it->isBool()) return it->toBool();
        if (it->isString()) {
            const auto v = it->toString().trimmed().toLower();
            if (v == "true" || v == "1")  return true;
            if (v == "false"|| v == "0")  return false;
        }
        return std::nullopt;
    };

    std::optional <QDate> date(const QJsonObject& obj, const char* key,
                              const char* fmt = "yyyy-MM-dd") {
        auto it = obj.find(key);
        if (it == obj.end() || !it->isString()) return std::nullopt;
        const QDate dt = QDate::fromString(it->toString(), fmt);
        return dt.isValid() ? std::optional<QDate>(dt) : std::nullopt;
    };

    std::optional <QJsonObject> object(const QJsonObject& obj, const char* key) {
        auto it = obj.find(key);
        return (it != obj.end() && it->isObject()) ? std::optional <QJsonObject>(it->toObject())
                                                   : std::nullopt;
    };

    std::optional <int64_t> integer(const QJsonObject& obj, const char* key) {
        auto it = obj.find(key);
        if (it == obj.end()) return std::nullopt;
        if (it->isDouble()) return it->toInteger();
        if (it->isString()) {
            bool ok = false;
            int v = it->toString().remove(',').toLongLong(&ok);
            if (ok) return v;
        }
        return std::nullopt;
    };

    std::optional <double> real(const QJsonObject& obj, const char* key) {
        auto it = obj.find(key);
        if (it == obj.end()) return std::nullopt;
        if (it->isDouble()) return it->toDouble();
        if (it->isString()) {
            bool ok = false;
            double d = it->toString().toDouble(&ok);
            if (ok) return d;
        }
        return std::nullopt;
    };
}

api::Eod::Eod(QObject* parent) : API(QUrl("https://eodhd.com/api"), parent)
{
    //
}

api::Eod* api::Eod::instance()
{
    static Eod* _instance = nullptr;
    if (_instance == nullptr)
        _instance = new Eod(qApp);

    return _instance;
}

void api::Eod::get_all_tag(QString exchange)
{
    Eod* data = Eod::instance();
    data->_request(Request::Exchange, exchange);
}

void api::Eod::get_all_exchange_tag()
{
    Eod* data = Eod::instance();
    QStringList list = sdk::exchange::all_exchange_venue();
    for (const auto& it : std::as_const(list))
        data->_request(Request::Exchange, it);
}

void api::Eod::fundamental(const sdk::Symbol& tag)
{
    Eod::instance()->_request(Request::Info, tag);
}

void api::Eod::historical_year(const sdk::Symbol& tag, int8_t year, char period)
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
    Eod::instance()->_request(Request::Candle, tag, params);
}

bool api::Eod::_request(Request type, const QString& name,
                        const sdk::Symbol& symbol, StringMap keys)
{
    Reply* post = _add(type);

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
            post->addQueryItem("api_token", settings::network()->key_eod());
            post->addQueryItem("fmt", "json");
            break;
        }
        case api::Request::Candle: {
            // https://eodhd.com/api/eod/MCD.US?api_token=683ebb8bc59b60.11043967&fmt=json
            post->addQueryItem("api_token", settings::network()->key_eod());
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

void api::Eod::_handler_answer(Reply* reply)
{
    qDebug() << "handler answer eod";

    switch (reply->type()){
        case api::Request::Exchange: { _handle_exchange(reply); break; }
        case api::Request::Candle:   { _handle_candle  (reply); break; }
        case api::Request::Info:     { _handle_info    (reply); break; }
        default:;
    }
}

void api::Eod::_handler_error(Reply* reply, QNetworkReply::NetworkError error)
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
            auto ticker = Nexus.market()->find_ticker(reply->symbol);
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

void api::Eod::_handle_exchange(Reply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->receive_data());
    auto market = Nexus.market();
    QJsonArray root = doc.array();
    for (const auto& it : std::as_const(root)){
        QJsonObject obj = it.toObject();
        Nexus.market()->add_ticker(sdk::Symbol(obj["Code"].toString(), reply->name),
                                   obj["Isin"].toString().toLatin1(),
                                   obj["Name"].toString(),
                                   sdk::instype::from_string(obj["Type"].toString()));
    }

    // handle exchange queue list
    // TODO found main ticker function need to call
    // if (not _queue_contains(Request::Exchange))
        // Nexus.market()->detect_main_ticker();

    market->save_meta();
}

void api::Eod::_handle_candle(Reply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->receive_data());
    auto ticker = Nexus.market()->find_ticker(reply->symbol);
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

void api::Eod::_handle_info(Reply* reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->receive_data());
    qDebug() << Q_FUNC_INFO;
    auto ticker = Nexus.market()->find_ticker(reply->symbol);
    if (ticker.ensure() == false) {
        qDebug() << reply->symbol << "not found";
        return;
    }

    // helpers ---------------------------------------------------------
    auto jnum = [](const QJsonValue& v) -> double {
        if (v.isDouble()) return v.toDouble();
        if (v.isString()) {
            QString s = v.toString();
            s.remove(','); // "1,234.56" -> "1234.56"
            bool ok=false; double x = s.toDouble(&ok);
            return ok ? x : std::numeric_limits<double>::quiet_NaN();
        }
        return std::numeric_limits<double>::quiet_NaN();
    };

    auto jdate = [](const QJsonValue& v) -> QDate {
        return QDate::fromString(v.toString(), "yyyy-MM-dd");
    };

    auto jstr = [](const QJsonValue& v) -> QString {
        return v.isString() ? v.toString() : QString();
    };

    sdk::Instrument* in = ticker->instrument();
    QJsonObject root = doc.object();

    sdk::Legal&           legal    = in->data()->legal;
    sdk::Meta&            meta     = in->data()->meta;
    sdk::Finance&         finance  = in->data()->finance;
    sdk::Fundamental&     fund     = finance.fundamental;
    sdk::Estimate&        est      = finance.estimates;
    sdk::Capital&         capital  = finance.capital;
    sdk::Dividend&        dividend = ticker->dividend;
    sdk::Quotes&          quotes   = ticker->quotes;
    sdk::ShortInterest&   shorts   = ticker->short_interst;
    sdk::CorporateAction& c_action = ticker->corp_action;
    sdk::Valuation&       val      = ticker->valuation;

    using namespace json;

    // ----------------------- General ------------------------------------------------------------
    if (auto g = json::object(root, "General")){
        // Legal
        if (auto v = json::string(*g, "OpenFigi"))             legal.setOpenFigi(*v);
        if (auto v = json::string(*g, "LEI"))                  legal.setLei(*v);
        if (auto v = json::string(*g, "CIK"))                  legal.setCik(*v);
        if (auto v = json::string(*g, "CUSIP"))                legal.setCusip(*v);
        if (auto v = json::string(*g, "EmployerIdNumber"))     legal.setEmployerIdNumber(*v);
        if (auto v = json::date   (*g, "IPODate"))             legal.setIpo(*v);
        if (auto v = json::boolean(*g, "IsDelisted"))          legal.setDelisted(*v);
        if (auto v = json::string (*g, "Address"))             legal.setAddress(*v);
        if (auto v = json::object (*g, "AddressData"))         legal.setAddress(*v);

        // FiscalYearEnd: мапимо англ. назву місяця у sdk::Month локально, без дод. функцій
        if (auto v = json::string(*g, "FiscalYearEnd"))
            legal.setFiscalYearEnd(sdk::month::from_string(*v));

        // Meta
        if (auto v = json::string(*g, "Sector"))               meta.setSector(*v);
        if (auto v = json::string(*g, "Industry"))             meta.setIndustry(*v);
        if (auto v = json::string(*g, "GicSector"))            meta.setGicSector(*v);
        if (auto v = json::string(*g, "GicGroup"))             meta.setGicGroup(*v);
        if (auto v = json::string(*g, "GicIndustry"))          meta.setGicIndustry(*v);
        if (auto v = json::string(*g, "GicSubIndustry"))       meta.setGicSubIndustry(*v);
        if (auto v = json::string(*g, "HomeCategory"))         meta.setHomeCategory(*v);
        if (auto v = json::string(*g, "Description"))          meta.setDescription(*v);
        if (auto v = json::object(*g,  "Officers"))            meta.setOfficers(*v);
        if (auto v = json::string(*g, "Phone"))                meta.setPhoneNumber(*v);
        if (auto v = json::string(*g, "WebURL"))               meta.setWebsite(QUrl(*v));
        if (auto v = json::string(*g, "LogoURL"))              meta.setLogoLink(QUrl(*v));
        if (auto v = json::integer(*g,"FullTimeEmployees"))    meta.setFulltimeEmployees(*v);

        // Listings (додаткові лістинги)
        if (auto Ls = json::object(*g, "Listings")) {
            for (auto it = Ls->begin(); it != Ls->end(); ++it) {
                if (!it->isObject()) continue;
                const QJsonObject li = it->toObject();
                auto code = json::string(li, "Code");
                auto ven  = json::string(li, "Exchange");
                if (!code || !ven) continue;
                in->data()->addTicker(sdk::Symbol(*code, *ven));
            }
        }

        // NOTE Поля без прямих сеттерів тут пропускаємо свідомо:
        // ISIN / Name / Type / Code / Exchange / Currency* / PrimaryTicker / UpdatedAt
    }
    // ============================================================================================

    // ----------------------- Highlights ---------------------------------------------------------
    if (auto h = json::object(root, "Highlights")) {
        auto& finance = in->data()->finance;

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
    // ============================================================================================

    // ----------------------- Valuation ----------------------------------------------------------
    if (auto v = json::object(root, "Valuation")) {
        // if (auto x = json::real(*v, "TrailingPE"))            val.setTrailingPe(*x);
        // if (auto x = json::real(*v, "ForwardPE"))             val.setForwardPe(*x);
        // if (auto x = json::real(*v, "PriceSalesTTM"))         val.setPriceSalesTtm(*x);
        // if (auto x = json::real(*v, "PriceBookMRQ"))          val.setPriceBookMrq(*x);
        // if (auto x = json::real(*v,"EnterpriseValue"))        val.setEnterpriseValue(*x);
        // if (auto x = json::real(*v, "EnterpriseValueRevenue"))val.setEnterpriseValueRevenue(*x);
        // if (auto x = json::real(*v, "EnterpriseValueEbitda")) val.setEnterpriseValueEbitda(*x);
    }
    // ============================================================================================

    // ----------------------- SharesStats --------------------------------------------------------
    if (auto s = json::object(root, "SharesStats")) {
        if (auto x = real(*s, "SharesOutstanding"))   capital.setSharesOutstanding(*x);
        if (auto x = real(*s, "SharesFloat"))         capital.setSharesFloat(*x);
        if (auto x = real(*s, "PercentInsiders"))     capital.setPercentOfInsiders(*x);
        if (auto x = real(*s, "PercentInstitutions")) capital.setPercentOfInsitution(*x);

        if (auto x = integer(*s, "SharesShort"))           shorts.setShortInterestShares(*x);
        if (auto x = integer(*s, "SharesShortPriorMonth")) shorts.setSharesPriorMonth(*x);
    }
    // ============================================================================================

    // ----------------------- Technicals ---------------------------------------------------------
    // NOTE already have it in other fields
    // ============================================================================================

    // ----------------------- SplitsDividends ----------------------------------------------------
    if (auto sd = json::object(root, "SplitsDividends")) {
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
    // ============================================================================================

    // ----------------------- AnalystRatings -----------------------------------------------------
    if (auto ar = json::object(root, "AnalystRatings"); ar.has_value()) {
        auto& est = in->data()->finance.estimates;

        if (auto v = real(*ar, "Rating"))      est.setAnalystConsensusRate(*v);
        if (auto v = real(*ar, "TargetPrice")) est.setWallStreetTargetPrice(*v);

        if (auto v = integer(*ar, "StrongBuy"))  est.setAnalystStrongBuy(*v);
        if (auto v = integer(*ar, "Buy"))        est.setAnalystBuy(*v);
        if (auto v = integer(*ar, "Hold"))       est.setAnalystHold(*v);
        if (auto v = integer(*ar, "Sell"))       est.setAnalystSell(*v);
        if (auto v = integer(*ar, "StrongSell")) est.setAnalystStrongSell(*v);
    }
    // ============================================================================================

    // ----------------------- Holders ------------------------------------------------------------
    // TODO not save for now
    // ============================================================================================

    // ----------------------- InsiderTransactions ------------------------------------------------
    // TODO not save for now
    // ============================================================================================

    // ----------------------- ESGScores ----------------------------------------------------------
    // NOTE i`ts trash block
    // ============================================================================================

    // ----------------------- outstandingShares --------------------------------------------------
    if (auto os = json::object(root, "outstandingShares")){
        if (auto q = json::object(*os, "quartely")){
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
    }
    // ============================================================================================

    // ----------------------- Earnings -----------------------------------------------------------
    if (auto e = json::object(root, "Earnings")){
        if (auto h = json::object(*e, "History")){
            for (auto it = h->begin(); it != h->end(); ++it){
                if (!it->isObject()) continue;
                const QJsonObject o = it->toObject();
                auto periodEnd  = json::date(o, "date");
                if (not periodEnd) continue;

                auto q = finance.quartel(*periodEnd);
                if (auto v = date(o, "reportDate"))  q.earning.setReportDate(*v);
                if (auto v = real(o, "epsActual"))   q.earning.setEpsActual(*v);
                if (auto v = real(o, "epsEstimate")) q.earning.setEpsEstimate(*v);
                if (auto v = string(o, "beforeAfterMarket"))
                    q.earning.setBeforeMarket(*v == "BeforeMarket"); // FIXME before
            }
        }

        if (auto t = json::object(*e, "Trend")){
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
    }
    // ============================================================================================

    // ----------------------- Financials ---------------------------------------------------------
    if (auto e = json::object(root, "Financials")){
        if (auto h = json::object(*e, "Balance_Sheet")){
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
                if (auto v = real(o, "nonCurrentAssetsOther"))  b.setNonCurrentAssetsOther(*v);
                if (auto v = real(o, "nonCurrentAssetsTotal"))   b.setNonCurrentAssetsTotal(*v);
                if (auto v = real(o, "deferredLongTermAssetCharges"))
                    b.setDeferredLongTermAssetCharges(*v);

                if (auto v = real(o, "accumulatedAmortization")) b.setAccumulatedAmortization(*v);

                if (auto v = real(o, "netTangibleAssets"))       b.setNetTangibleAssets(*v);

                // liabilities
                if (auto v = real(o, "accountsPayable"))         b.setAccountsPayable(*v);
                if (auto v = real(o, "otherCurrentLiab"))        b.setOtherCurrentLiabilities(*v);
                if (auto v = real(o, "totalCurrentLiabilities")) b.setTotalCurrentLiabilities(*v);
                if (auto v = real(o, "currentDeferredRevenue"))  b.setCurrentDeferredRevenue(*v);
                if (auto v = real(o, "shortTermDebt"))           b.setShortTermDebt(*v);
                if (auto v = real(o, "longTermDebt"))            b.setLongTermDebt(*v);
                if (auto v = real(o, "longTermDebtTotal"))       b.setLongTermDebtTotal(*v);
                if (auto v = real(o, "totalLiab"))               b.setTotalLiabilities(*v);
                if (auto v = real(o, "otherLiab"))               b.setOtherLiabilities(*v);
                if (auto v = real(o, "capitalLeaseObligations")) b.setCapitalLeaseObligations(*v);
                if (auto v = real(o, "warrants"))                b.setWarrants(*v);
                if (auto v = real(o, "preferredStockRedeemable"))b.setRedeemablePreferredStock(*v);
                if (auto v = real(o, "negativeGoodwill"))        b.setNegativeGoodwill(*v);
                if (auto v = real(o, "deferredLongTermLiab")) b.setDeferredLongTermLiabilities(*v);
                if (auto v = real(o, "shortLongTermDebt"))   b.setCurrentPortionOfLongTermDebt(*v);
                if (auto v = real(o, "nonCurrentLiabilitiesOther"))
                    b.setNonCurrentLiabilitiesOther(*v);

                if (auto v = real(o, "nonCurrentLiabilitiesTotal"))
                    b.setNonCurrentLiabilitiesTotal(*v);

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
                if (auto v = real(o, "noncontrollingInterestInConsolidatedEntity"))
                    b.setNoncontrollingInterest(*v);

                if (auto v = real(o, "temporaryEquityRedeemableNoncontrollingInterests"))
                    b.setRedeemableNoncontrolInterests(*v);

                if (auto v = real(o, "accumulatedOtherComprehensiveIncome"))
                    b.setOtherComprehensIncomeAccumulat(*v);

                if (auto v = real(o, "preferredStockTotalEquity"))
                    b.setPreferredStockTotalEquity(*v);

                if (auto v = real(o, "retainedEarningsTotalEquity"))
                    b.setRetainedEarningsTotalEquity(*v);
            }
        }

        if (auto cf = json::object(root, "Cash_Flow")) {
            if (auto q = json::object(*cf, "quarterly")) {
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
                    if (auto v = integer(o, "totalCashflowsFromInvestingActivities"))
                        c.setInvestingTotal(*v);

                    if (auto v = integer(o, "capitalExpenditures"))
                        c.setInvestingCapitalSpending(*v);

                    if (auto v = integer(o, "otherCashflowsFromInvestingActivities"))
                        c.setInvestingOther(*v);

                    // operating
                    if (auto v = integer(o, "depreciation")) c.setDepreciation(*v);
                    if (auto v = integer(o, "netIncome")) c.setNetIncome(*v);
                    if (auto v = integer(o, "totalCashFromOperatingActivities"))
                        c.setOperatingTotal(*v);

                    if (auto v = integer(o, "stockBasedCompensation"))
                        c.setStockBasedCompensation(*v);

                    if (auto v = integer(o, "changeToOperatingActivities"))
                        c.setOperatingOtherAdjustments(*v);

                    if (auto v = integer(o, "changeToLiabilities"))
                        c.setOperatingChangeLiabilities(*v);

                    if (auto v = integer(o, "changeToInventory"))
                        c.setOperatingChangeInventory(*v);

                    if (auto v = integer(o, "changeToAccountReceivables"))
                        c.setOperatingChangeReceivables(*v);

                    if (auto v = integer(o, "changeReceivables"))
                        c.setOperatingChangeReceivables(*v);// alias

                    if (auto v = integer(o, "cashFlowsOtherOperating")) c.setOperatingOther(*v);
                    if (auto v = integer(o, "otherNonCashItems")) c.setOperatingNoncashOther(*v);
                    if (auto v = integer(o, "changeToNetincome"))
                        c.setOperatingNetIncomeAdjust(*v);

                    // financing
                    if (auto v = integer(o, "netBorrowings"))      c.setFinancingNetBorrowings(*v);
                    if (auto v = integer(o, "salePurchaseOfStock"))c.setFinancingBuybackNet(*v);
                    if (auto v = integer(o, "dividendsPaid"))      c.setFinancingDividendsPaid(*v);
                    if (auto v = integer(o, "issuanceOfCapitalStock"))
                        c.setFinancingEquityIssuance(*v);

                    if (auto v = integer(o, "otherCashflowsFromFinancingActivities"))
                        c.setFinancingOther(*v);

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
        }

        if (auto is = json::object(root, "Income_Statement"); is.has_value()) {
            if (auto q = json::object(*is, "quarterly"); q.has_value()) {
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
                    if (auto v = integer(o, "researchDevelopment"))
                        inc.setResearchDevelopment(*v);

                    if (auto v = integer(o, "sellingGeneralAdministrative"))
                        inc.setSellingGeneralAdmin(*v);

                    if (auto v = integer(o, "sellingAndMarketingExpenses"))
                        inc.setSellingMarketingExpenses(*v);

                    if (auto v = integer(o, "otherOperatingExpenses"))
                        inc.setOtherOperatingExpenses(*v);

                    // D&A
                    if (auto v = integer(o, "depreciationAndAmortization"))
                        inc.setDeprAmortization(*v);

                    if (auto v = integer(o, "reconciledDepreciation"))
                        inc.setReconciledDepreciation(*v);

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
                    if (auto v = integer(o, "netIncomeFromContinuingOps"))
                        inc.setNetIncomeContOps(*v);

                    if (auto v = integer(o, "discontinuedOperations"))
                        inc.setDiscontinuedOps(*v);

                    if (auto v = integer(o, "minorityInterest"))
                        inc.setMinorityInterest(*v);

                    if (auto v = integer(o, "netIncome"))
                        inc.setNetIncome(*v);

                    // adjustments
                    if (auto v = integer(o, "preferredStockAndOtherAdjustments"))
                        inc.setPreferredAdjustments(*v);

                    if (auto v = integer(o, "effectOfAccountingCharges"))
                        inc.setAccountingEffects(*v);
                }
            }
        }
    }
    // ============================================================================================

    ticker->save();
}
