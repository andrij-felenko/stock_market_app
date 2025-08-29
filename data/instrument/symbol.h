#ifndef DATA_INSTRUMENT_SYMBOL_H
#define DATA_INSTRUMENT_SYMBOL_H

#include "data/geo/geo.h"
#include <QDataStream>
#include <QString>
#include <QtCore/QObject>

namespace data::ticker {
    class Symbol;
}

class data::ticker::Symbol // : public QObject
{
    // Q_OBJECT
    // Q_PROPERTY(geo::Currency currency   READ currency                 NOTIFY venueChanged)
    // Q_PROPERTY(geo::Exchange exchange   READ exchange WRITE set_venue NOTIFY venueChanged)
    // Q_PROPERTY(QString       venue      READ venue    WRITE set_venue NOTIFY venueChanged)
    // Q_PROPERTY(QString       sufix      READ sufix                    NOTIFY venueChanged)
    // Q_PROPERTY(QString       full       READ full                     NOTIFY fullChanged)
    // Q_PROPERTY(QString       full_venue READ full_venue               NOTIFY fullChanged)
    // Q_PROPERTY(QString       code       READ code     WRITE set_code  NOTIFY  codeChanged)
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

    bool check_exchange(geo::Exchange  ex) const;
    bool contains(std::vector <geo::Exchange> ex) const;

    QString full_venue() const;
    QString full() const;
    QString code() const;
    QString sufix() const;
    QString venue() const;

    geo::Currency currency() const;
    geo::Exchange exchange() const;

    void set_code(QString code);
    void set_venue(QString str);
    void set_venue(geo::Exchange e);

    Symbol& operator =  (const Symbol& other);
    bool    operator == (const Symbol& other) const;
    bool    operator != (const Symbol& other) const;
    bool    operator <  (const Symbol& other) const;
    operator QString() const; // return full()
    bool    operator == (const geo::Exchange venue) const;
    bool    operator == (const std::vector <geo::Exchange> list) const;

    void clear();

// signals:
//     void codeChanged();
//     void fullChanged();
//     void venueChanged();

private:
    // void _();

    geo::Exchange _venue;
    QString _code;

    friend QDataStream& operator << (QDataStream& s, const Symbol& d);
    friend QDataStream& operator >> (QDataStream& s,       Symbol& d);

    friend QDebug operator << (QDebug dbg, const Symbol& symbol);
};

namespace data::ticker { using SymbolList = std::vector <Symbol>; }

#endif // DATA_INSTRUMENT_SYMBOL_H
