#ifndef API_EOD_H
#define API_EOD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include "api/api.h"

namespace api { class Eod; }

class api::Eod : public API {
    Q_OBJECT
public:
    static Eod* instance();

    // void set_api_key(const QString& key);

    static void get_all_tag(QString exchange);
    static void get_all_exchange_tag();

    // void fetch_ticker_data(const QString& ticker);

signals:
    void data_ready(const QJsonObject& data);
    void error_occurred(const QString& message);

private:
    explicit Eod(QObject* parent = nullptr);

    // QString _api_key;

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif
