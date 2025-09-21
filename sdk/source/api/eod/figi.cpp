#include "api/eod/figi.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QJsonArray>

#include "service/market.h"
#include "core/security/instrument.h"
#include "loader.h"

/**
 * Запит до API OpenFIGI.
 *
 * Для деталей дивись:
 * \link https://www.openfigi.com/api/documentation OpenFIGI Mapping API \endlink
 */

sdk::api::Figi::Figi(QObject* parent) : Provider(QUrl("https://api.openfigi.com/"), parent)
{
    //
}

sdk::api::Figi* sdk::api::Figi::instance()
{
    static Figi* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Figi(qApp);
    }
    return _instance;
}

void sdk::api::Figi::updateInfoByTag(const sdk::Symbol& tag)
{
    Figi::instance()->request(Request::Info, tag);
}

bool sdk::api::Figi::request(Request type, const QString& name, const sdk::Symbol& symbol,
                             StringMap keys)
{
    Call* post = add(type);

    // as we work only with US marker, we nee to cut .US domen from tag
    QString subname;
    if (symbol.us()) subname = symbol.venue();
    else             subname = symbol.full();

    switch (type){
        // case api::Request::Text: return false;

        // case api::Request::MetricAll:
        // case api::Request::MetricPrice:
        // case api::Request::MetricMargin:
        // case api::Request::MetricValuation: url = base + "stock/metric"; break;

        case api::Request::Info:    post->suburl = "v3/mapping"; break;
        // case api::Request::Peers:    url = base + "stock/peers";    break;
        // case api::Request::Quote:    url = base + "quote";          break;
        // case api::Request::Tag:
        // case api::Request::Candle:   url = base + "";   break;
        // case api::Request::Dividend: url = base + "stock/dividend"; break;
        // case api::Request::Earnings: url = base + "calendar/earnings";         break;
        // case api::Request::Reported: url = base + "stock/financials-reported"; break;
        default:;
    }

    post->request()->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    post->request()->setRawHeader("X-OPENFIGI-APIKEY", endpoints()->key_figi().toUtf8());
    QJsonObject to_send;

    switch (type){
        // case api::Request::MetricAll:       query.addQueryItem("metric", "all");       break;
        // case api::Request::MetricPrice:     query.addQueryItem("metric", "price");     break;
        // case api::Request::MetricMargin:    query.addQueryItem("metric", "margin");    break;
        // case api::Request::MetricValuation: query.addQueryItem("metric", "valuation"); break;
        // case api::Request::Candle: {
            // if (!keys.contains("func"))
                // return false;

            // query.addQueryItem("function", keys["func"]);
            // query.addQueryItem("symbol", subname);
            // if (keys["func"] == "TIME_SERIES_INTRADAY")
                // query.addQueryItem("interval", "1min");
            // query.addQueryItem("outputsize", "full");
            // query.addQueryItem("apikey", settings::network()->key_av());
            // break;
        // }
        // case api::Request::Dividend: {
            // query.addQueryItem("function", "DIVIDENDS");
            // query.addQueryItem("symbol", subname);
            // query.addQueryItem("apikey", settings::network()->key_av());
            // break;
        // }
        case api::Request::Info: {
            QStringList split = name.split(".");
            QString exch;
            if (split.length() == 1)
                exch = "US";
            else
                exch = split.last();

            split.removeLast();
            QString subname = split.join(".");

            to_send["idType"] = "TICKER";
            to_send["idValue"] = subname;
            to_send["exchCode"] = "GR";//exch;
            break;
        }
        // case api::Request::Peers:    break;
        // case api::Request::Quote:    break;
        // case api::Request::Earnings: break;
        // case api::Request::Reported: break;
        // case api::Request::Tag: {
        //     query.addQueryItem("function", "SYMBOL_SEARCH");
        //     query.addQueryItem("keywords", subname);
        //     query.addQueryItem("apikey", settings::network()->key_av());
        //     break;
        // }
        default:;
    }

    // QNetworkRequest request(url);
    QJsonArray array( { to_send } );
    qDebug() << "next:::" << array << to_send;
    QByteArray data = QJsonDocument(array).toJson();

    post->send_data = data;
    post->prepare();
    return true;
}

