#ifndef API_REPLY_H
#define API_REPLY_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

namespace api {
    class Reply;

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
        Candle, // hostorical data for pricing
        Dividend,
        Exchange,

        Tag,
        Text,
    };
}

class api::Reply : public QObject
{
    Q_OBJECT
public:
    Reply(Request type, QNetworkReply* reply, const QString& symbol,
          QObject*parent = nullptr,
          std::function <QByteArray (QByteArray)> reader = nullptr);

signals:
    void finish(QNetworkReply* reply);

private:
    QNetworkReply* _reply;
    api::Request _type;
    QString _symbol;
    std::function <QByteArray (QByteArray)> _reader;

    QByteArray _buffer;

    void ready_read();

    friend class API;
    friend class Eod;
};

#endif
