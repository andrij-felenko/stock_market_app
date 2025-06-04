#include "api.h"

api::API::API(QObject* parent) : QObject(parent)
{
    _replies.reserve(100);
}

void api::API::add_reply(Request type, QNetworkReply* reply, const QString& symbol)
{
    Reply *r = new Reply(type, reply, symbol, this);
    _replies.emplace_back(r);

    connect(r, &Reply::have_answer, this, &API::have_answer);
    connect(r, &Reply::finish,      this, &API::finish);
}

void api::API::finish(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError){
        // TODO call error
    }

    std::erase_if(_replies, [reply](Reply* r) {
        if (r && r->_reply == reply) {
            r->deleteLater();
            return true;
        }
        return false;
    });
}

void api::API::have_answer(QNetworkReply* reply)
{
    qDebug() << "have answer";
    for (const auto& it : _replies)
        if (it->_reply == reply){
            handler_answer(it->_type, reply->readAll(), it->_symbol);
            qDebug() << "found reply";
        }
}
