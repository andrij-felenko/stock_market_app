#include "api/reply.h"
#include "api/api.h"
#include <QtNetwork/QNetworkRequest>

api::Reply::Reply(Request type, API* parent) : QObject(parent), _type(type)
{
    //
}

api::Request api::Reply::type() const { return _type; }

void api::Reply::ready_read()
{
    QByteArray data = _reply->readAll();
    if (reader) _receive_data += reader(data);
    else        _receive_data += data;
}

bool api::Reply::available()
{
    if (name.isEmpty() && symbol.empty())
        return false;

    if (url().isEmpty())
        return false;

    return _status == Status::InQueue;
}

QUrl api::Reply::url() const { return api()->url.toString() + suburl; }

QNetworkRequest* api::Reply::request(){ return &_request; }

const QByteArray& api::Reply::receive_data() const { return _receive_data; }

api::API* api::Reply::api() const { return static_cast <API*> (parent()); }

void api::Reply::prepare() { _status = Status::InQueue; api()->_next(); }
void api::Reply::finish()  { _status = Status::Finished; }

void api::Reply::send()
{
    QUrl temp(url());
    if (not _query.isEmpty())
        temp.setQuery(_query);
    _request.setUrl(temp);

    if (send_data.isEmpty()) // post
        _reply = api()->_netmanager.post(_request, send_data);
    else // get
        _reply = api()->_netmanager.get(_request);

    connect(_reply, &QNetworkReply::readyRead, this, &Reply::ready_read);
    connect(_reply, &QNetworkReply::finished,  this, &Reply::done);
    _status = Status::Sended;
}

void api::Reply::addQueryItem(const QString& key, const QString& value)
{
    _query.addQueryItem(key, value);
}
