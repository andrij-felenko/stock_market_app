#include "eod.h"
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonArray>
#include "data/instrument.h"
#include "data/market.h"
#include "loader.h"

api::Eod::Eod(QObject* parent) : API(parent)
{
    //
}

api::Eod* api::Eod::instance()
{
    static Eod* _instance = nullptr;
    if (_instance == nullptr)
        _instance = new Eod(qApp);

    return _instance;
}

void api::Eod::get_all_tag(QString exchange)
{
    Eod* data = Eod::instance();
    data->_send(Request::Exchange, exchange);
}

void api::Eod::get_all_exchange_tag()
{
    Eod* data = Eod::instance();
    QStringList list = data::ticker::Symbol::all_exchange_short();
    for (const auto& it : std::as_const(list))
        data->_send(Request::Exchange, it);
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
    Eod::instance()->_send(Request::Candle, tag, params);
}

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

    // handle exchange queue list
    if (not _queue_contains(Request::Exchange))
        Nexus.market()->clusterise_ticker_meta();
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
        float open      = obj[  "open"].toDouble();
        float close     = obj[ "close"].toDouble();
        float high      = obj[  "high"].toDouble();
        float low       = obj[   "low"].toDouble();
        uint64_t volume = obj["volume"].toDouble();

        QDate date = QDate::fromString(dtime, "yyyy-MM-dd");
        ticker->quotes()->set_data(date, open, close, high, low, volume);
    }

    ticker->quotes()->recalculate();
    ticker->save();
    emit ticker->update_data();
}
