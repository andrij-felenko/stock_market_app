#ifndef API_ALPHA_VANTAGE_H
#define API_ALPHA_VANTAGE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"
#include "instrument/ticker.h"
#include "services/market.h"

namespace api { class AlphaVantage; }

class api::AlphaVantage : public API {
    Q_OBJECT
public:
    static AlphaVantage* instance();

    static void update_info_by_tag (const sdk::Symbol& tag);
    static void daily_candle_by_tag(const sdk::Symbol& tag);
    static void today_candle_by_tag(const sdk::Symbol& tag);
    static void find_tag(QString str);

public slots:
    void find_symbol(QString str);

private:
    explicit AlphaVantage(QObject* parent = nullptr);
    using api::API::_request;

    virtual bool _request(Request type, const QString& name, const sdk::Symbol& tag,
                          StringMap keys = {}) override;
    virtual void _handler_answer(Reply* reply) override;

    void _handle_info    (Reply* reply);
    void _handle_candle  (Reply* reply);
    void _handle_tag     (Reply* reply);
    void _handle_dividend(Reply* reply);
};

#endif
