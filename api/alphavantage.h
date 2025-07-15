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

signals:
    void error_occurred(const QString& message);

private:
    explicit AlphaVantage(QObject* parent = nullptr);

    QNetworkAccessManager m_manager;
    QString m_api_key;
    QNetworkReply* m_reply = nullptr;
    // void _ready_read();

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif
