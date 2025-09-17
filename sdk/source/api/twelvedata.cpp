#include "api/twelvedata.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonArray>
#include <QtGui/QGuiApplication>

#include "services/market.h"
#include "loader.h"

api::TwelveData::TwelveData(QObject* parent) : API(QUrl("https://api.twelvedata.com/"), parent)
{
    // set_api_key("f9b33ba1139a4b5e8c0572bcd1e11258");
}

api::TwelveData* api::TwelveData::instance()
{
    static TwelveData* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new TwelveData(qApp);
    }
    return _instance;
}

// void api::TwelveData::set_api_key(const QString& key) {
    // m_api_key = key;
// }

bool api::TwelveData::_request(Request type, const QString& name, const sdk::Symbol& tag,
                               StringMap keys)
{
    Reply* post = _add(type);

    QString subname;
    if (tag.us()) subname = tag.venue();
    else          subname = tag.full();

    switch (type) {
        case api::Request::Candle:
            post->suburl = "time_series";
            if (!keys.contains("start_date") ||
                !keys.contains("end_date") ||
                !keys.contains("interval"))
                return false;

            post->addQueryItem("symbol", subname);
            post->addQueryItem("interval", keys["interval"]);
            post->addQueryItem("start_date", keys["start_date"]);
            post->addQueryItem("end_date", keys["end_date"]);
            post->addQueryItem("apikey", settings::network()->key_td());
            break;
        default:
            return false;
    }

    post->name = name;
    post->prepare();
    return true;
}

void api::TwelveData::update_by_tag(QString tag)
{
    // TwelveData* data = TwelveData::instance();
}

void api::TwelveData::add_by_tag(QString tag)
{
    qDebug() << "ADD BY TAG" << tag;
    api::StringMap params = {
        { "interval", "1day" },
        { "start_date", QDate(2004, 06, 6).toString("yyyy-MM-dd") },
        { "end_date", QDate(2025, 06, 6).toString("yyyy-MM-dd") }
    };
    _request(Request::Candle, tag, params);
}

void api::TwelveData::_handler_answer(Reply* reply)
{
    qDebug() << "handler answer";
    qDebug() << reply->receive_data();
    QJsonDocument doc = QJsonDocument::fromJson(reply->receive_data());
    QJsonObject obj = doc.object();
    qDebug() << 1;

    if (obj.contains("status") && obj["status"].toString() == "error") {
        emit error_occurred(obj["message"].toString());
        return;
    }
    qDebug() << 2;

    auto t = Nexus.market()->find_ticker(reply->name);
    if (t.ensure() == false)
        return;

    qDebug() << 3;

    qDebug() << 4;
    if (reply->type() == api::Request::Candle && obj.contains("values")) {
        QJsonArray values = obj["values"].toArray();
        qDebug() << 5;
        for (const QJsonValue& v : std::as_const(values)) {
            QJsonObject o = v.toObject();
            QString datetime = o["datetime"].toString();
            float open  = o["open"].toString().toFloat();
            float close = o["close"].toString().toFloat();
            float high  = o["high"].toString().toFloat();
            float low   = o["low"].toString().toFloat();
            uint64_t volume = o["volume"].toString().toULongLong();

            QDate date = QDate::fromString(datetime, "yyyy-MM-dd");
            // або time, залежно від granularності

            t->quotes.setData(date, open, close, high, low, volume);
            qDebug() << "=====================================" << date;
        }
        qDebug() << 6;

        t->save();
        qDebug() << 8;
    }
    qDebug() << 9;
}
