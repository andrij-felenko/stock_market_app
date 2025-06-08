#ifndef API_REPLY_H
#define API_REPLY_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

namespace api {
    class Reply;

    enum class Request : uint8_t {
        Info,
        Quote,
        MetricAll,
        MetricPrice,
        MetricValuation,
        MetricMargin,
        Earnings,
        Peers,
        Reported,
        Candle,
        Dividend,
    };
}

class api::Reply : public QObject
{
    Q_OBJECT
public:
    Reply(Request type, QNetworkReply* reply, const QString& symbol,
          QObject*parent = nullptr);

signals:
    void finish(QNetworkReply* reply);

private:
    QNetworkReply* _reply;
    api::Request _type;
    QString _symbol;

    QByteArray _buffer;

    void ready_read();

    friend class API;
};

#endif
