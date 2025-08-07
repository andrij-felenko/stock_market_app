#ifndef DATA_META_H
#define DATA_META_H

#include <vector>
#include "data/currency/name.h"
#include "data/instrument/symbol.h"
#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>
#include <QtCore/QDataStream>

namespace meta {
    struct Ticker;
    struct Instrument;
}

struct meta::Instrument
{
    QString prime_ticker;
    QString country;
    QString title;

    struct Ticker {
        currency::Tag currency;
        data::ticker::Symbol symbol;
        QString country;

        friend QDataStream& operator << (QDataStream& s, const Ticker& d);
        friend QDataStream& operator >> (QDataStream& s,       Ticker& d);
    };
    std::vector <Ticker> list;
    QByteArray data() const;

    explicit Instrument();
    Instrument(const QByteArray& data);

    friend QDataStream& operator << (QDataStream& s, const Instrument& d);
    friend QDataStream& operator >> (QDataStream& s,       Instrument& d);
};

struct meta::Ticker
{
    Ticker(data::ticker::Symbol symbol = data::ticker::Symbol());
    Ticker(const QByteArray& data);
    QByteArray data() const;

    data::ticker::Symbol symbol;
    QString name;
    QString type;
    QString region;
    QString currency;

    currency::Tag currency_tag() const;

    friend QDataStream& operator << (QDataStream& s, const Ticker& d);
    friend QDataStream& operator >> (QDataStream& s,       Ticker& d);
};

using TickerMetaList = std::vector <meta::Ticker>;

#endif
