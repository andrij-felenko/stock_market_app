#ifndef API_TWELVEDATA_H
#define API_TWELVEDATA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/api.h"
#include "reply.h"

namespace api { class TwelveData; }

class api::TwelveData : public API {
    Q_OBJECT
public:
    static TwelveData* instance();

    static void updateByTag(QString tag);

public slots:
    void addByTag(QString tag);

private:
    explicit TwelveData(QObject* parent = nullptr);
    using api::API::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Reply* reply) override;
};

#endif // API_TWELVEDATA_H
