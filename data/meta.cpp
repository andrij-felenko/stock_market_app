#include "meta.h"
#include "utilities/features.h"
#include <QtCore/QIODevice>
#include "currency/name.h"

meta::Instrument::Instrument()
    : prime_ticker(""),
      country(""),
      title("")
{
    //
}

meta::Instrument::Instrument(const QByteArray& data) : meta::Instrument()
{
    QDataStream stream(data);
    stream >> *this;
}

QByteArray meta::Instrument::data() const
{
    QByteArray raw;
    QDataStream stream(&raw, QIODevice::WriteOnly);
    stream << *this;
    return raw;
}

namespace meta {
    QDataStream& operator << (QDataStream& s, const Ticker& d)
    { return s << d.symbol << d.name << d.region << d.currency; }
    QDataStream& operator >> (QDataStream& s,       Ticker& d)
    { return s >> d.symbol >> d.name >> d.region >> d.currency; }

    QDataStream& operator << (QDataStream& s, const Instrument::Ticker& d)
    { return s << d.symbol << d.country << d.currency; }
    QDataStream& operator >> (QDataStream& s,       Instrument::Ticker& d)
    { return s >> d.symbol >> d.country >> d.currency; }

    QDataStream& operator << (QDataStream& s, const Instrument& d) {
        s << d.prime_ticker << d.country << d.title;
        return util::list_to_stream(s, d.list);
    }

    QDataStream& operator >> (QDataStream& s, Instrument& d) {
        s >> d.prime_ticker >> d.country >> d.title;
        return util::list_from_stream(s, d.list);
    }
}

meta::Ticker::Ticker(data::ticker::Symbol symbol) : symbol(symbol)
{
    //
}

meta::Ticker::Ticker(const QByteArray& data)
{
    QDataStream stream(data);
    stream >> *this;
}

QByteArray meta::Ticker::data() const
{
    QByteArray raw;
    QDataStream stream(&raw, QIODevice::WriteOnly);
    stream << *this;
    return raw;
}

currency::Tag meta::Ticker::currency_tag() const { return currency::Name::to_enum(currency); }
