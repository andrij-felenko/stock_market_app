#include "alphavantage.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>

#include "data/market.h"

api::AlphaVantage::AlphaVantage(QObject* parent) : API(parent)
{
    set_api_key("539EYFYAYCXFMWIL");
}

api::AlphaVantage* api::AlphaVantage::instance()
{
    static AlphaVantage* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new AlphaVantage(qApp);
    }
    return _instance;
}

void api::AlphaVantage::set_api_key(const QString& key) { m_api_key = key; }

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

bool api::AlphaVantage::_request(Request type, QString name, StringMap keys)
{
    QString base("https://www.alphavantage.co/query");
    // as we work only with US marker, we nee to cut .US domen from tag

    QUrl url;
    switch (type){
        case api::Request::Tag:
        case api::Request::Text: return false;

        case api::Request::MetricAll:
        case api::Request::MetricPrice:
        case api::Request::MetricMargin:
        case api::Request::MetricValuation: url = base + "stock/metric"; break;

        case api::Request::Info:     url = base + "stock/profile2"; break;
        case api::Request::Peers:    url = base + "stock/peers";    break;
        case api::Request::Quote:    url = base + "quote";          break;
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

        // resolution REQUIRED
        // Supported resolution includes 1, 5, 15, 30, 60, D, W, M .
        // Some timeframes might not be available depending on the exchange.
        case api::Request::Candle: {
            if (!keys.contains("func"))
                return false;

            query.addQueryItem("function", keys["func"]);
            query.addQueryItem("symbol", name);
            query.addQueryItem("outputsize", "full");
            query.addQueryItem("apikey", m_api_key);
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
        default:;
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    API::_add_reply(type, m_manager.get(request), name);
    qDebug() << "request:" << url;
    return true;
}

void api::AlphaVantage::_handler_answer(Request type, QByteArray data, QString name, bool stream)
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
        t->save();
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
