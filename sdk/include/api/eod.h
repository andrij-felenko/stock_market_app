#ifndef API_EOD_H
#define API_EOD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include "api/api.h"
#include "instrument/symbol.h"

namespace api { class Eod; }

class api::Eod final : public API {
    Q_OBJECT
public:
    static Eod* instance();

    // void set_api_key(const QString& key);

    static void get_all_tag(QString exchange);
    static void get_all_exchange_tag();

    // void fetch_ticker_data(const QString& ticker);

    static void fundamental(const sdk::Symbol& tag);
    static void historical_year(const sdk::Symbol& tag, int8_t year = -1, char period = 'd');

signals:
    void data_ready(const QJsonObject& data);

private:
    explicit Eod(QObject* parent = nullptr);

    using api::API::_request;
    friend class sdk::Market;
    friend class sdk::Instrument;

    virtual bool _request(Request type, const QString& name, const sdk::Symbol& symbol,
                          StringMap keys = {}) override;
    virtual void _handler_answer(Reply* reply) override;
    virtual void _handler_error(Reply* reply, QNetworkReply::NetworkError error) override;

    void _handle_exchange(Reply* reply);
    void _handle_candle  (Reply* reply);
    void _handle_info    (Reply* reply);
};

#endif
