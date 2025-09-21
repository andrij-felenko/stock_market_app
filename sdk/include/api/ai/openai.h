#ifndef API_OPENAI_H
#define API_OPENAI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "api/transport/provider.h"


class sdk::api::OpenAI : public Provider {
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
    virtual void handlerAnswer(Call* reply) override;
};

#endif // API_OPENAI_H
