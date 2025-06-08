#include "reply.h"

api::Reply::Reply(Request type, QNetworkReply* reply, const QString& symbol,
                  QObject* parent)
    : QObject(parent), _type(type), _reply(reply), _symbol(symbol)
{
    connect(_reply, &QNetworkReply::readyRead, this, &Reply::ready_read);

    connect(_reply, &QNetworkReply::finished,  this,
            [this](){ emit this->finish(_reply); });
}

void api::Reply::ready_read()
{
    _buffer += _reply->readAll();
}
