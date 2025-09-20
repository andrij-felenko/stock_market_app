#include "api/api.h"

using namespace api;

api::API::API(QUrl url, QObject* parent) : QObject(parent), url(url), _lock(false), shift_ms(0)
{
    //
}

bool API::request(Request t, const QString& n,     StringMap k) { return request(t, n, {}, k); }
bool API::request(Request t, const sdk::Symbol& s, StringMap k) { return request(t, "", s, k); }
void API::handlerError(Reply* reply, QNetworkReply::NetworkError error) { /* */ }

api::Reply* api::API::add(api::Request type)
{
    Reply* ret = new Reply(type, this);
    _queue.push_back(ret);
    connect(ret, &Reply::done, this, [ret, this](){ this->finish(ret->_reply); });
    return ret;
}

void api::API::finish(QNetworkReply* reply)
{
    _queue.pop_front();

    bool error = false;
    if (reply->error() != QNetworkReply::NoError){
        emit errorOccurred(reply->errorString());
        emit errorReply(reply);
        error = true;
    }

    std::erase_if(_queue, [reply, this, error](Reply* r) {
        if (r && r->_reply == reply) {
            if (error) handlerError (r, reply->error());
            else       handlerAnswer(r);
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

bool api::API::queueContains(Request r) const
{
    for (const auto& it : _queue)
        if (it->_type == r)
            return true;
    return false;
}

void api::API::_next()
{
    if (_lock)
        return;

    for (auto& it : _queue){
        if (it->available()){
            _lock = true;
            it->send();
            return;
        }
    }
}
