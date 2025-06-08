#include "twelvedata.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonArray>
#include <QtGui/QGuiApplication>

#include "data/market.h"

api::TwelveData::TwelveData(QObject* parent) : API(parent)
{
    set_api_key("f9b33ba1139a4b5e8c0572bcd1e11258");
}

api::TwelveData* api::TwelveData::instance()
{
    static TwelveData* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new TwelveData(qApp);
    }
    return _instance;
}

void api::TwelveData::set_api_key(const QString& key) {
    m_api_key = key;
}

bool api::TwelveData::_request(Request type, QString name, StringMap keys)
{
    QString base = "https://api.twelvedata.com/";

    QStringList cutted_list = name.split('.');
    cutted_list.removeLast();
    QString cutted = cutted_list.join('.');

    QUrl url;
    QUrlQuery query;

    switch (type) {
        case api::Request::Candle:
            url = base + "time_series";
            if (!keys.contains("start_date") ||
                !keys.contains("end_date") ||
                !keys.contains("interval"))
                return false;

            query.addQueryItem("symbol", cutted);
            query.addQueryItem("interval", keys["interval"]);
            query.addQueryItem("start_date", keys["start_date"]);
            query.addQueryItem("end_date", keys["end_date"]);
            query.addQueryItem("apikey", m_api_key);
            break;
        default:
            return false;
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    API::_add_reply(type, m_manager.get(request), name);
    qDebug() << "request:" << url;
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

void api::TwelveData::_handler_answer(Request type, QByteArray data, QString name)
{
    qDebug() << "handler answer";
    qDebug() << data;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    qDebug() << 1;

    if (obj.contains("status") && obj["status"].toString() == "error") {
        emit error_occurred(obj["message"].toString());
        return;
    }
    qDebug() << 2;

    auto finded = data::Market::find(name);
    if (!finded.has_value()) {
        data::Market::add(name);
        finded = data::Market::find(name);
        qDebug() << finded.has_value();
        if (!finded.has_value())
            return;
    }
    qDebug() << 3;

    data::Ticker* t = finded.value();

    qDebug() << 4;
    if (type == api::Request::Candle && obj.contains("values")) {
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

            t->quotes()->set_data(date, open, close, high, low, volume);
            qDebug() << "=====================================" << date;
        }
        qDebug() << 6;

        t->quotes()->recalculate();
        emit t->update_data();
        qDebug() << 7;
        t->save();
        qDebug() << 8;
    }
    qDebug() << 9;
}
