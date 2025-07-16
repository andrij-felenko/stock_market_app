#ifndef API_ALPHA_VANTAGE_H
#define API_ALPHA_VANTAGE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class AlphaVantage; }

class api::AlphaVantage : public API {
    Q_OBJECT
public:
    static AlphaVantage* instance();

    void set_api_key(const QString& key);

    static void update_info_by_tag(QString tag);
    static void daily_candle_by_tag(QString tag);
    static void today_candle_by_tag(QString tag);
    static void find_tag(QString str);

signals:
    void error_occurred(const QString& message);

public slots:
    void find_symbol(QString str);

private:
    explicit AlphaVantage(QObject* parent = nullptr);

    QString _api_key;

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif
