#include "meta.h"
#include "utilities/features.h"
#include <QtCore/QIODevice>
#include <qregularexpression.h>

meta::Instrument::Instrument()
    : prime_ticker(""),
      country(geo::Country::Unknown),
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
    { return s << d.symbol << d.name; }
    QDataStream& operator >> (QDataStream& s,       Ticker& d)
    { return s >> d.symbol >> d.name; }

    QDataStream& operator << (QDataStream& s, const Instrument::Ticker& d)
    { return s << d.symbol << d.currency; }
    QDataStream& operator >> (QDataStream& s,       Instrument::Ticker& d)
    { return s >> d.symbol >> d.currency; }

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

void meta::Ticker::normalize() const
{
    QString n = name.toUpper();
    n.remove(".");
    n.remove(",");
    n.replace("-", " ");
    n.replace("_", " ");
    n = n.simplified();

    QStringList temp = n.split(' ', Qt::SkipEmptyParts);

    static const QSet<QString> drop {
        "INC","INC.","CORP","CORPORATION","CO","COMPANY","LLC","PLC","PLC.",
        "AG","SE","NV","SA","SPA","GMBH",
        "O","O.N","ON",
        "VZO","VZ","VORZUG","PREF","PFD","NON","NON-VTG","NONVOTING",
        "ADR","CDR","GDR",
        "CLASS","CL","SHARES","ORD","ORDINARY","PREFERRED",
        "AG", "N",
        "A","B","C","D","E","F",
        "SERIES","SER","SER.A","SER.B","SER.C"
    };

    temp.erase(std::remove_if(temp.begin(), temp.end(),
                              [&](const QString& part){ return drop.contains(part); }),
               temp.end());

    _name_normalize = temp.join(' ');
}

const QString& meta::Ticker::name_normalize() const
{
    if (_name_normalize.isEmpty())
        normalize();
    return _name_normalize;
}

void meta::Ticker::clear_cache()
{
    _name_normalize.clear();
}
