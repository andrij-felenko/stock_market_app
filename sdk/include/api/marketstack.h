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

    static void update_info_by_tag(const sdk::Symbol& tag);
    static void update_quotes_by_tag(sdk::SymbolList tags);

private:
    explicit MarketStack(QObject* parent = nullptr);
    using api::API::_request;

    virtual bool _request(Request type, const QString& name, const sdk::Symbol& tag,
                          StringMap keys = {}) override;
    virtual void _handler_answer(Reply* reply) override;
};

#endif // API_MARKETSTACK_H
