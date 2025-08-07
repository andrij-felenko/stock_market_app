#include "eod.h"
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonArray>
#include "data/instrument.h"
#include "data/market.h"
#include "loader.h"

api::Eod::Eod(QObject* parent) : API(parent)
{
    // set_api_key("683ebb8bc59b60.11043967");

    connect(this, &API::error_reply, this, [this](QNetworkReply* reply){
        for (const auto& it : _replies)
            if (it->_reply == reply){
                qDebug() << "catch reply";
                if (_exchange_list_queue.length() > 0)
                    _exchange_list_queue.removeFirst();
                _next_get_all_exchange_tag();
            }
    });
}

api::Eod* api::Eod::instance()
{
    static Eod* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Eod(qApp);
    }
    return _instance;
}

// void api::Eod::set_api_key(const QString& key) { _api_key = key; }

void api::Eod::get_all_tag(QString exchange)
{
    Eod* data = Eod::instance();
    data->_request(Request::Exchange, exchange);
}

QMap<QString, QString> exchange_map = {
    { "US",     ".US"  },  // США: без суфікса
    { "LSE",    ".L"   },  // Лондон
    { "XETRA",  ".DE"  },  // Франкфурт (Xetra)

    { "TO",     ".TO"  }, // Торонто
    { "PA",     ".PA"  }, // Париж
    { "BE",     ".BE"  }, // Берлін
    { "BR",     ".BR"  }, // Брюссель
    { "AM",     ".AM"  }, // amsterdam

    // Альтернативні / дублікати
    { "MC",     ".MC"  },  // Мадрид
    { "HE",     ".FI"  },  // Гельсінкі
    { "CO",     ".CO"  },  // Копенгаген
    { "OL",     ".OL"  },  // Осло
    { "ST",     ".ST"  },  // Стокгольм
    { "SW",     ".SW"  },  // Швейцарія
    { "AU",     ".AU"  }, // Australia
    { "TW",     ".TW"  }, // Taiwan
};

void api::Eod::get_all_exchange_tag()
{
    Eod* data = Eod::instance();
    data->_exchange_list_queue = exchange_map.keys();
    data->_next_get_all_exchange_tag();
}

void api::Eod::historical_year(data::ticker::Symbol tag, int8_t year, char period)
{
    qDebug() << "EOD ADD BY TAG" << tag;
    if (year >= 20 || year < 1)
        year = 20;

    QDate date = QDate::currentDate();

    api::StringMap params = {
        { "period", QString(period) },
        { "from", QDate(date.year() - year, date.month(), date.day()).toString("yyyy-MM-dd") },
        { "to", date.toString("yyyy-MM-dd") }
    };
    Eod::instance()->_request(Request::Candle, tag, params);
}

void api::Eod::_next_get_all_exchange_tag()
{
    qDebug() << "Next" << _exchange_list_queue;
    if (_exchange_list_queue.empty()){
        Loader::instance()->market()->save_ticker_meta();
        Loader::instance()->market()->clusterise_ticker_meta();
        return;
    }

    get_all_tag(_exchange_list_queue[0]);
}

// void api::Eod::fetch_ticker_data(const QString& ticker) {
//     QUrl url("https://eodhd.com/api/eod/" + ticker);
//     QUrlQuery query;
//     query.addQueryItem("api_token", _api_key);
//     query.addQueryItem("fmt", "json");
//     query.addQueryItem("from", "2025-03-12");
//     query.addQueryItem("to", "2025-04-24");
//     url.setQuery(query);
//     // QUrl url("https://eodhd.com/api/fundamentals/" + ticker);
//     // QUrlQuery query;
//     // query.addQueryItem("api_token", m_api_key);
//     // query.addQueryItem("fmt", "json");
//     // url.setQuery(query);
//     qDebug() << url;

//     QNetworkRequest request(url);

//     QNetworkReply* reply = _netmanager.get(request);
//     connect(reply, &QNetworkReply::finished, this, [=, this]() {
//         if (reply->error() != QNetworkReply::NoError) {
//             emit error_occurred(reply->errorString());
//             reply->deleteLater();
//             return;
//         }

//         QByteArray response = reply->readAll();
//         qDebug() << response;

//         QJsonDocument doc = QJsonDocument::fromJson(response);
//         qDebug() << response;
//         if (!doc.isObject()) {
//             emit error_occurred("Invalid JSON received");
//             qDebug() << "error" << reply->readAll();
//             reply->deleteLater();
//             return;
//         }

