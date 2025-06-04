#include "reply.h"

api::Reply::Reply(Request type, QNetworkReply* reply, const QString& symbol,
                  QObject* parent)
    : QObject(parent), _type(type), _reply(reply), _symbol(symbol)
{
    connect(_reply, &QNetworkReply::readyRead, this,
            [this](){ emit this->have_answer(_reply); });

    connect(_reply, &QNetworkReply::finished,  this,
            [this](){ emit this->finish(_reply); });
}
