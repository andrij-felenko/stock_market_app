#include "finnhub.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>

#include "loader.h"
#include "data/market.h"
#include "data/instrument.h"

api::FinnHub::FinnHub(QObject* parent) : API(parent)
{
    // set_api_key("d0vg7fhr01qkepd02j60d0vg7fhr01qkepd02j6g");
}

api::FinnHub* api::FinnHub::instance()
{
    static FinnHub* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new FinnHub(qApp);
    }
    return _instance;
}

// void api::FinnHub::set_api_key(const QString& key) { _api_key = key; }

void api::FinnHub::update_info_by_tag(QString tag)
{
    FinnHub* data = FinnHub::instance();
    data->_request(Request::Info, tag);
}

bool api::FinnHub::_request(Request type, QString name, StringMap keys)
{
    QString base("https://finnhub.io/api/v1/");

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

        case api::Request::Tag:
        default: return false;
    }

    // as we work only with US marker, we nee to cut .US domen from tag
    QString subname = name;
    if (subname.right(3).toUpper() == ".US")
        subname.chop(3);

    if (subname.isEmpty())
        return false;

    QUrlQuery query;
    query.addQueryItem("symbol", subname);
    query.addQueryItem("token", settings::network()->key_fh());

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
        default:;
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    API::_add_reply(type, _netmanager.get(request), name);
    qDebug() << "request:" << url;
    return true;
}

void api::FinnHub::_handler_answer(Request type, QByteArray data, QString name, bool stream)
{
    qDebug() << "handler answer";
    qDebug() << data;
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    qDebug() << name << "return data" << doc;
    // qDebug() << response;

    auto finded = Nexus.market()->find(name);
    if (!finded.has_value()){
        // Nexus.market()->add(name);
        finded = Nexus.market()->find(name);
        if (!finded.has_value())
            return;
    }

    data::Ticker* t = finded.value();
    data::Instrument* in = t->instrument();
    QJsonObject obj = doc.object();
    if (obj.isEmpty())
        return;

    switch (type){
        case api::Request::Info: {
            // ticker->_currency = currency::Name::to_enum(obj.value("currency").toString());
            t->symbol().set_exchange(obj.value("exchange").toString());
            in->identity()->set_title(obj.value("name").toString());
            in->identity()->set_logo(obj.value("logo").toString());
            in->identity()->set_country   (obj.value("country").toString());
            in->identity()->set_industry  (obj.value("finnhubIndustry").toString());
            in->valuation()->set_market_cap(obj.value("marketCapitalization").toDouble() * 1'000'000);

            in->identity()->set_ipo(QDate::fromString(obj.value("ipo").toString(), "YYYY-MM-DD"));
            in->identity()->set_url(obj.value("weburl").toString());
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
