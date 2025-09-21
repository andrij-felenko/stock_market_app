#include "api/eod/marketstack.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QJsonArray>

#include "service/market.h"

sdk::api::MarketStack::MarketStack(QObject* parent)
    : Provider(QUrl("https://api.marketstack.com/"), parent)
{
    // set_api_key("d0vg7fhr01qkepd02j60d0vg7fhr01qkepd02j6g");
}

sdk::api::MarketStack* sdk::api::MarketStack::instance()
{
    static MarketStack* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new MarketStack(qApp);
    }
    return _instance;
}

// void api::FinnHub::set_api_key(const QString& key) { _api_key = key; }

void sdk::api::MarketStack::updateInfoByTag(const sdk::Symbol& tag)
{
    MarketStack::instance()->request(Request::Info, tag);
}

// https://api.marketstack.com/v2/eod?access_key=c68c8ac43610203b7b46616e0bb8124a&symbols=VK.PA
void sdk::api::MarketStack::updateQuotesByTag(sdk::SymbolList tags)
{
    QString name;
    if (tags->empty())
        return;

    if (tags.size() > 1){
        QStringList list;
        for (auto& it : tags){
            if (it.us())
                list.push_back(it.code());
            else
                list.push_back(it.full());
        }
        name = list.join(",").toUpper();
    }
    else
        name = tags->at(0);

    MarketStack* data = MarketStack::instance();
    data->request(Request::Quote, name);
}

bool sdk::api::MarketStack::request(Request type, const QString& name, const sdk::Symbol& tag,
                                    StringMap keys)
{
    Call* post = add(type);

    QString subname;
    if (tag.us()) subname = tag.venue();
    else          subname = tag.full();

    switch (type){
        // case api::Request::MetricAll:
        // case api::Request::MetricPrice:
        // case api::Request::MetricMargin:
        // case api::Request::MetricValuation: url = base + "stock/metric"; break;

        case api::Request::Info:     post->suburl = "v2/tickers/" + subname; break;
        // case api::Request::Peers:    url = base + "stock/peers";    break;
        case api::Request::Quote:    post->suburl = "v2/eod";         break;
        // case api::Request::Candle:   url = base + "stock/candle";   break;
        // case api::Request::Dividend: url = base + "stock/dividend"; break;
        // case api::Request::Earnings: url = base + "calendar/earnings";         break;
        // case api::Request::Reported: url = base + "stock/financials-reported"; break;

        // case api::Request::Tag:
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
        case api::Request::Info: {
            post->addQueryItem("access_key", endpoints()->key_ms());
            break;
        }
        case api::Request::Peers:    break;
        case api::Request::Quote: {
            // ?access_key=c68c8ac43610203b7b46616e0bb8124a&symbols=VK.PA
            post->addQueryItem("access_key", endpoints()->key_ms());
            post->addQueryItem("symbols", name);
            post->addQueryItem("limit", "999");
            break;
        }
        case api::Request::Earnings: break;
        case api::Request::Reported: break;
        default:;
    }

    post->name = name;
    post->prepare();
    return true;
}

void sdk::api::MarketStack::handlerAnswer(Call* reply)
{
    qDebug() << "handler answer";
    qDebug() << reply->receiveData();
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(reply->receiveData());
    qDebug() << reply->name << reply->symbol << "return data" << doc;
    // qDebug() << response;

    QJsonObject root = doc.object();
    if (root.isEmpty())
        return;

    // switch (type){
    //     case api::Request::Quote: {
    //         QJsonArray array = root.value("data").toArray();
    //         for (const auto& it : std::as_const(array)){
    //             QJsonObject obj = it.toObject();
    //             QString tag = obj.value("symbol").toString();

    //             auto finded = Nexus.market()->find(tag);
    //             if (!finded.has_value()){
    //                 // Nexus.market()->add(tag);
    //                 finded = Nexus.market()->find(tag);
    //                 if (!finded.has_value())
    //                     continue;
    //             }
    //             data::Ticker* t = finded.value();
    //             t->quotes()->setData(QDate::fromString(obj.value("date").toString().left(10),
    //                                                     "yyyy-MM-dd"),
    //                                   obj.value("open").toDouble(),
    //                                   obj.value("close").toDouble(),
    //                                   obj.value("high").toDouble(),
    //                                   obj.value("low").toDouble(),
    //                                   obj.value("volume").toInt());
    //             t->save();
    //         }
    //         break;
    //     }
    //     case api::Request::Info: {
    //         auto finded = Nexus.market()->find(name);
    //         if (!finded.has_value()){
    //             // Nexus.market()->add(name);
    //             finded = Nexus.market()->find(name);
    //             if (!finded.has_value())
    //                 break;
    //         }
    //         data::Ticker* t = finded.value();
    //         data::Instrument* in = t->instrument();
    //         t->symbol().set_code(root.value("symbol").toString());
    //         t->symbol().set_venue(root.value("stock_exchange").toObject().value("acronym").toString());
    //         in->identity()->setTitle(root.value("name").toString());
    //         in->identity()->setSector(root.value("sector").toString());
    //         in->identity()->setIndustry(root.value("industry").toString());
    //         in->identity()->setIsin(root.value("isin").toString());
    //         in->identity()->setCountry(root.value("stock_exchange").toObject().value("country_code").toString());

    //         t->save();
    //         break;
    //     }
    //     default:;
    // }
}
