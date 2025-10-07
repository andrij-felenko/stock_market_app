#include "api/transport/call.h"
#include "api/transport/provider.h"
#include <QtNetwork/QNetworkRequest>

sdk::api::Call::Call(sdk::api::Request type, sdk::api::Provider* parent)
    : QObject(parent), _type(type)
{
    //
}

sdk::api::Request sdk::api::Call::type() const { return _type; }

void sdk::api::Call::readyRead()
{
    QByteArray data = _reply->readAll();
    if (reader) _receive_data += reader(data);
    else        _receive_data += data;
}

bool sdk::api::Call::available()
{
    if (name.isEmpty() && symbol.empty())
        return false;

    if (url().isEmpty())
        return false;

    return _status == Status::InQueue;
}

QUrl sdk::api::Call::url() const { return provider()->url.toString() + suburl; }

QNetworkRequest* sdk::api::Call::request(){ return &_request; }

const QByteArray& sdk::api::Call::receiveData() const { return _receive_data; }

sdk::api::Provider* sdk::api::Call::provider() const { return static_cast <Provider*> (parent()); }

void sdk::api::Call::prepare() { _status = Status::InQueue; provider()->_next(); }
void sdk::api::Call::finish()  { _status = Status::Finished; }

void sdk::api::Call::send()
{
    QUrl temp(url());
    if (not _query.isEmpty())
        temp.setQuery(_query);
    _request.setUrl(temp);

    if (send_data.isEmpty()) // post
        _reply = provider()->_netmanager.get(_request);
    else // get
        _reply = provider()->_netmanager.post(_request, send_data);

    connect(_reply, &QNetworkReply::readyRead, this, &Call::readyRead);
    connect(_reply, &QNetworkReply::finished,  this, &Call::done);
    _status = Status::Sended;
}

void sdk::api::Call::addQueryItem(const QString& key, const QString& value)
{
    _query.addQueryItem(key, value);
}
