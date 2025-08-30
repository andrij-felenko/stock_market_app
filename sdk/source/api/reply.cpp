#include "api/reply.h"

api::Reply::Reply(Request type, QNetworkReply* reply, const QString& symbol,
                  QObject* parent,
                  std::function <QByteArray (QByteArray)> reader)
    : QObject(parent), _type(type), _reply(reply), _symbol(symbol), _reader(reader)
{
    connect(_reply, &QNetworkReply::readyRead, this, &Reply::ready_read);

    connect(_reply, &QNetworkReply::finished,  this,
            [this](){ emit this->finish(_reply); });
}

void api::Reply::ready_read()
{
    QByteArray data = _reply->readAll();
    if (_reader) _buffer += _reader(data);
    else         _buffer += data;
}
