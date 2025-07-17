#include "alphavantage.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QJsonArray>

#include "data/market.h"
#include "model/search_tag.h"

// https://www.alphavantage.co/documentation/

api::AlphaVantage::AlphaVantage(QObject* parent) : API(parent)
{
    // set_api_key("539EYFYAYCXFMWIL");
    // set_api_key("T6H9IGZ3PJA7PSYV");
}

api::AlphaVantage* api::AlphaVantage::instance()
{
    static AlphaVantage* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new AlphaVantage(qApp);
    }
    return _instance;
}

// void api::AlphaVantage::set_api_key(const QString& key) { _api_key = key; }

void api::AlphaVantage::update_info_by_tag(QString tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    data->_request(Request::Info, tag);
}

void api::AlphaVantage::daily_candle_by_tag(QString tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    api::StringMap params;
    params["func"] = "TIME_SERIES_DAILY";
    data->_request(Request::Candle, tag, params);
}

void api::AlphaVantage::today_candle_by_tag(QString tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    api::StringMap params;
    params["func"] = "TIME_SERIES_INTRADAY";
    data->_request(Request::Candle, tag, params);
}

void api::AlphaVantage::find_symbol(QString str) { find_tag(str); }
void api::AlphaVantage::find_tag   (QString str)
{
    AlphaVantage* data = AlphaVantage::instance();
    data->_request(Request::Tag, str);
}

