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
    void setModel(const QString& new_model);

public slots:
    void recheckTag(const sdk::Symbol& tag);

signals:
    void replyComplete(const QString& full_text);

private:
    explicit OpenAI(QObject* parent = nullptr);

    // QString _api_key;
    QString _model;

    virtual bool request(Request type, const QString& name, const sdk::Symbol& symbol,
                         StringMap keys = {}) override;
    virtual void handlerAnswer(Reply* reply) override;
};

#endif // API_OPENAI_H