void sdk::api::Figi::handlerAnswer(Call* reply)
{
    qDebug() << "handler answer";
    qDebug() << reply->receiveData();
    QJsonDocument doc = QJsonDocument::fromJson(reply->receiveData());
    qDebug() << reply->name << reply->symbol << "return data" << doc;
    return;

    auto t = Nexus.market()->findTicker(reply->symbol);
    if (t.ensure() == false)
        return;

    sdk::Instrument* in = t->instrument();
    in->create();
    QJsonObject obj = doc.object();
    switch (reply->type()){
    case api::Request::Info: {
        in->data()->meta.setDescription(obj.value("Description").toString());
        in->data()->meta.setSector(obj.value("Sector").toString());
        in->data()->meta.setIndustry(obj.value("Industry").toString());
        in->data()->legal.setAddress(obj.value("Address").toString());
        in->data()->meta.setWebsite(obj.value("OfficialSite").toString());
        in->data()->legal.setIpo(QDate::fromString(obj.value("LatestQuarter").toString(),
                                                   "yyyy-MM-dd"));

        // TODO Figi
        // in->valuation()->setMarketCapitalization(obj.value("MarketCapitalization").toDouble());
        // in->valuation()->set_eps(obj.value("EPS").toDouble());
        // in->valuation()->set_pe_ratio(obj.value("PERatio").toDouble());
        // in->valuation()->set_pb_ratio(obj.value("PriceToBookRatio").toDouble());
        // in->valuation()->set_book_value(obj.value("BookValue").toDouble());
        // in->valuation()->set_share_count(obj.value("SharesOutstanding").toDouble());

        // in->profitability()->setRoa(obj.value("ReturnOnAssetsTTM").toDouble());
        // in->profitability()->setRoe(obj.value("ReturnOnEquityTTM").toDouble());
        // in->profitability()->setMarginGros(obj.value("GrossProfitTTM").toDouble() /
        //                                    obj.value("RevenueTTM").toDouble());
        // in->profitability()->setMarginOper(obj.value("OperatingMarginTTM").toDouble());
        // in->profitability()->setNetIncome(obj.value("ProfitMargin").toDouble());

        // in->dividend()->setYield(obj.value("DividendYield").toDouble());
        // in->dividend()->setPerShare(obj.value("DividendPerShare").toDouble());
        // in->dividend()->setNextDate(QDate::fromString(obj.value("DividendDate").toString(),
        //                                               "yyyy-MM-dd"));
        // in->dividend()->setPrevDate(QDate::fromString(obj.value("ExDividendDate").toString(),
        //                                               "yyyy-MM-dd"));

        // in->stability()->setBeta(obj.value("Beta").toDouble());
        // in->profitability()->setRevenueTtm(obj.value("RevenueTTM").toDouble());

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

            t->quotes.setData(date, open, close, high, low, vol);
        }

        QJsonObject time_minute = root.value("Time Series (1min)").toObject();
        if (! time_minute.isEmpty()){
            QJsonObject meta = root.value("Meta Data").toObject();
            QString str = meta.value("Last Refreshed").toString().first(10);
            t->quotes.setIntraday(QDate::fromString(str, "yyyy-MM-dd"));
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

            t->quotes.setData(time, open, close, high, low, vol);
        }

        t->save();
        break;
    }
    case api::Request::Tag: {
        QJsonObject root = doc.object();
        QJsonArray list = root.value("bestMatches").toArray();
        // model::SearchTag* st = Loader::instance()->search_tag();
        // st->clear();
        for (const auto& it : std::as_const(list)){
            // QJsonObject obj = it.toObject();
            // st->add(obj.value("1. symbol"  ).toString(),
            //         obj.value("2. name"    ).toString(),
            //         obj.value("3. type"    ).toString());
        }
        break;
    }
    case api::Request::Dividend: {
        QJsonObject root = doc.object();
        QJsonArray array = root.value("data").toArray();
        for (const auto& it : std::as_const(array)){
            QJsonObject obj = it.toObject();
            // TODO Dividend
            // in->dividend()->setHistory(QDate::fromString(obj.value("ex_dividend_date").toString(),
                                                          // "yyy-MM-dd"),
                                        // obj.value("amount").toDouble());
        }
        break;
    }
    default:;
    }
    in->release();
}
