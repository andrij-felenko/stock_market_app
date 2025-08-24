#ifndef API_EOD_H
#define API_EOD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include "api/api.h"
#include "data/instrument/symbol.h"

namespace api { class Eod; }

class api::Eod : public API {
    Q_OBJECT
public:
    static Eod* instance();

    // void set_api_key(const QString& key);

    static void get_all_tag(QString exchange);
    static void get_all_exchange_tag();

    // void fetch_ticker_data(const QString& ticker);

    static void fundamental(data::ticker::Symbol tag);
    static void historical_year(data::ticker::Symbol tag, int8_t year = -1, char period = 'd');

signals:
    void data_ready(const QJsonObject& data);

private:
    explicit Eod(QObject* parent = nullptr);

    // QString _api_key;

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;

    void _handle_exchange(const QJsonDocument& json, QString name);
    void _handle_candle  (const QJsonDocument& json, QString name);
    void _handle_info    (const QJsonDocument& json, QString name);
};

#endif
