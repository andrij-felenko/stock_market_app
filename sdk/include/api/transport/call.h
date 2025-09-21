#ifndef SDK_API_TRANSPORT_CALL_H
#define SDK_API_TRANSPORT_CALL_H

#include "core/symbol.h"
#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrlQuery>

namespace sdk::api {
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

class sdk::api::Call : public QObject
{
    Q_OBJECT
public:
    Call(Request type, Provider* parent);

    api::Request type() const;
    QUrl url() const;
    QString suburl;

    QNetworkRequest* request();
    void addQueryItem(const QString& key, const QString& value);

    QByteArray send_data;
    const QByteArray& receiveData() const;

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
    Provider* provider() const;

    QUrlQuery _query;

    enum class Status {
        Created,
        InQueue,
        Sended,
        Finished
    };
    Status _status = Status::Created;

    QByteArray _receive_data;
    void readyRead();

    friend class sdk::api::Provider;
};

#endif
