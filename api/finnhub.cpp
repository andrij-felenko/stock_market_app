#include "finnhub.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>

#include "data/market.h"

api::FinnHub::FinnHub(QObject* parent) : API(parent)
{
    set_api_key("d0vg7fhr01qkepd02j60d0vg7fhr01qkepd02j6g");
}

void api::FinnHub::set_api_key(const QString& key) { m_api_key = key; }

bool api::FinnHub::request(Request type, QString name, StringMap keys)
{
    QString base("https://finnhub.io/api/v1/");
    // as we work only with US marker, we nee to cut .US domen from tag
    if (name.right(3).toUpper() == ".US")
        name.chop(3);

    QUrl url;
    switch (type){
        case api::Request::MetricAll:
        case api::Request::MetricPrice:
        case api::Request::MetricMargin:
        case api::Request::MetricValuation: url = base + "stock/metric"; break;

        case api::Request::Info:     url = base + "stock/profile2"; break;
        case api::Request::Peers:    url = base + "stock/peers";    break;
        case api::Request::Quote:    url = base + "quote";          break;
        case api::Request::Candle:   url = base + "stock/candle";   break;
        case api::Request::Dividend: url = base + "stock/dividend"; break;
        case api::Request::Earnings: url = base + "calendar/earnings";         break;
        case api::Request::Reported: url = base + "stock/financials-reported"; break;
    }

    QUrlQuery query;
    query.addQueryItem("symbol", name);
    query.addQueryItem("token", m_api_key);

    switch (type){
    case api::Request::MetricAll:       query.addQueryItem("metric", "all");       break;
    case api::Request::MetricPrice:     query.addQueryItem("metric", "price");     break;
    case api::Request::MetricMargin:    query.addQueryItem("metric", "margin");    break;
    case api::Request::MetricValuation: query.addQueryItem("metric", "valuation"); break;

    // resolution REQUIRED
    // Supported resolution includes 1, 5, 15, 30, 60, D, W, M .
    // Some timeframes might not be available depending on the exchange.
    case api::Request::Candle: {
        if (!keys.contains("from") ||
            !keys.contains("to")   ||
            !keys.contains("resolution")) // can be
            return false;

        query.addQueryItem("resolution", keys["resolution"]);
        query.addQueryItem("from",       keys["from"]);
        query.addQueryItem("to",         keys["to"]);
        break;
    }
    case api::Request::Dividend: {
        if (!keys.contains("from") || !keys.contains("to"))
            return false;

        query.addQueryItem("from", keys["from"]);
        query.addQueryItem("to",   keys["to"]);
        break;
    }
    case api::Request::Info:     break;
    case api::Request::Peers:    break;
    case api::Request::Quote:    break;
    case api::Request::Earnings: break;
    case api::Request::Reported: break;
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    API::add_reply(type, m_manager.get(request), name);
    qDebug() << "request:" << url;
    return true;
}

void api::FinnHub::handler_answer(Request type, QByteArray data, QString name)
{
    qDebug() << "handler answer";
    qDebug() << data;
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    qDebug() << name << "return data" << doc;
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
        // ticker->_currency = currency::Name::to_enum(obj.value("currency").toString());
        t->identity()->set_title(obj.value("name").toString());
        t->identity()->set_logo(obj.value("logo").toString());
        t->identity()->set_country   (obj.value("country").toString());
        t->identity()->set_industry  (obj.value("finnhubIndustry").toString());
        t->identity()->set_exchange  (obj.value("exchange").toString());
        t->valuation()->set_market_cap(obj.value("marketCapitalization").toDouble() * 1'000'000);

        t->identity()->set_ipo(QDate::fromString(obj.value("ipo").toString(), "YYYY-MM-DD"));
        t->identity()->set_url(obj.value("weburl").toString());
        // ticker->count_akcij = obj.value("sharedOutstanding").toDouble() * 1'000'000;
        break;
    }
    case api::Request::Quote: {
        //
        break;
    }
    default:;
    }
}
