#ifndef API_FIGI_H
#define API_FIGI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class Figi; }

class api::Figi : public API {
    Q_OBJECT
public:
    static Figi* instance();

    static void update_info_by_tag(const sdk::Symbol& tag);

private:
    explicit Figi(QObject* parent = nullptr);
    using api::API::_request;

    virtual bool _request(Request type, const QString& name, const sdk::Symbol& symbol,
                          StringMap keys = {}) override;
    virtual void _handler_answer(Reply* reply) override;
};

#endif