bool api::AlphaVantage::_request(Request type, QString name, StringMap keys)
{
    QString base("https://www.alphavantage.co/query");
    // as we work only with US marker, we nee to cut .US domen from tag
    QString subname = name;
    if (subname.right(3).toUpper() == ".US")
        subname.chop(3);

    QUrl url;
    switch (type){
        case api::Request::Text: return false;

        case api::Request::MetricAll:
        case api::Request::MetricPrice:
        case api::Request::MetricMargin:
        case api::Request::MetricValuation: url = base + "stock/metric"; break;

        case api::Request::Info:     url = base + "stock/profile2"; break;
        case api::Request::Peers:    url = base + "stock/peers";    break;
        case api::Request::Quote:    url = base + "quote";          break;
        case api::Request::Tag:
        case api::Request::Candle:   url = base + "";   break;
        case api::Request::Dividend: url = base + "stock/dividend"; break;
        case api::Request::Earnings: url = base + "calendar/earnings";         break;
        case api::Request::Reported: url = base + "stock/financials-reported"; break;
    }

    QUrlQuery query;

    switch (type){
        case api::Request::MetricAll:       query.addQueryItem("metric", "all");       break;
        case api::Request::MetricPrice:     query.addQueryItem("metric", "price");     break;
        case api::Request::MetricMargin:    query.addQueryItem("metric", "margin");    break;
        case api::Request::MetricValuation: query.addQueryItem("metric", "valuation"); break;
        case api::Request::Candle: {
            if (!keys.contains("func"))
                return false;

            query.addQueryItem("function", keys["func"]);
            query.addQueryItem("symbol", subname);
            if (keys["func"] == "TIME_SERIES_INTRADAY")
                query.addQueryItem("interval", "1min");
            query.addQueryItem("outputsize", "full");
            query.addQueryItem("apikey", settings::network()->key_av());
            break;
        }
        case api::Request::Dividend: {
            query.addQueryItem("function", "DIVIDENDS");
            query.addQueryItem("symbol", subname);
            query.addQueryItem("apikey", settings::network()->key_av());
            break;
        }
        case api::Request::Info: {
            query.addQueryItem("function", "OVERVIEW");
            query.addQueryItem("symbol", subname);
            query.addQueryItem("apikey", settings::network()->key_av());
            break;
        }
        case api::Request::Peers:    break;
        case api::Request::Quote:    break;
        case api::Request::Earnings: break;
        case api::Request::Reported: break;
        case api::Request::Tag: {
            query.addQueryItem("function", "SYMBOL_SEARCH");
            query.addQueryItem("keywords", subname);
            query.addQueryItem("apikey", settings::network()->key_av());
            break;
        }
        default:;
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    API::_add_reply(type, _netmanager.get(request), name);
    qDebug() << "request:" << url;
    return true;
}

void api::AlphaVantage::_handler_answer(Request type, QByteArray data, QString name, bool stream)
{
    qDebug() << "handler answer";
    // qDebug() << data;
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // qDebug() << name << "return data" << doc;
    // qDebug() << response;

    auto finded = data::Market::find(name);
    if (!finded.has_value()){
        data::Market::add(name);
        finded = data::Market::find(name);
        if (!finded.has_value())
            return;
    }

    data::Ticker* t = finded.value();
    QJsonObject obj = doc.object();
    switch (type){
    case api::Request::Info: {
        t->identity()->set_title(obj.value("Name").toString());
        t->identity()->set_descrip(obj.value("Description").toString());
        t->identity()->set_exchange(obj.value("Exchange").toString());
        t->identity()->set_country(obj.value("Country").toString());
        t->identity()->set_sector(obj.value("Sector").toString());
        t->identity()->set_industry(obj.value("Industry").toString());
        t->identity()->set_headquart(obj.value("Address").toString());
        t->identity()->set_url(obj.value("OfficialSite").toString());
        t->identity()->set_ipo(QDate::fromString(obj.value("LatestQuarter").toString(), "yyyy-MM-dd"));
        t->identity()->set_currency(currency::Name::to_enum(obj.value("Currency").toString()));

        t->valuation()->set_market_cap(obj.value("MarketCapitalization").toDouble());
        t->valuation()->set_eps(obj.value("EPS").toDouble());
        t->valuation()->set_pe_ratio(obj.value("PERatio").toDouble());
        t->valuation()->set_pb_ratio(obj.value("PriceToBookRatio").toDouble());
        t->valuation()->set_book_value(obj.value("BookValue").toDouble());
        t->valuation()->set_share_count(obj.value("SharesOutstanding").toDouble());

        t->profitability()->set_roa(obj.value("ReturnOnAssetsTTM").toDouble());
        t->profitability()->set_roe(obj.value("ReturnOnEquityTTM").toDouble());
        t->profitability()->set_margin_gros(obj.value("GrossProfitTTM").toDouble() / obj.value("RevenueTTM").toDouble());
        t->profitability()->set_margin_oper(obj.value("OperatingMarginTTM").toDouble());
        t->profitability()->set_netincome(obj.value("ProfitMargin").toDouble());

        t->dividend()->set_yield(obj.value("DividendYield").toDouble());
        t->dividend()->set_per_share(obj.value("DividendPerShare").toDouble());
        t->dividend()->set_next_date(QDate::fromString(obj.value("DividendDate").toString(), "yyyy-MM-dd"));
        t->dividend()->set_prev_date(QDate::fromString(obj.value("ExDividendDate").toString(), "yyyy-MM-dd"));

        t->stability()->set_beta(obj.value("Beta").toDouble());
        t->stability()->set_revenue(obj.value("RevenueTTM").toDouble());

        t->save();
        break;
    }
    case api::Request::Candle: {
        QJsonObject root = doc.object();

        QJsonObject time_daily = root.value("Time Series (Daily)").toObject();
        for (auto it = time_daily.begin(); it != time_daily.end(); ++it) {
            QString dateStr = it.key();
            QJsonObject candle = it.value().toObject();

            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            float open  = candle.value("1. open"  ).toString().toFloat();
            float high  = candle.value("2. high"  ).toString().toFloat();
            float low   = candle.value("3. low"   ).toString().toFloat();
            float close = candle.value("4. close" ).toString().toFloat();
            quint64 vol = candle.value("5. volume").toString().toULongLong();

            t->quotes()->set_data(date, open, close, high, low, vol);
        }

        QJsonObject time_minute = root.value("Time Series (1min)").toObject();
        if (! time_minute.isEmpty()){
            QJsonObject meta = root.value("Meta Data").toObject();
            QString str = meta.value("Last Refreshed").toString().first(10);
            t->quotes()->set_intraday(QDate::fromString(str, "yyyy-MM-dd"));
        }
        for (auto it = time_minute.begin(); it != time_minute.end(); ++it) {
            QString dateStr = it.key();
            QJsonObject candle = it.value().toObject();

            QTime time = QTime::fromString(dateStr.last(8), "hh:mm:ss");
            float open  = candle.value("1. open"  ).toString().toFloat();
            float high  = candle.value("2. high"  ).toString().toFloat();
            float low   = candle.value("3. low"   ).toString().toFloat();
            float close = candle.value("4. close" ).toString().toFloat();
            quint64 vol = candle.value("5. volume").toString().toULongLong();

            t->quotes()->set_data(time, open, close, high, low, vol);
        }

        t->quotes()->recalculate();
        t->save();
        break;
    }
    case api::Request::Tag: {
        QJsonObject root = doc.object();
        QJsonArray list = root.value("bestMatches").toArray();
        model::SearchTag* st = model::SearchTag::instance();
        st->clear();
        for (const auto& it : std::as_const(list)){
            QJsonObject obj = it.toObject();
            st->add(obj.value("1. symbol"  ).toString(),
                    obj.value("2. name"    ).toString(),
                    obj.value("3. type"    ).toString(),
                    obj.value("4. region"  ).toString(),
                    obj.value("8. currency").toString());
        }
        break;
    }
    case api::Request::Dividend: {
        QJsonObject root = doc.object();
        QJsonArray array = root.value("data").toArray();
        for (const auto& it : std::as_const(array)){
            QJsonObject obj = it.toObject();
            t->dividend()->set_history(QDate::fromString(obj.value("ex_dividend_date").toString(),
                                                         "yyy-MM-dd"),
                                       obj.value("amount").toDouble());
        }
        break;
    }
    default:;
    }
}
