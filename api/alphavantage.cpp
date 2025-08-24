#include "alphavantage.h"
#include "loader.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QJsonArray>

#include "data/market.h"
#include "model/search_tag.h"
#include "data/instrument.h"

api::AlphaVantage::AlphaVantage(QObject* parent) : API(parent)
{
    shift_ms = 15'100;
}

api::AlphaVantage* api::AlphaVantage::instance()
{
    static AlphaVantage* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new AlphaVantage(qApp);
    }
    return _instance;
}

void api::AlphaVantage::update_info_by_tag(QString tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    data->_send(Request::Info, tag);
}

void api::AlphaVantage::daily_candle_by_tag(QString tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    api::StringMap params;
    params["func"] = "TIME_SERIES_DAILY";
    data->_send(Request::Candle, tag, params);
}

void api::AlphaVantage::today_candle_by_tag(QString tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    api::StringMap params;
    params["func"] = "TIME_SERIES_INTRADAY";
    data->_send(Request::Candle, tag, params);
}

void api::AlphaVantage::find_symbol(QString str) { find_tag(str); }
void api::AlphaVantage::find_tag   (QString str)
{
    AlphaVantage* data = AlphaVantage::instance();
    data->_send(Request::Tag, str);
}

bool api::AlphaVantage::_request(Request type, QString name, StringMap keys)
{
    QString base("https://www.alphavantage.co/query/");
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
        case Request::Exchange:
            break;
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

    auto finded = Nexus.market()->find(name);
    if (!finded.has_value()){
        finded = Nexus.market()->find(name);
        if (!finded.has_value())
            return;
    }

    data::Ticker* t = finded.value();
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    switch (type){
        case api::Request::Info:     _handle_info(obj, name, t); break;
        case api::Request::Candle:   _handle_candle(obj, t);     break;
        case api::Request::Tag:      _handle_tag(obj);           break;
        case api::Request::Dividend: _handle_dividend(obj, t);   break;
        default:;
    }
}

void api::AlphaVantage::_handle_info(const QJsonObject& root, QString name, data::Ticker* t)
{
    data::Instrument* in = t->instrument();
    in->identity()->set_title(root.value("Name").toString());
    in->identity()->set_descrip(root.value("Description").toString());

    in->identity()->set_country(root.value("Country").toString());
    in->identity()->set_sector(root.value("Sector").toString());
    in->identity()->set_industry(root.value("Industry").toString());
    in->identity()->set_headquart(root.value("Address").toString());
    in->identity()->set_url(root.value("OfficialSite").toString());
    in->identity()->set_ipo(QDate::fromString(root.value("LatestQuarter").toString(),
                                              "yyyy-MM-dd"));

    // TODO Alpha vintage update
    // in->valuation()->setMarketCapitalization(root.value("MarketCapitalization").toDouble());
    // in->valuation()->set_eps(root.value("EPS").toDouble());
    // in->valuation()->set_pe_ratio(root.value("PERatio").toDouble());
    // in->valuation()->set_pb_ratio(root.value("PriceToBookRatio").toDouble());
    // in->valuation()->set_book_value(root.value("BookValue").toDouble());
    // in->shares()->setSharesOutstanding(root.value("SharesOutstanding").toDouble());

    in->profitability()->setRoa(root.value("ReturnOnAssetsTTM").toDouble());
    in->profitability()->setRoe(root.value("ReturnOnEquityTTM").toDouble());
    in->profitability()->setMarginOper(root.value("OperatingMarginTTM").toDouble());
    in->profitability()->setNetIncome(root.value("ProfitMargin").toDouble());
    in->profitability()->setMarginGros(root.value("GrossProfitTTM").toDouble() /
                                         root.value("RevenueTTM").toDouble());


    in->dividend()->set_yield(root.value("DividendYield").toDouble());
    in->dividend()->set_per_share(root.value("DividendPerShare").toDouble());
    in->dividend()->set_next_date(QDate::fromString(root.value("DividendDate").toString(),
                                                    "yyyy-MM-dd"));
    in->dividend()->set_prev_date(QDate::fromString(root.value("ExDividendDate").toString(),
                                                    "yyyy-MM-dd"));

    in->stability()->setBeta(root.value("Beta").toDouble());
    in->profitability()->setRevenueTtm(root.value("RevenueTTM").toDouble());

    t->save();
}

void api::AlphaVantage::_handle_candle(const QJsonObject& root, data::Ticker* t)
{
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
}

void api::AlphaVantage::_handle_tag(const QJsonObject& root)
{
    QJsonArray list = root.value("bestMatches").toArray();
    Nexus.search_tag()->clear();
    for (const auto& it : std::as_const(list)){
        QJsonObject obj = it.toObject();
        Nexus.search_tag()->add(obj.value("1. symbol"  ).toString(),
                                obj.value("2. name"    ).toString(),
                                obj.value("3. type"    ).toString());
    }
}

void api::AlphaVantage::_handle_dividend(const QJsonObject& root, data::Ticker* t)
{
    QJsonArray array = root.value("data").toArray();
    for (const auto& it : std::as_const(array)){
        QJsonObject obj = it.toObject();
        t->instrument()->dividend()->set_history(QDate::fromString(obj.value("ex_dividend_date")
                                                                   .toString(), "yyyy-MM-dd"),
                                                 obj.value("amount").toDouble());
    }
}
