#ifndef DATA_INSTRUMENT_SYMBOL_H
#define DATA_INSTRUMENT_SYMBOL_H

#include <QDataStream>
#include <QString>

namespace data::ticker {
    class Symbol;
}

class data::ticker::Symbol
{
public:
    enum Exchange : uint8_t {
        // ----------------------------------------------------------------------------------------
        US        = 0b0000'0000, // 00..'....
        NASDAQ    = 0b0001'0001, // 0001'0...
        NYSE      = 0b0001'1000, // 0001'1...
        NYSE_ARCA = NYSE| 0b001,
        NYSE_MKT  = NYSE| 0b010,
        AMEX      = NYSE| 0b011,
        BATS      = NYSE| 0b100,

        PINK        = 0b10'0000, // 0010'....
        OTC_GREY = PINK | 0b001,
        OTC_CE   = PINK | 0b010,
        OTC_BB   = PINK | 0b011,

        OTC       = 0b0011'0000, // 0011'....
        OTC_QB   = OTC | 0b0001,
        OTC_QX   = OTC | 0b0010,
        NMFQA    = OTC | 0b0101,
        OTC_MKTS = OTC | 0b1001, // 0011'1...
        OTC_MTKS = OTC | 0b1010,
        // ----------------------------------------------------------------------------------------
        World         = 0b0100'0000, // 01..'....

        America = World | 0b00'0000, // 0100'.... america
        Toronto  = America | 0b0001,
        NEO      = America | 0b0010,
        TSX      = America | 0b0011,

        Africa  = World | 0b01'0000, // 0101'.... africa

        Asia = World | 0b10'0000, // 011.'.... asia
        Taiwan   = Asia | 0b0001,
        Korea    = Asia | 0b0010,
        TEL_AVIV = Asia | 0b0011,

        Oceania  = World | 0b11'1000, // 0111'1... oceania
        Australia = Oceania |  0b001,
        // ----------------------------------------------------------------------------------------
        Unknown = 0b1111'1111,
        // ----------------------------------------------------------------------------------------
        EU        = 0b1000'0000, // 1...'....
        EU1   = EU | 0b000'0000, // 10..'.... // europe main exchanges
        LSE   = EU1 | 0b00'0001,
        PA    = EU1 | 0b00'0010,
        AM    = EU1 | 0b00'0011,
        LU    = EU1 | 0b00'0100,
        DE    = EU1 | 0b01'0000, // 1001'.... // Germany
        BE    = EU1 | DE|0b0001,
        XETRA = EU1 | DE|0b0010,

        EU2  = EU | 0b100'0000, // 11..'....
        EU2C = EU2 | 0b01'0000, // 1101'.... // central europe sub echanges
        SW     = EU2C | 0b0001,
        MC     = EU2C | 0b0010,
        BR     = EU2C | 0b0011,
        VI     = EU2C | 0b0101,
        LS     = EU2C | 0b0110,
        PRAGA  = EU2C | 0b0111,
        WARSH  = EU2C | 0b1000,
        ATHENS = EU2C | 0b1001,
        DE2   = EU2| 0b10'0000, // 1110'.... // Germany 2
        DUSEL = EU2|DE2|0b0011,
        HANOV = EU2|DE2|0b0100,
        MUNIC = EU2|DE2|0b0101,
        STUTG = EU2|DE2|0b0110,
        FRANK = EU2|DE2|0b0111,
        HUMBR = EU2|DE2|0b1000,

        Scand = EU2 | 0b10'0000, // 1110'.... // Scandinavia
        OL     = Scand | 0b0001,
        HE     = Scand | 0b0010,
        CO     = Scand | 0b0011,
        Sweden = Scand | 0b0100, // 1110'01.. // Sweden
        ST     = Sweden |  0b01,
        XSTO   = Sweden |  0b10,
        // ----------------------------------------------------------------------------------------
    };

    Symbol(QString full = "");
    Symbol(QString code, QString exch);
    Symbol(QString code, Exchange exchange);

    static bool us       (Exchange e);
    static bool nyse     (Exchange e);
    static bool nasdaq   (Exchange e);
    static bool otc      (Exchange e);
    static bool europe   (Exchange e);
    static bool trash    (Exchange e);
    static bool euromajor(Exchange e);
    static bool eurominor(Exchange e);
    static bool asia     (Exchange e);
    static bool world    (Exchange e);
    static bool exist    (Exchange e);

    static QString  decription(Exchange e);
    static QString  to_short(Exchange e);
    static Exchange from_short(QString str);
    static QString  exchange_str(Exchange e);
    static Exchange from_exchange(QString str);

    static QList <Exchange> major_europe_sufix();
    static QList <Exchange> minor_europe_sufix();
    static QList <Exchange> other_worlds_sufix();
    static QList <Exchange> us_sufix();

    static QList <Exchange> all_exchange();
    static QStringList all_exchange_short();

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

    bool check_exchange(QString ex) const;
    bool check_exchange(Exchange  ex) const;
    bool contains(QList <Exchange> ex) const;

    QString decription() const;
    QString to_short() const;
    QString exchange_str() const;

    void set_short   (QString str);
    void set_exchange(QString str);
    void set_exchange(Exchange e);

    Exchange exchange() const;
    QString full() const;
    QString code() const;

    void set_code(QString code);

    Symbol& operator =  (const Symbol& other) = default;
    bool    operator == (const Symbol& other) const;
    bool    operator != (const Symbol& other) const;
    bool    operator <  (const Symbol& other) const;
    operator QString() const;
    bool    operator == (const Exchange enum_) const;
    bool    operator == (const std::vector <Exchange> list) const;

    void clear();

private:
    Exchange _enum;
    QString _code;

    struct ExchangeEnumStruct {
        Exchange enum_;
        QString sufix;
        QString exchange;
        QString fullname;
        int8_t priority;
    };

    static const std::vector <ExchangeEnumStruct>& ex_map_list();

    friend QDataStream& operator << (QDataStream& s, const Symbol& d);
    friend QDataStream& operator >> (QDataStream& s,       Symbol& d);

    friend QDebug operator << (QDebug dbg, const Symbol& symbol);
};

namespace data::ticker { using SymbolList = std::vector <Symbol>; }

using ExchangeEnum = data::ticker::Symbol::Exchange;
using ExchangeEnumList = QList <ExchangeEnum>;

#endif // DATA_INSTRUMENT_SYMBOL_H
