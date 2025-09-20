#include "api/alphavantage.h"
#include "loader.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QJsonArray>

#include "services/market.h"
#include "instrument/instrument.h"

api::AlphaVantage::AlphaVantage(QObject* parent)
    : API(QUrl("https://www.alphavantage.co/query/"), parent)
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

void api::AlphaVantage::updateInfoByTag(const sdk::Symbol& tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    data->request(Request::Info, tag);
}

void api::AlphaVantage::dailyCandleByTag(const sdk::Symbol& tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    api::StringMap params;
    params["func"] = "TIME_SERIES_DAILY";
    data->request(Request::Candle, tag, params);
}

void api::AlphaVantage::todayCandleByTag(const sdk::Symbol& tag)
{
    AlphaVantage* data = AlphaVantage::instance();
    api::StringMap params;
    params["func"] = "TIME_SERIES_INTRADAY";
    data->request(Request::Candle, tag, params);
}

void api::AlphaVantage::findSymbol(QString str) { findTag(str); }
void api::AlphaVantage::findTag   (QString str)
{
    AlphaVantage* data = AlphaVantage::instance();
    data->request(Request::Tag, str);
}

bool api::AlphaVantage::request(Request type, const QString& name,
                                const sdk::Symbol& tag, StringMap keys)
{
    Reply* post = add(type);

    QString subname;
    if (tag.us()) subname = tag.venue();
    else          subname = tag.full();

    switch (type){
        case Request::Logo:
        case Request::Text: return false;
        case Request::MetricAll:
        case Request::MetricPrice:
        case Request::MetricMargin:
        case Request::MetricValuation: post->suburl = "stock/metric"; break;
        case Request::Info:     post->suburl = "stock/profile2"; break;
        case Request::Peers:    post->suburl = "stock/peers";    break;
        case Request::Quote:    post->suburl = "quote";          break;
        case Request::Tag:
        case Request::Dividend: post->suburl = "stock/dividend"; break;
        case Request::Earnings: post->suburl = "calendar/earnings";         break;
        case Request::Reported: post->suburl = "stock/financials-reported"; break;
        case Request::Exchange:
        case Request::Candle: break;
    }

    switch (type){
        case api::Request::MetricAll:       post->addQueryItem("metric", "all");       break;
        case api::Request::MetricPrice:     post->addQueryItem("metric", "price");     break;
        case api::Request::MetricMargin:    post->addQueryItem("metric", "margin");    break;
        case api::Request::MetricValuation: post->addQueryItem("metric", "valuation"); break;
        case api::Request::Candle: {
            if (!keys.contains("func"))
                return false;

            post->addQueryItem("function", keys["func"]);
            post->addQueryItem("symbol", subname);
            if (keys["func"] == "TIME_SERIES_INTRADAY")
                post->addQueryItem("interval", "1min");
            post->addQueryItem("outputsize", "full");
            post->addQueryItem("apikey", settings::network()->key_av());
            break;
        }
        case api::Request::Dividend: {
            post->addQueryItem("function", "DIVIDENDS");
            post->addQueryItem("symbol", subname);
            post->addQueryItem("apikey", settings::network()->key_av());
            break;
        }
        case api::Request::Info: {
            post->addQueryItem("function", "OVERVIEW");
            post->addQueryItem("symbol", subname);
            post->addQueryItem("apikey", settings::network()->key_av());
            break;
        }
        case api::Request::Peers:    break;
        case api::Request::Quote:    break;
        case api::Request::Earnings: break;
        case api::Request::Reported: break;
        case api::Request::Tag: {
            post->addQueryItem("function", "SYMBOL_SEARCH");
            post->addQueryItem("keywords", subname);
            post->addQueryItem("apikey", settings::network()->key_av());
            post->name = name;
            break;
        }
        default:;
    }

    post->prepare();
    return true;
}

void api::AlphaVantage::handlerAnswer(Reply* reply)
{
    qDebug() << "handler answer";

    switch (reply->type()){
        case api::Request::Info:     handleInfo    (reply); break;
        case api::Request::Candle:   handleCandle  (reply); break;
        case api::Request::Tag:      handleTag     (reply); break;
        case api::Request::Dividend: handleDividend(reply); break;
        default:;
    }
}

