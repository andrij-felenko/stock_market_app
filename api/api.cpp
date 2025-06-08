#include "api.h"

api::API::API(QObject* parent) : QObject(parent)
{
    _replies.reserve(100);
}

void api::API::_add_reply(Request type, QNetworkReply* reply, const QString& symbol)
{
    Reply *r = new Reply(type, reply, symbol, this);
    _replies.emplace_back(r);

    connect(r, &Reply::finish,      this, &API::_finish);
}

void api::API::_finish(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError){
        // TODO call error
    }

    std::erase_if(_replies, [reply, this](Reply* r) {
        if (r && r->_reply == reply) {
            _handler_answer(r->_type, r->_buffer, r->_symbol);
            r->deleteLater();
            return true;
        }
        return false;
    });
}
