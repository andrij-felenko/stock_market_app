#ifndef TICKER_H
#define TICKER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "currency/name.h"

namespace data { class Ticker; }

class data::Ticker : public QObject
{
public:
    Ticker(QObject* parent = nullptr);

    QString _name;
    QString _ticker;
    QString _description;

    QString _country;
    QString _exchange;
    QString _industry;

    QDate _ipo;

    currency::Tag _currency;
    double _capitalization;

    QUrl _logo;
    QUrl _url;
};

#endif // TICKER_H
