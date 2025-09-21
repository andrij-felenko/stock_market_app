#ifndef SDK_API_EOD_TWELVEDATA_H
#define SDK_API_EOD_TWELVEDATA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "api/transport/provider.h"
#include "api/transport/call.h"

class sdk::api::TwelveData : public sdk::api::Provider {
    Q_OBJECT
public:
    static TwelveData* instance();

    static void updateByTag(QString tag);

public slots:
    void addByTag(QString tag);

private:
    explicit TwelveData(QObject* parent = nullptr);
    using api::Provider::request;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& tag,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Call* reply) override;
};

#endif // SDK_API_EOD_TWELVEDATA_H
