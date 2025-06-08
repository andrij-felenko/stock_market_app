#include "tradernet.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QDebug>
#include <QtCore/QJsonArray>
#include "data/market.h"

api::TraderNet::TraderNet(QObject* parent)
    : API(parent), m_manager(this)
{
    //
}

void api::TraderNet::set_api_key   (const QString& key) {    _api_key = key; }
void api::TraderNet::set_secret_key(const QString& key) { _secret_key = key; }

bool api::TraderNet::_request(Request type, QString name, StringMap keys)
{
    QString base = "https://tradernet.ru/api/";
    QString endpoint;

    switch (type) {
    case Request::Quote:
        endpoint = "quotes";
        break;
    default:
        emit error_occurred("Unsupported request type");
        return false;
    }

    QUrl url(base + endpoint);
    QUrlQuery query;
    query.addQueryItem("q", name);

    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QString payload = QString("q=%1&ts=%2").arg(name).arg(timestamp);
    QString signature = _sign(payload);

    query.addQueryItem("ts", QString::number(timestamp));
    query.addQueryItem("sig", signature);
    query.addQueryItem("apiKey", _api_key);

    url.setQuery(query);

    QNetworkRequest request(url);
    API::_add_reply(type, m_manager.get(request), name);
    return true;
}

QString api::TraderNet::_sign(const QString& msg) const
{
    QByteArray key = _secret_key.toUtf8();
    QByteArray data = msg.toUtf8();
    return QCryptographicHash::hash(key + data, QCryptographicHash::Sha256).toHex();
}

void api::TraderNet::_handler_answer(Request type, QByteArray data, QString name)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();

    if (obj.contains("error")) {
        emit error_occurred(obj["error"].toString());
        return;
    }

    auto opt = data::Market::find(name);
    if (!opt.has_value()) return;
    data::Ticker* t = opt.value();

    switch (type) {
        case Request::Quote: {
            float price = obj["lastPrice"].toDouble();
            t->quotes()->set_data(QDate::currentDate(), price, price, price, price, 0);
            t->quotes()->recalculate();
            break;
        }
        case Request::Candle: {
            QJsonArray candles = obj["candles"].toArray();
            for (const auto& v : std::as_const(candles)) {
                QJsonObject c = v.toObject();
                QDate date = QDate::fromString(c["date"].toString(), Qt::ISODate);
                float open = c["open"].toDouble();
                float high = c["high"].toDouble();
                float low  = c["low"].toDouble();
                float close = c["close"].toDouble();
                uint64_t volume = c["volume"].toVariant().toULongLong();
                t->quotes()->set_data(date, open, close, high, low, volume);
            }
            t->quotes()->recalculate();
            break;
        }
        case Request::Dividend: {
            // handle dividends parsing here
            break;
        }
        default: break;
    }
}
