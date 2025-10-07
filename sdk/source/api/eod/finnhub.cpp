#include "api/eod/finnhub.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>

#include "loader.h"
#include "service/market.h"

sdk::api::FinnHub::FinnHub(QObject* parent) : Provider(QUrl("https://finnhub.io/api/v1/"), parent)
{
    shift_ms = 1000;
    // set_api_key("d0vg7fhr01qkepd02j60d0vg7fhr01qkepd02j6g");
}

sdk::api::FinnHub* sdk::api::FinnHub::instance()
{
    static FinnHub* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new FinnHub(qApp);
    }
    return _instance;
}

// void api::FinnHub::set_api_key(const QString& key) { _api_key = key; }

void sdk::api::FinnHub::updateInfoByTag(QString tag)
{
    FinnHub* data = FinnHub::instance();
    data->request(Request::Info, tag);
}

bool sdk::api::FinnHub::request(Request type, const QString& name,
                                const sdk::Symbol& tag, StringMap keys)
{
    Call* post = add(type);

    QString subname;
    if (tag.us()) subname = tag.venue();
    else          subname = tag.full();
    if (subname.isEmpty())
        return false;

    switch (type){
        case api::Request::MetricAll:
        case api::Request::MetricPrice:
        case api::Request::MetricMargin:
        case api::Request::MetricValuation: post->suburl = "stock/metric"; break;

        case api::Request::Info:     post->suburl = "stock/profile2"; break;
        case api::Request::Peers:    post->suburl = "stock/peers";    break;
        case api::Request::Quote:    post->suburl = "quote";          break;
        case api::Request::Candle:   post->suburl = "stock/candle";   break;
        case api::Request::Dividend: post->suburl = "stock/dividend"; break;
        case api::Request::Earnings: post->suburl = "calendar/earnings";         break;
        case api::Request::Reported: post->suburl = "stock/financials-reported"; break;

        case api::Request::Tag:
        default: return false;
    }

    post->addQueryItem("symbol", subname);
    post->addQueryItem("token", endpoints()->key_fh());

    switch (type){
        case api::Request::MetricAll:       post->addQueryItem("metric", "all");       break;
        case api::Request::MetricPrice:     post->addQueryItem("metric", "price");     break;
        case api::Request::MetricMargin:    post->addQueryItem("metric", "margin");    break;
        case api::Request::MetricValuation: post->addQueryItem("metric", "valuation"); break;

        // resolution REQUIRED
        // Supported resolution includes 1, 5, 15, 30, 60, D, W, M .
        // Some timeframes might not be available depending on the exchange.
        case api::Request::Candle: {
            if (!keys.contains("from") ||
                !keys.contains("to")   ||
                !keys.contains("resolution")) // can be
                return false;

            post->addQueryItem("resolution", keys["resolution"]);
            post->addQueryItem("from",       keys["from"]);
            post->addQueryItem("to",         keys["to"]);
            break;
        }
        case api::Request::Dividend: {
            if (!keys.contains("from") || !keys.contains("to"))
                return false;

            post->addQueryItem("from", keys["from"]);
            post->addQueryItem("to",   keys["to"]);
            break;
        }
        case api::Request::Info:     break;
        case api::Request::Peers:    break;
        case api::Request::Quote:    break;
        case api::Request::Earnings: break;
        case api::Request::Reported: break;
        default:;
    }

    post->name = name;
    post->prepare();
    return true;
}

void sdk::api::FinnHub::handlerAnswer(Call* reply)
{
    qDebug() << "handler answer";
    qDebug() << reply->receiveData();
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(reply->receiveData());
    qDebug() << reply->name << reply->symbol << "return data" << doc;
    // qDebug() << response;

    auto ticker = Nexus->market()->findTicker(reply->symbol);
    if (ticker.ensure() == false){
        qDebug() << Q_FUNC_INFO << reply->symbol << "FALSE";
        return;
    }

    sdk::Ticker* t = ticker.ticker;
    sdk::Instrument* in = ticker.instrument;
    QJsonObject obj = doc.object();
    if (obj.isEmpty())
        return;

    switch (reply->type()){
        case api::Request::Info: {
            // ticker->_currency = currency::Name::to_enum(obj.value("currency").toString());
            // t->symbol().set_venue(obj.value("exchange").toString());
            // in->identity()->setTitle(obj.value("name").toString());
            // in->identity()->setLogo(obj.value("logo").toString());
            // in->identity()->setCountry   (obj.value("country").toString());
            // in->identity()->setIndustry  (obj.value("finnhubIndustry").toString());
            // in->valuation()->setMarketCapitalization(obj.value("marketCapitalization").toDouble() * 1'000'000);

            // in->identity()->setIpo(QDate::fromString(obj.value("ipo").toString(), "YYYY-MM-DD"));
            // in->identity()->setUrl(obj.value("weburl").toString());
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
