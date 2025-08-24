#ifndef DATA_INSTRUMENT_SYMBOL_H
#define DATA_INSTRUMENT_SYMBOL_H

#include "data/geo/geo.h"
#include <QDataStream>
#include <QString>

namespace data::ticker {
    class Symbol;
}

class data::ticker::Symbol
{
public:
    Symbol(const QString& full = "");
    Symbol(QString code, QString venue);
    Symbol(QString code, geo::Exchange venue);

    bool us()        const;
    bool nyse()      const;
    bool nasdaq()    const;
    bool otc()       const;
    bool europe()    const;
    bool trash()     const;
    bool euromajor() const;
    bool eurominor() const;
    bool asia()      const;
    bool world()     const;
    bool exist()     const;
    bool empty()     const;
    bool lse_outer() const;
    bool lse_inner() const;

    // bool check_exchange(QString ex) const;
    bool check_exchange(geo::Exchange  ex) const;
    bool contains(std::vector <geo::Exchange> ex) const;

    QString full() const;
    QString code() const;
    QString sufix() const;
    QString venue() const;

    geo::Currency currency() const;
    geo::Exchange exchange() const;

    void set_code(QString code);
    void set_venue(QString str);
    void set_venue(geo::Exchange e);

    Symbol& operator =  (const Symbol& other) = default;
    bool    operator == (const Symbol& other) const;
    bool    operator != (const Symbol& other) const;
    bool    operator <  (const Symbol& other) const;
    operator QString() const; // return full()
    bool    operator == (const geo::Exchange venue) const;
    bool    operator == (const std::vector <geo::Exchange> list) const;

    void clear();

private:
    geo::Exchange _venue;
    QString _code;

    friend QDataStream& operator << (QDataStream& s, const Symbol& d);
    friend QDataStream& operator >> (QDataStream& s,       Symbol& d);

    friend QDebug operator << (QDebug dbg, const Symbol& symbol);
};

namespace data::ticker { using SymbolList = std::vector <Symbol>; }

#endif // DATA_INSTRUMENT_SYMBOL_H
