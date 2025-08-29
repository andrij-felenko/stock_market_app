#include "eod.h"
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonArray>
#include "data/instrument.h"
#include "data/market.h"
#include "loader.h"
#include "api/alphavantage.h"
#include "data/instrument/meta.h"

api::Eod::Eod(QObject* parent) : API(parent)
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
    data->_send(Request::Exchange, exchange);
}

void api::Eod::get_all_exchange_tag()
{
    Eod* data = Eod::instance();
    QStringList list = geo::exchange::all_exchange_venue();
    for (const auto& it : std::as_const(list))
        data->_send(Request::Exchange, it);
}

void api::Eod::fundamental(data::ticker::Symbol tag)
{
    api::StringMap params = { { "tag", tag.full() } };
    Eod::instance()->_send(Request::Info, tag.full_venue());
}

void api::Eod::historical_year(data::ticker::Symbol tag, int8_t year, char period)
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
    Eod::instance()->_send(Request::Candle, tag.full_venue(), params);
}

bool api::Eod::_request(Request type, QString name, StringMap keys)
{
    QString base("https://eodhd.com/api");
    // as we work only with US marker, we nee to cut .US domen from tag

    QUrl url;
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
        case api::Request::Reported: return false;
        case api::Request::Exchange: url = base + "/exchange-symbol-list/" + name; break;
        case api::Request::Candle: url = base + "/eod/" + keys["tag"]; break;
        case api::Request::Info: url = base + "/fundamentals/" + keys["tag"]; break;
    }

    QUrlQuery query;

    switch (type){
        case api::Request::Exchange:
        case api::Request::Info:
        {
            query.addQueryItem("api_token", settings::network()->key_eod());
            query.addQueryItem("fmt", "json");
            break;
        }
        case api::Request::Candle: {
            // https://eodhd.com/api/eod/MCD.US?api_token=683ebb8bc59b60.11043967&fmt=json
            query.addQueryItem("api_token", settings::network()->key_eod());
            query.addQueryItem("period", keys.value("period", "d"));
            query.addQueryItem("fmt", "json");
            query.addQueryItem("from", keys.value("from"));
            query.addQueryItem("to",   keys.value("to"));
            break;
        }
        default:;
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    API::_add_reply(type, _netmanager.get(request), name);
    qDebug() << "request:" << url << _replies.size();
    return true;
}

void api::Eod::_handler_answer(Request type, QByteArray data, QString name, bool stream)
{
    qDebug() << "handler answer eod";
    qDebug() << data;
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // qDebug() << name << "return data" << doc;
    // qDebug() << response;

    switch (type){
    case api::Request::Exchange: {
        _handle_exchange(doc, name);
        break;
    }
    case api::Request::Candle: {
        _handle_candle(doc, name);
        break;
    }
    case api::Request::Info: {
        _handle_info(doc, name);
        break;
    }
    default:;
    }
}

void api::Eod::_handler_error(Request type, QNetworkReply::NetworkError error, QString name)
{
    qDebug() << Q_FUNC_INFO << static_cast <uint32_t> (type) << error << name;
    switch (type){
    case api::Request::Exchange: {
        break;
    }
    case api::Request::Candle: {
        break;
    }
    case api::Request::Info: {
        if (error == QNetworkReply::ContentNotFoundError){
            qDebug() << "Info" << name;
            auto finded = Nexus.market()->find(name);
            if (!finded.has_value()) {
                // Nexus.market()->add(name);
                finded = Nexus.market()->find(name);
                if (!finded.has_value()){
                    qDebug() << "return ffg";
                    return;
                }
            }
            data::Ticker* ticker = finded.value();
            data::Instrument* in = ticker->instrument();
            bool after = false;
            for (const auto& it : in->tickers()){
                qDebug() << after << it;
                if (after){
                    fundamental(it.full());
                    break;
                }
                else {
                    if (it.full() == name)
                        after = true;
                }
            }
        }
        break;
    }
    default:;
    }
}

void api::Eod::_handle_exchange(const QJsonDocument& json, QString name)
{
    auto market = Loader::instance()->market();
    QJsonArray root = json.array();
    for (const auto& it : std::as_const(root)){
        QJsonObject obj = it.toObject();
        data::Meta meta;
        meta.set_isin(obj["Isin"].toString().toLatin1());
        meta.set_title(obj["Name"].toString());
        meta.set_type(geo::instype::from_string(obj["Type"].toString()));
        data::ticker::Symbol symbol(obj["Code"].toString(), name);

        Nexus.market()->ensure(meta, symbol);
    }

    // handle exchange queue list
    if (not _queue_contains(Request::Exchange))
        Nexus.market()->detect_main_ticker();

    market->save_meta();
}

