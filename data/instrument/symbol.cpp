#include "symbol.h"
#include <QStringList>
#include <QtCore/QDebug>

data::ticker::Symbol::Symbol(QString code, geo::Exchange exchange)
    : _code(code), _enum(exchange) { /* */ }

data::ticker::Symbol::Symbol(QString code, QString exch)
    : _code(code), _enum(geo::Exchange::Unknown)
{
    set_exchange(exch);
    set_short(exch);
}

data::ticker::Symbol::Symbol(QString full) : _code(""), _enum(geo::Exchange::Unknown)
{
    QStringList list = full.split(".");
    if (list.length() == 1){
        _code = full;
        _enum = geo::Exchange::US;
    }
    else if (list.length() == 2){
        _code = list[0];
        set_exchange(list[1]);
    }
}

using dtS = data::ticker::Symbol;

bool dtS::operator == (const Symbol& other) const
{ return _enum == other._enum && _code == other._code; }

bool dtS::Symbol::operator != (const Symbol& other) const
{ return !(*this == other); }

bool dtS::Symbol::operator < (const Symbol& other) const
{ return (_code == other._code) ? _enum < other._enum : _code < other._code; }

dtS::operator QString() const { return full(); }

bool dtS::Symbol::operator == (const geo::Exchange enum_) const { return _enum == enum_; }
bool dtS::Symbol::operator == (const std::vector <geo::Exchange> list) const
{
    for (const auto& it : list)
        if (*this == it)
            return true;
    return false;
}


QString       dtS::name()     const { return geo::exchange::name (_enum); }
QString       dtS::sufix()    const { return geo::exchange::sufix(_enum); }
QString       dtS::to_short() const { return geo::exchange::code (_enum); }
geo::Currency dtS::currency() const { return geo::exchange::currency(_enum); }

void dtS::set_short   (QString str) { set_exchange(geo::exchange::from_string(str)); }
void dtS::set_exchange(QString str) { set_exchange(geo::exchange::from_string(str)); }
void dtS::set_exchange(geo::Exchange  e) { if (e != geo::Exchange::Unknown) _enum = e; }

geo::Exchange dtS::exchange() const { return _enum; }
QString dtS::full() const { return QString("%1.%2").arg(_code, to_short()); }
QString dtS::code() const { return _code; }
void dtS::set_code(QString code) { _code = code.toUpper(); }
void dtS::clear() { _code.clear(); _enum = geo::Exchange::Unknown; }
bool dtS::empty() const { return not exist() || _code.isEmpty(); }

bool dtS::lse_outer() const
{ return _enum == geo::Exchange::LSE && ! _code.isEmpty() &&  _code[0].isDigit(); }

bool dtS::lse_inner() const
{ return _enum == geo::Exchange::LSE && ! _code.isEmpty() && !_code[0].isDigit(); }

bool dtS::us()        const { return geo::exchange::us       (_enum); }
bool dtS::nyse()      const { return geo::exchange::nyse     (_enum); }
bool dtS::nasdaq()    const { return geo::exchange::nasdaq   (_enum); }
bool dtS::otc()       const { return geo::exchange::otc      (_enum); }
bool dtS::europe()    const { return geo::exchange::europe   (_enum); }
bool dtS::trash()     const { return geo::exchange::trash    (_enum); }
bool dtS::euromajor() const { return geo::exchange::euromajor(_enum); }
bool dtS::eurominor() const { return geo::exchange::eurominor(_enum); }
bool dtS::asia()      const { return geo::exchange::asia     (_enum); }
bool dtS::world()     const { return geo::exchange::world    (_enum); }
bool dtS::exist()     const { return geo::exchange::exist    (_enum); }

bool dtS::check_exchange(geo::Exchange ex) const { return _enum == ex; }
bool dtS::check_exchange(QString ex) const
{ return !empty() && geo::exchange::from_string(ex) == _enum; }

namespace data::ticker {
    QDataStream& operator << (QDataStream& s, const Symbol& d) { return s << d._code << d._enum; }
    QDataStream& operator >> (QDataStream& s,       Symbol& d) { return s >> d._code >> d._enum; }

    QDebug operator << (QDebug dbg, const Symbol& symbol) {
        QDebugStateSaver saver(dbg);
        dbg.nospace() << "Symbol(" << symbol.code() << "." << symbol.to_short() << ")";
        return dbg;
    }
}