void api::AlphaVantage::handleInfo(Reply* reply)
{
    auto ticker = Nexus.market()->findTicker(reply->symbol);
    if (ticker.ensure() == false){
        qDebug() << Q_FUNC_INFO << reply->symbol << "FALSE";
        return;
    }
    QJsonObject root = QJsonDocument::fromJson(reply->receiveData()).object();
    // in->identity()->setTitle(root.value("Name").toString());
    // in->identity()->setDescrip(root.value("Description").toString());

    // in->identity()->setCountry(root.value("Country").toString());
    // in->identity()->setSector(root.value("Sector").toString());
    // in->identity()->setIndustry(root.value("Industry").toString());
    // in->identity()->setHeadquart(root.value("Address").toString());
    // in->identity()->setUrl(root.value("OfficialSite").toString());
    // in->identity()->setIpo(QDate::fromString(root.value("LatestQuarter").toString(),
    //                                           "yyyy-MM-dd"));

    // TODO Alpha vintage update
    // in->valuation()->setMarketCapitalization(root.value("MarketCapitalization").toDouble());
    // in->valuation()->set_eps(root.value("EPS").toDouble());
    // in->valuation()->set_pe_ratio(root.value("PERatio").toDouble());
    // in->valuation()->set_pb_ratio(root.value("PriceToBookRatio").toDouble());
    // in->valuation()->set_book_value(root.value("BookValue").toDouble());
    // in->shares()->setSharesOutstanding(root.value("SharesOutstanding").toDouble());

    // in->profitability()->setRoa(root.value("ReturnOnAssetsTTM").toDouble());
    // in->profitability()->setRoe(root.value("ReturnOnEquityTTM").toDouble());
    // in->profitability()->setMarginOper(root.value("OperatingMarginTTM").toDouble());
    // in->profitability()->setNetIncome(root.value("ProfitMargin").toDouble());
    // in->profitability()->setMarginGros(root.value("GrossProfitTTM").toDouble() /
    //                                      root.value("RevenueTTM").toDouble());


    // in->dividend()->setYield(root.value("DividendYield").toDouble());
    // in->dividend()->setPerShare(root.value("DividendPerShare").toDouble());
    // in->dividend()->setNextDate(QDate::fromString(root.value("DividendDate").toString(),
    //                                                 "yyyy-MM-dd"));
    // in->dividend()->setPrevDate(QDate::fromString(root.value("ExDividendDate").toString(),
    //                                                 "yyyy-MM-dd"));

    // in->stability()->setBeta(root.value("Beta").toDouble());
    // in->profitability()->setRevenueTtm(root.value("RevenueTTM").toDouble());

    ticker->save();
    ticker->instrument()->release();
}

void api::AlphaVantage::handleCandle(Reply* reply)
{
    auto ticker = Nexus.market()->findTicker(reply->symbol);
    if (ticker.ensure() == false){
        qDebug() << Q_FUNC_INFO << reply->symbol << "FALSE";
        return;
    }
    QJsonObject root = QJsonDocument::fromJson(reply->receiveData()).object();
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

        ticker->quotes.setData(date, open, close, high, low, vol);
    }

    QJsonObject time_minute = root.value("Time Series (1min)").toObject();
    if (! time_minute.isEmpty()){
        QJsonObject meta = root.value("Meta Data").toObject();
        QString str = meta.value("Last Refreshed").toString().first(10);
        ticker->quotes.setIntraday(QDate::fromString(str, "yyyy-MM-dd"));
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

        ticker->quotes.setData(time, open, close, high, low, vol);
    }

    ticker->save();
    ticker->instrument()->release();
}

void api::AlphaVantage::handleTag(Reply* reply)
{
    QJsonObject root = QJsonDocument::fromJson(reply->receiveData()).object();
    QJsonArray list = root.value("bestMatches").toArray();
    // Nexus.search_tag()->clear();
    // for (const auto& it : std::as_const(list)){
        // QJsonObject obj = it.toObject();
        // Nexus.search_tag()->add(obj.value("1. symbol").toString(),
        //                         obj.value("2. name"  ).toString(),
        //                         obj.value("3. type"  ).toString());
    // }
}

void api::AlphaVantage::handleDividend(Reply* reply)
{
    auto ticker = Nexus.market()->findTicker(reply->symbol);
    if (ticker.ensure() == false){
        qDebug() << Q_FUNC_INFO << reply->symbol << "FALSE";
        return;
    }
    QJsonObject root = QJsonDocument::fromJson(reply->receiveData()).object();
    QJsonArray array = root.value("data").toArray();
    for (const auto& it : std::as_const(array)){
        QJsonObject obj = it.toObject();
        ticker->dividend.setPerShare(obj.value("amount").toDouble());
        ticker->dividend.setDate(QDate::fromString(obj.value("ex_dividend_date").toString(),
                                                   "yyyy-MM-dd"));
    }
    ticker->instrument()->release();
}
