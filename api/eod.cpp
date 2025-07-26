#include "eod.h"
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonArray>
#include "data/instrument.h"
#include "data/market.h"

api::Eod::Eod(QObject* parent) : API(parent)
{
    // set_api_key("683ebb8bc59b60.11043967");

    connect(this, &API::error_reply, this, [this](QNetworkReply* reply){
        for (const auto& it : _replies)
            if (it->_reply == reply){
                qDebug() << "catch reply";
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
    // { "EPA",    ".PA"  },  // Париж
    // { "MIL",    ".MI"  },  // Мілан
    // { "TSE",    ".T"   },  // Токіо
    // { "HKEX",   ".HK"  },  // Гонконг
    // { "XHEL",   ".FI"  },  // Гельсінкі
    // { "XSTO",   ".ST"  },  // Стокгольм
    // { "XCSE",   ".CO"  },  // Копенгаген
    // { "XOSL",   ".OL"  },  // Осло
    // { "XMAD",   ".MC"  },  // Мадрид
    // { "XMEX",   ".MX"  },  // Мехіко
    // { "XSES",   ".SI"  },  // Сінгапур
    { "TO",     ".TO"  }, // Торонто
    { "PA",     ".PA"  }, // Париж
    { "BE",     ".BE"  }, // Берлін
    { "BR",     ".BR"  }, // Бразилія (B3)
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

void api::Eod::_next_get_all_exchange_tag()
{
    qDebug() << "Next" << _exchange_list_queue;
    if (_exchange_list_queue.empty()){
        data::Market::instance()->save_ticker_meta();
        data::Market::instance()->clusterise_ticker_meta();
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
        case api::Request::Candle:
        case api::Request::Dividend:
        case api::Request::Earnings:
        case api::Request::Reported: return false;
        case api::Request::Exchange: url = base + "/exchange-symbol-list/" + name; break;
    }

    QUrlQuery query;

    switch (type){
        case api::Request::Exchange: {
            query.addQueryItem("api_token", settings::network()->key_eod());
            query.addQueryItem("fmt", "json");
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
    qDebug() << "handler answer";
    // qDebug() << data;
    // QByteArray response = m_reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // qDebug() << name << "return data" << doc;
    // qDebug() << response;

    auto market = data::Market::instance();
    switch (type){
    case api::Request::Exchange: {
        QJsonArray root = doc.array();
        for (const auto& it : std::as_const(root)){
            QJsonObject obj = it.toObject();
            data::TickerMeta meta;
            meta.symbol   = obj.value("Code")    .toString() + exchange_map[name];
            meta.name     = obj.value("Name")    .toString();
            meta.region   = obj.value("Country") .toString();
            meta.exchange = obj.value("Exchange").toString();
            meta.currency = obj.value("Currency").toString();
            meta.type     = obj.value("Type")    .toString();
            // if (meta.name.startsWith("Ren", Qt::CaseInsensitive))
                market->add(meta);
        }
        market->save_ticker_meta();

        // remove echange queue list
        _exchange_list_queue.removeOne(name);
        QTimer::singleShot(0, this, [this](){ _next_get_all_exchange_tag(); });
        break;
    }
    default:;
    }
}