//         emit data_ready(doc.object());
//         reply->deleteLater();
//     });

//     connect(reply, &QNetworkReply::readyRead, this, [=, this]() {
//         QByteArray response = reply->readAll();
//         qDebug() << response;
//     });
// }


bool api::Eod::_request(Request type, QString name, StringMap keys)
{
    QString base("https://eodhd.com/api");
    // as we work only with US marker, we nee to cut .US domen from tag
    QString subname = name;
    if (subname.right(3).toUpper() == ".US")
        subname.chop(3);

    QUrl url;
    switch (type){
        case api::Request::Text:

        case api::Request::MetricAll:
        case api::Request::MetricPrice:
        case api::Request::MetricMargin:
        case api::Request::MetricValuation:

        case api::Request::Info:
        case api::Request::Peers:
        case api::Request::Quote:
        case api::Request::Tag:
        case api::Request::Dividend:
        case api::Request::Earnings:
        case api::Request::Reported: return false;
        case api::Request::Exchange: url = base + "/exchange-symbol-list/" + name; break;
        case api::Request::Candle: url = base + "/eod/" + name; break;
    }

    QUrlQuery query;

    switch (type){
        case api::Request::Exchange: {
            query.addQueryItem("api_token", settings::network()->key_eod());
            query.addQueryItem("fmt", "json");
            break;
        }
        case api::Request::Candle: {
            // https://eodhd.com/api/eod/MCD.US?api_token=683ebb8bc59b60.11043967&fmt=json
            query.addQueryItem("api_token", settings::network()->key_eod());
            query.addQueryItem("period", keys.value("period", "d"));
            query.addQueryItem("fmt", "json");
            query.addQueryItem("from", keys.value("from"));
            query.addQueryItem("to",   keys.value("to"));
            break;
        }
        default:;
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    API::_add_reply(type, _netmanager.get(request), name);
    qDebug() << "request:" << url << _replies.size();
    return true;
}

void api::Eod::_handler_answer(Request type, QByteArray data, QString name, bool stream)
{
    qDebug() << "handler answer eod";
    qDebug() << data;
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // qDebug() << name << "return data" << doc;
    // qDebug() << response;

    switch (type){
    case api::Request::Exchange: {
        _handle_exchange(doc, name);
        break;
    }
    case api::Request::Candle: {
        _handle_candle(doc, name);
        break;
    }
    default:;
    }
}

void api::Eod::_handle_exchange(const QJsonDocument& json, QString name)
{
    auto market = Loader::instance()->market();
    QJsonArray root = json.array();
    for (const auto& it : std::as_const(root)){
        QJsonObject obj = it.toObject();
        meta::Ticker meta;
        meta.symbol.set_code    (obj.value("Code")    .toString());
        meta.symbol.set_exchange(obj.value("Exchange").toString());

        meta.name     = obj.value("Name")    .toString();
        meta.region   = obj.value("Country") .toString();
        meta.currency = obj.value("Currency").toString();
        meta.type     = obj.value("Type")    .toString();

        market->add_meta(meta);
    }
    market->save_ticker_meta();

    // remove echange queue list
    _exchange_list_queue.removeOne(name);
    QTimer::singleShot(0, this, [this](){ _next_get_all_exchange_tag(); });
}

void api::Eod::_handle_candle(const QJsonDocument& json, QString name)
{
    auto finded = Nexus.market()->find(name);
    if (!finded.has_value()) {
        // Nexus.market()->add(name);
        finded = Nexus.market()->find(name);
        if (!finded.has_value())
            return;
    }

    data::Ticker* ticker = finded.value();

    QJsonArray array = json.array();
    for (const QJsonValue& v : std::as_const(array)){
        QJsonObject obj = v.toObject();
        QString dtime   = obj[  "date"].toString();
        float open      = obj[  "open"].toString().toFloat();
        float close     = obj[ "close"].toString().toFloat();
        float high      = obj[  "high"].toString().toFloat();
        float low       = obj[   "low"].toString().toFloat();
        uint64_t volume = obj["volume"].toString().toULongLong();

        QDate date = QDate::fromString(dtime, "yyyy-MM-dd");
        ticker->quotes()->set_data(date, open, close,high, low, volume);
    }

    ticker->quotes()->recalculate();
    emit ticker->update_data();
    ticker->save();
}
