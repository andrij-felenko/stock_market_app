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

    // void set_api_key(const QString& key);
    virtual bool _request(Request type, QString name, StringMap keys = {}) override;

    static void update_by_tag(QString tag);

public slots:
    void add_by_tag(QString tag);

signals:
    void error_occurred(const QString& message);

private:
    explicit TwelveData(QObject* parent = nullptr);

    QNetworkAccessManager m_manager;
    // QString m_api_key;
    QNetworkReply* m_reply = nullptr;

    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif // API_TWELVEDATA_H
