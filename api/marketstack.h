#ifndef API_MARKETSTACK_H
#define API_MARKETSTACK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class MarketStack; }

class api::MarketStack : public API {
    Q_OBJECT
public:
    static MarketStack* instance();

    static void update_info_by_tag(QString tag);
    static void update_quotes_by_tag(QStringList tags);

private:
    explicit MarketStack(QObject* parent = nullptr);

    // QString _api_key;

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif // API_MARKETSTACK_H
