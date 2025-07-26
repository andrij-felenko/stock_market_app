#include "api.h"

api::API::API(QObject* parent) : QObject(parent)
{
    _replies.reserve(100);
}

void api::API::_add_reply(Request type, QNetworkReply* reply, const QString& symbol,
                          std::function<QByteArray (QByteArray)> reader)
{
    Reply *r = new Reply(type, reply, symbol, this, reader);
    _replies.push_back(r);

    connect(r, &Reply::finish, this, &API::_finish);
}

void api::API::_finish(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError){
        emit error_occurred(reply->errorString());
        emit error_reply(reply);
        return;
    }

    std::erase_if(_replies, [reply, this](Reply* r) {
        if (r && r->_reply == reply) {
            _handler_answer(r->_type, r->_buffer, r->_symbol, r->_reader != nullptr);
            r->deleteLater();
            return true;
        }
        return false;
    });
}
