#include "api/transport/provider.h"
#include <QtCore/QTimer>

using namespace sdk::api;

Provider::Provider(QUrl url, QObject* parent)
    : QObject(parent), url(url), _lock(false), shift_ms(0)
{
    //
}

bool Provider::request(Request t, const QString& n, StringMap k) { return request(t, n, {}, k); }
bool Provider::request(Request t, const Symbol& s,  StringMap k) { return request(t, "", s, k); }
void Provider::handlerError(Call* reply, QNetworkReply::NetworkError error) { /* */ }

Call* Provider::add(api::Request type)
{
    Call* ret = new Call(type, this);
    _queue.push_back(ret);
    connect(ret, &Call::done, this, [ret, this](){ this->finish(ret->_reply); });
    return ret;
}

void Provider::finish(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
        qDebug() << Q_FUNC_INFO << reply->error();
    // _queue.pop_front();

    bool error = false;
    if (reply->error() != QNetworkReply::NoError){
        emit errorOccurred(reply->errorString());
        emit errorReply(reply);
        error = true;
    }

    std::erase_if(_queue, [reply, this, error](Call* r) {
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
        QTimer::singleShot(shift_ms, this, &Provider::_next);
}

bool Provider::queueContains(Request r) const
{
    for (const auto& it : _queue)
        if (it->_type == r)
            return true;
    return false;
}

void Provider::_next()
{
    // qDebug() << Q_FUNC_INFO << _lock;
    if (_lock)
        return;

    for (auto& it : _queue){
        if (it->available()){
            _lock = true;
            it->send();
            qDebug() << "It need also" << _queue.size() << QDateTime::currentDateTime();
            return;
        }
    }
}
