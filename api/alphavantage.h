#ifndef API_ALPHA_VANTAGE_H
#define API_ALPHA_VANTAGE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"
#include "data/instrument/ticker.h"

namespace api { class AlphaVantage; }

class api::AlphaVantage : public API {
    Q_OBJECT
public:
    static AlphaVantage* instance();

    static void update_info_by_tag(QString tag);
    static void daily_candle_by_tag(QString tag);
    static void today_candle_by_tag(QString tag);
    static void find_tag(QString str);

public slots:
    void find_symbol(QString str);

private:
    explicit AlphaVantage(QObject* parent = nullptr);

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;

    void _handle_info    (const QJsonObject& root, QString name, data::Ticker* t);
    void _handle_candle  (const QJsonObject& root, data::Ticker* t);
    void _handle_tag     (const QJsonObject& root);
    void _handle_dividend(const QJsonObject& root, data::Ticker* t);
};

#endif
