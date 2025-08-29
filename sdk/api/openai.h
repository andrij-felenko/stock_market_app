#ifndef API_OPENAI_H
#define API_OPENAI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "api/api.h"

namespace api { class OpenAI; }

class api::OpenAI : public API {
    Q_OBJECT
public:
    static OpenAI* instance();

    // void set_api_key(const QString& key);
    void set_model(QString new_model);

public slots:
    void recheck_tag(QString tag);

signals:
    void reply_complete(const QString& full_text);

private:
    explicit OpenAI(QObject* parent = nullptr);

    // QString _api_key;
    QString _model;

    virtual bool _request(Request type, QString name, StringMap keys = {}) override;
    virtual void _handler_answer(Request type, QByteArray data,
                                 QString name, bool stream = false) override;
};

#endif // API_OPENAI_H
