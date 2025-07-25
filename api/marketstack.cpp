#include "marketstack.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtGui/QGuiApplication>
#include <QJsonArray>

#include "data/market.h"
#include "data/instrument.h"

api::MarketStack::MarketStack(QObject* parent) : API(parent)
{
    // set_api_key("d0vg7fhr01qkepd02j60d0vg7fhr01qkepd02j6g");
}

api::MarketStack* api::MarketStack::instance()
{
    static MarketStack* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new MarketStack(qApp);
    }
    return _instance;
}

// void api::FinnHub::set_api_key(const QString& key) { _api_key = key; }

void api::MarketStack::update_info_by_tag(QString tag)
{
    MarketStack* data = MarketStack::instance();
    data->_request(Request::Info, tag);
}

// https://api.marketstack.com/v2/eod?access_key=c68c8ac43610203b7b46616e0bb8124a&symbols=VK.PA
void api::MarketStack::update_quotes_by_tag(QStringList tags)
{
    QString name;
    if (tags.isEmpty())
        return;

    if (tags.length() > 1)
        name = tags.join(",");
    else
        name = tags[0];

    for (auto& it : tags)
        if (it.toUpper().right(3) == ".US")
            it.chop(3);

    MarketStack* data = MarketStack::instance();
    data->_request(Request::Quote, name);
}

bool api::MarketStack::_request(Request type, QString name, StringMap keys)
{
    QString base("https://api.marketstack.com/");

    // as we work only with US marker, we nee to cut .US domen from tag
    QString subname = name;
    if (subname.right(3).toUpper() == ".US")
        subname.chop(3);

    QUrl url;
    switch (type){
        // case api::Request::MetricAll:
        // case api::Request::MetricPrice:
        // case api::Request::MetricMargin:
        // case api::Request::MetricValuation: url = base + "stock/metric"; break;

        case api::Request::Info:     url = base + "v2/tickers/" + subname; break;
        // case api::Request::Peers:    url = base + "stock/peers";    break;
        case api::Request::Quote:    url = base + "v2/eod";         break;
        // case api::Request::Candle:   url = base + "stock/candle";   break;
        // case api::Request::Dividend: url = base + "stock/dividend"; break;
        // case api::Request::Earnings: url = base + "calendar/earnings";         break;
        // case api::Request::Reported: url = base + "stock/financials-reported"; break;

        // case api::Request::Tag:
        default: return false;
    }

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
        case api::Request::Info: {
            query.addQueryItem("access_key", settings::network()->key_ms());
            break;
        }
        case api::Request::Peers:    break;
        case api::Request::Quote: {
            // ?access_key=c68c8ac43610203b7b46616e0bb8124a&symbols=VK.PA
            query.addQueryItem("access_key", settings::network()->key_ms());
            query.addQueryItem("symbols", name);
            query.addQueryItem("limit", "999");
            break;
        }
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

void api::MarketStack::_handler_answer(Request type, QByteArray data, QString name, bool stream)
{
    qDebug() << "handler answer";
    qDebug() << data;
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    qDebug() << name << "return data" << doc;
    // qDebug() << response;

    QJsonObject root = doc.object();
    if (root.isEmpty())
        return;

    switch (type){
        case api::Request::Quote: {
            QJsonArray array = root.value("data").toArray();
            for (const auto& it : std::as_const(array)){
                QJsonObject obj = it.toObject();
                QString tag = obj.value("symbol").toString();

                auto finded = data::Market::find(tag);
                if (!finded.has_value()){
                    data::Market::add(tag);
                    finded = data::Market::find(tag);
                    if (!finded.has_value())
                        continue;
                }
                data::Ticker* t = finded.value();
                t->quotes()->set_data(QDate::fromString(obj.value("date").toString().left(10),
                                                        "yyyy-MM-dd"),
                                      obj.value("open").toDouble(),
                                      obj.value("close").toDouble(),
                                      obj.value("high").toDouble(),
                                      obj.value("low").toDouble(),
                                      obj.value("volume").toInt());
                t->save();
            }
            break;
        }
        case api::Request::Info: {
            auto finded = data::Market::find(name);
            if (!finded.has_value()){
                data::Market::add(name);
                finded = data::Market::find(name);
                if (!finded.has_value())
                    break;
            }
            data::Ticker* t = finded.value();
            data::Instrument* in = t->instrument();
            t->set_symbol(root.value("symbol").toString());
            t->set_exchange(root.value("stock_exchange").toObject().value("acronym").toString());
            in->identity()->set_title(root.value("name").toString());
            in->identity()->set_sector(root.value("sector").toString());
            in->identity()->set_industry(root.value("industry").toString());
            in->identity()->set_isin(root.value("isin").toString());
            in->identity()->set_country(root.value("stock_exchange").toObject().value("country_code").toString());

            t->save();
            break;
        }
        default:;
    }
}
