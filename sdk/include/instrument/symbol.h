#ifndef SDK_INSTRUMENT_SYMBOL_H
#define SDK_INSTRUMENT_SYMBOL_H

#include "sdk.h"

class sdk::Symbol // : public QObject
{
    // Q_OBJECT
    // Q_PROPERTY(sdk::Currency currency   READ currency                 NOTIFY venueChanged)
    // Q_PROPERTY(sdk::Exchange exchange   READ exchange WRITE set_venue NOTIFY venueChanged)
    // Q_PROPERTY(QString       venue      READ venue    WRITE set_venue NOTIFY venueChanged)
    // Q_PROPERTY(QString       sufix      READ sufix                    NOTIFY venueChanged)
    // Q_PROPERTY(QString       full       READ full                     NOTIFY fullChanged)
    // Q_PROPERTY(QString       full_venue READ full_venue               NOTIFY fullChanged)
    // Q_PROPERTY(QString       code       READ code     WRITE set_code  NOTIFY  codeChanged)
public:
    Symbol(const QString& full = "");
    Symbol(QString code, QString venue);
    Symbol(QString code, sdk::Exchange venue);

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

    bool check_exchange(sdk::Exchange  ex) const;
    bool contains(std::vector <sdk::Exchange> ex) const;

    QString full_venue() const;
    QString full() const;
    QString code() const;
    QString sufix() const;
    QString venue() const;

    sdk::Currency currency() const;
    sdk::Exchange exchange() const;

    void set_code(QString code);
    void set_venue(QString str);
    void set_venue(sdk::Exchange e);

    Symbol& operator =  (const Symbol& other);
    bool    operator == (const Symbol& other) const;
    bool    operator != (const Symbol& other) const;
    bool    operator <  (const Symbol& other) const;
    operator QString() const; // return full()
    bool    operator == (const sdk::Exchange venue) const;
    bool    operator == (const std::vector <sdk::Exchange> list) const;

    void clear();

// signals:
//     void codeChanged();
//     void fullChanged();
//     void venueChanged();

private:
    // void _();

    sdk::Exchange _venue;
    QString _code;

    friend QDataStream& operator << (QDataStream& s, const Symbol& d);
    friend QDataStream& operator >> (QDataStream& s,       Symbol& d);

    friend QDebug operator << (QDebug dbg, const Symbol& symbol);
};

namespace sdk { using SymbolList = std::vector <Symbol>; }

#endif // SDK_INSTRUMENT_SYMBOL_H
