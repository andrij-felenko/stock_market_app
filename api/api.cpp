#include "api.h"

api::API::API(QObject* parent) : QObject(parent), _lock(false), shift_ms(0)
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

void api::API::_send(Request type, QString name, StringMap keys)
{
    for (const auto& it : _queue)
        if (it.type == type && it.name == name && it.keys == keys)
            return;

    _queue.emplace_back(type, name, keys);
    _next();
}

void api::API::_finish(QNetworkReply* reply)
{
    _queue.pop_front();

    bool error = false;
    if (reply->error() != QNetworkReply::NoError){
        emit error_occurred(reply->errorString());
        emit error_reply(reply);
        error = true;
    }

    std::erase_if(_replies, [reply, this, error](Reply* r) {
        if (r && r->_reply == reply) {
            if (not error)
                _handler_answer(r->_type, r->_buffer, r->_symbol, r->_reader != nullptr);
            r->deleteLater();
            return true;
        }
        return false;
    });

    _lock = false;

    if (shift_ms == 0)
        _next();
    else
        QTimer::singleShot(shift_ms, this, &API::_next);
}

bool api::API::_queue_contains(Request r) const
{
    for (const auto& it : _queue)
        if (it.type == r)
            return true;
    return false;
}

void api::API::_next()
{
    if (_queue.empty() || _lock)
        return;

    _lock = true;
    _request(_queue[0].type, _queue[0].name, _queue[0].keys);
}