void api::Eod::_handle_candle(const QJsonDocument& json, QString name)
{
    auto finded = Nexus.market()->find(name);
    if (!finded.has_value()) {
        // Nexus.market()->add(name);
        finded = Nexus.market()->find(name);
        if (!finded.has_value())
            return;
    }

    data::Ticker* ticker = finded.value();

    QJsonArray array = json.array();
    for (const QJsonValue& v : std::as_const(array)){
        QJsonObject obj = v.toObject();
        QString dtime   = obj[  "date"].toString();
        float open      = obj[  "open"].toDouble();
        float close     = obj[ "close"].toDouble();
        float high      = obj[  "high"].toDouble();
        float low       = obj[   "low"].toDouble();
        uint64_t volume = obj["volume"].toDouble();

        QDate date = QDate::fromString(dtime, "yyyy-MM-dd");
        ticker->quotes()->setData(date, open, close, high, low, volume);
    }

    ticker->quotes()->recalculate();
    qDebug() << "save 1" << Q_FUNC_INFO << ticker->quotes()->raw_points().size() << ticker << name;
    ticker->save();
    qDebug() << "save 2" << Q_FUNC_INFO << ticker->quotes()->raw_points().size() << ticker << name;
}

void api::Eod::_handle_info(const QJsonDocument& json, QString name)
{
    qDebug() << Q_FUNC_INFO << json;
    auto finded = Nexus.market()->find(name);
    if (!finded.has_value()) {
        finded = Nexus.market()->find(name);
        if (!finded.has_value()){
            qDebug() << "not found";
            return;
        }
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

    // Pick MRQ object from Financials.Balance_Sheet.quarterly (supports object-of-objects or array-of-objs)
    auto pick_mrq = [&](const QJsonObject& finance)->QJsonObject {
        const QJsonObject bs = finance.value("Balance_Sheet").toObject();
        const QJsonValue quarterly = bs.value("quarterly");
        if (quarterly.isObject()) {
            QJsonObject q = quarterly.toObject();
            QString best;
            for (auto it=q.begin(); it!=q.end(); ++it) {
                if (best.isEmpty() || it.key() > best) best = it.key();
            }
            return q.value(best).toObject();
        }
        if (quarterly.isArray()) {
            QJsonArray arr = quarterly.toArray();
            QString bestDate; QJsonObject bestObj;
            for (const QJsonValue& v : std::as_const(arr)) {
                QJsonObject o = v.toObject();
                const QString d = o.value("date").toString();
                if (bestDate.isEmpty() || d > bestDate) { bestDate=d; bestObj=o; }
            }
            return bestObj;
        }
        return {};
    };

    data::Ticker* ticker = finded.value();
    data::Instrument* in = ticker->instrument();
    QJsonObject root = json.object();

    QJsonObject general  = root.value("General").toObject();
    QJsonObject high     = root.value("Highlights").toObject();
    QJsonObject valuation= root.value("Valuation").toObject();
    QJsonObject shares   = root.value("SharesStats").toObject();
    QJsonObject tech     = root.value("Technicals").toObject();
    QJsonObject finance  = root.value("Financials").toObject();
    QJsonObject splits   = root.value("SplitsDividends").toObject();

    qDebug() << "GENERAL RRRRRRRRRR" << general;
    in->identity()->setIsin   (general["ISIN"].toString());
    in->identity()->setDescrip(general["Description"].toString());
    in->identity()->setLogo   ("https://eodhd.com" + general["LogoURL"].toString());
    in->identity()->setUrl    (general["WebURL"].toString());
    in->updatePrimarySymbol    (general["PrimaryTicker"].toString());
    // in->identity()->setCountry(general["CountryName"].toString());
    in->identity()->setIndustry(general["Industry"].toString());
    in->identity()->setSector(general["Sector"].toString());
    in->identity()->setHeadquart(general["Address"].toString());
    // fix country
    geo::Country c_by_primary = geo::exchange::country(in->primary_symbol(true).exchange());
    if (in->identity()->country() == c_by_primary ||
        in->identity()->country() == geo::Country::Unknown)
        return;
    if (geo::exchange::exist(in->identity()->country())){
        in->identity()->setCountry(c_by_primary);
    }

    QJsonObject mrq = pick_mrq(finance);

    // ------- VALUATION -------
    in->valuation()->setMarketCapitalization( jnum(high.value("MarketCapitalization")) );
    in->valuation()->setSharesOutstanding(    jnum(shares.value("SharesOutstanding")) );
    in->valuation()->setBookValuePsMrq(       jnum(high.value("BookValue")) );
    in->valuation()->setEnterpriseValue(      jnum(valuation.value("EnterpriseValue")) );

    // EPS TTM (prefer DilutedEpsTTM, else EarningsShare)
    {
        double eps_ttm = jnum(high.value("DilutedEpsTTM"));
        if (std::isnan(eps_ttm)) eps_ttm = jnum(high.value("EarningsShare"));
        in->valuation()->setEarningsPerShareTtm(eps_ttm);
    }

    in->valuation()->setPriceToEarningsFwd(   jnum(valuation.value("ForwardPE")) );
    in->valuation()->setPriceToEarningsTtm(   jnum(high.value("PERatio")) );             // fallback: valuation.TrailingPE
    in->valuation()->setPriceToBookMrq(       jnum(valuation.value("PriceBookMRQ")) );
    in->valuation()->setPriceToSalesTtm(      jnum(valuation.value("PriceSalesTTM")) );
    in->valuation()->setEvToSalesTtm(         jnum(valuation.value("EnterpriseValueRevenue")) );
    in->valuation()->setEvToEbitda(           jnum(valuation.value("EnterpriseValueEbitda")) );

    // ------- PROFITABILITY -------
    in->profitability()->setRoa( jnum(high.value("ReturnOnAssetsTTM")) );
    in->profitability()->setRoe( jnum(high.value("ReturnOnEquityTTM")) );

    // Gross margin: якщо немає поля — порахувати TTM
    {
        double gross_margin = std::numeric_limits<double>::quiet_NaN();
        // деякі джерела мають Highlights.GrossMarginTTM — якщо ні, рахуємо самі
        if (high.contains("GrossMarginTTM"))
            gross_margin = jnum(high.value("GrossMarginTTM"));
        else {
            const double gp  = jnum(high.value("GrossProfitTTM"));
            const double rev = jnum(high.value("RevenueTTM"));
            if (!std::isnan(gp) && !std::isnan(rev) && rev!=0.0)
                gross_margin = gp / rev;
        }
        in->profitability()->setMarginGros(gross_margin);
    }

    in->profitability()->setMarginOper(          jnum(high.value("OperatingMarginTTM")) );

    // Net income (спробувати Highlights.NetIncomeTTM; інакше лишити NaN)
    {
        double net_income = jnum(high.value("NetIncomeTTM"));
        in->profitability()->setNetIncome(net_income);
    }

    in->profitability()->setRevenueTtm(          jnum(high.value("RevenueTTM")) );
    in->profitability()->setGrossProfitTtm(      jnum(high.value("GrossProfitTTM")) );
    in->profitability()->setRevenuePerShareTtm(  jnum(high.value("RevenuePerShareTTM")) );
    in->profitability()->setProfitMargin(        jnum(high.value("ProfitMargin")) );
    in->profitability()->setOperatingMarginTtm(  jnum(high.value("OperatingMarginTTM")) );

    // EPS TTM дублюємо в profitability
    {
        double eps_ttm = jnum(high.value("DilutedEpsTTM"));
        if (std::isnan(eps_ttm)) eps_ttm = jnum(high.value("EarningsShare"));
        in->profitability()->setEpsTtm(eps_ttm);
    }

    in->profitability()->setEpsEstimateYear(     jnum(high.value("EPSEstimateCurrentYear")) );
    in->profitability()->setEpsEstimateNextYear( jnum(high.value("EPSEstimateNextYear")) );
    in->profitability()->setPegRatio(            jnum(high.value("PEGRatio")) );
    in->profitability()->setTargetPrice(         jnum(high.value("WallStreetTargetPrice")) );
    in->profitability()->setMostRecentQuarter(   jdate(high.value("MostRecentQuarter")) );
    in->profitability()->setQepsGrowthYoy(       jnum(high.value("QuarterlyEarningsGrowthYOY")) );
    in->profitability()->setQrevGrowthYoy(       jnum(high.value("QuarterlyRevenueGrowthYOY")) );
    in->profitability()->setEbitda(              jnum(high.value("EBITDA")) );

    // ------- EARNINGS -------
    in->earnings()->setRevenueTtm(           jnum(high.value("RevenueTTM")) );
    in->earnings()->setRevenuePerShareTtm(   jnum(high.value("RevenuePerShareTTM")) );
    in->earnings()->setEbitda(               jnum(high.value("EBITDA")) );
    {
        double eps_ttm = jnum(high.value("DilutedEpsTTM"));
        if (std::isnan(eps_ttm)) eps_ttm = jnum(high.value("EarningsShare"));
        in->earnings()->setDilutedEpsTtm(eps_ttm);
    }
    in->earnings()->setRevGrowthYoy(         jnum(high.value("QuarterlyRevenueGrowthYOY")) );
    in->earnings()->setEarningsGrowthYoy(    jnum(high.value("QuarterlyEarningsGrowthYOY")) );
    in->earnings()->setMostRecentQuarter(    jdate(high.value("MostRecentQuarter")) );

    // ------- SHARES -------
    in->shares()->setSharesOutstanding(      jnum(shares.value("SharesOutstanding")) );
    in->shares()->setSharesFloat(            jnum(shares.value("SharesFloat")) );
    in->shares()->setSharesShort(            jnum(shares.value("SharesShort")) );
    in->shares()->setPctInsiders(            jnum(shares.value("PercentInsiders")) );
    in->shares()->setPctInstitutions(        jnum(shares.value("PercentInstitutions")) );
    in->shares()->setShortRatio(             jnum(shares.value("ShortRatio")) );
    in->shares()->setShortPctOut(            jnum(shares.value("ShortPercentOutstanding")) );
    in->shares()->setShortPctFloat(          jnum(shares.value("ShortPercentFloat")) );

    // ------- BALANCE (MRQ) -------
    double longDebt  = jnum(mrq.value("longTermDebtTotal"));
    if (std::isnan(longDebt))  longDebt  = jnum(mrq.value("longTermDebt"));

    double shortDebt = jnum(mrq.value("shortLongTermDebtTotal"));
    if (std::isnan(shortDebt)) shortDebt = jnum(mrq.value("shortLongTermDebt"));
    if (std::isnan(shortDebt)) shortDebt = jnum(mrq.value("shortTermDebt"));

    double equity    = jnum(mrq.value("totalStockholderEquity"));
    double totalDebt = (std::isnan(longDebt)?0.0:longDebt) + (std::isnan(shortDebt)?0.0:shortDebt);

    double cashSti   = jnum(mrq.value("cashAndShortTermInvestments"));
    if (std::isnan(cashSti)) cashSti = jnum(mrq.value("cashAndEquivalents"));
    if (std::isnan(cashSti)) cashSti = jnum(mrq.value("cash"));

    double netDebt   = jnum(mrq.value("netDebt"));
    if (std::isnan(netDebt) && !std::isnan(totalDebt) && !std::isnan(cashSti))
        netDebt = totalDebt - cashSti;

    double curAssets = jnum(mrq.value("totalCurrentAssets"));
    double curLiabs  = jnum(mrq.value("totalCurrentLiabilities"));
    double nwc       = (!std::isnan(curAssets) && !std::isnan(curLiabs)) ? (curAssets - curLiabs)
                                                                         : std::numeric_limits<double>::quiet_NaN();

    in->balance()->setTotalDebt( totalDebt );
    in->balance()->setCashSti(   cashSti );
    in->balance()->setEquity(    equity );

    // ------- STABILITY -------
    in->stability()->setBeta( jnum(tech.value("Beta")) );

    double de_ratio = (!std::isnan(equity) && equity!=0.0)
                    ? (totalDebt / equity)
                    : std::numeric_limits<double>::quiet_NaN();

    in->stability()->setDebtEquity(   de_ratio );
    in->stability()->setNetDebt(      netDebt );
    in->stability()->setShortDebt(    shortDebt );
    in->stability()->setLongDebt(     longDebt );
    in->stability()->setNetWorkingCap(nwc);

    // ------- DIVIDEND -------
    {
        // yield + per_share з Highlights, fallback — SplitsDividends
        double dy = jnum(high.value("DividendYield"));
        if (std::isnan(dy)) dy = jnum(splits.value("ForwardAnnualDividendYield"));

        double dps = jnum(high.value("DividendShare"));
        if (std::isnan(dps)) dps = jnum(splits.value("ForwardAnnualDividendRate"));

        double payout = jnum(splits.value("PayoutRatio"));

        QDate exDate   = jdate(splits.value("ExDividendDate"));
        QDate prevDate = jdate(splits.value("DividendDate"));

        in->dividend()->setYield(     dy );
        in->dividend()->setPerShare( dps );
        in->dividend()->setPayRatio( payout );
        in->dividend()->setNextDate( exDate );
        in->dividend()->setPrevDate( prevDate );
    }

    auto list = in->tickers();
    for (const auto& it : list){
        if (it.us()){
            // api::FinnHub::update_info_by_tag(it.code());
            // api::AlphaVantage::update_info_by_tag(it.code());
        }
    }

    ticker->save();
}
