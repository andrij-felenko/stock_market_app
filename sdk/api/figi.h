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

    static void update_info_by_tag(QString tag);

private:
    explicit Figi(QObject* parent = nullptr);

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif
