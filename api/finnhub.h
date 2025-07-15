#ifndef API_FINNHUB_H
#define API_FINNHUB_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class FinnHub; }

class api::FinnHub : public API {
    Q_OBJECT
public:
    static FinnHub* instance();

    void set_api_key(const QString& key);

    static void update_info_by_tag(QString tag);

signals:
    void error_occurred(const QString& message);

private:
    explicit FinnHub(QObject* parent = nullptr);

    QString _api_key;

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif
