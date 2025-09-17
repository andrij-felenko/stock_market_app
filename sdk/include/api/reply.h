#ifndef API_REPLY_H
#define API_REPLY_H

#include "instrument/symbol.h"
#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrlQuery>

namespace api {
    class Reply;
    class API;

    enum class Request : uint8_t {
        Info,
        Quote, // current price, maybe inside today
        MetricAll,
        MetricPrice,
        MetricValuation,
        MetricMargin,
        Earnings,
        Peers,
        Reported,
        Candle, // historical data for pricing
        Dividend,
        Exchange,

        Tag,
        Text,
        Logo,
    };
}

class api::Reply : public QObject
{
    Q_OBJECT
public:
    Reply(Request type, API* parent);

    api::Request type() const;
    QUrl url() const;
    QString suburl;

    QNetworkRequest* request();
    void addQueryItem(const QString& key, const QString& value);

    QByteArray send_data;
    const QByteArray& receive_data() const;

    QString name;
    sdk::Symbol symbol;
    std::function <QByteArray (QByteArray)> reader = nullptr;

    bool available();
    void prepare();
    void send();
    void finish();

signals:
    void done();

private:
    QNetworkReply* _reply = nullptr;
    QNetworkRequest _request;
    api::Request _type;
    API* api() const;

    QUrlQuery _query;

    enum class Status {
        Created,
        InQueue,
        Sended,
        Finished
    };
    Status _status = Status::Created;

    QByteArray _receive_data;
    void ready_read();

    friend class API;
    friend class Eod;
    friend class FileFetch;
    friend class OpenAI;
    friend class AlphaVantage;
    friend class Figi;
    friend class Finnhub;
    friend class TwelveData;
    friend class MarketStack;
};

#endif
