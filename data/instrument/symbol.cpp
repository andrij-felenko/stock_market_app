#include "symbol.h"
#include <QStringList>
#include <QtCore/QDebug>

data::ticker::Symbol::Symbol(QString code, geo::Exchange exchange)
    : _code(code), _venue(exchange) { /* */ }

data::ticker::Symbol::Symbol(QString code, QString exch)
    : _code(code), _venue(geo::Exchange::Unknown)
{
    set_venue(exch);
}

data::ticker::Symbol::Symbol(const QString& full) : _code(""), _venue(geo::Exchange::Unknown)
{
    // qDebug() << Q_FUNC_INFO << full;
    QStringList list = full.split(".");
    if (list.length() == 1){
        _code = full;
        _venue = geo::Exchange::US;
    }
    else if (list.length() == 2){
        _code = list[0];
        set_venue(list[1]);
    }
}

using dtS = data::ticker::Symbol;

bool dtS::operator == (const Symbol& other) const
{ return _venue == other._venue && _code == other._code; }

bool dtS::Symbol::operator != (const Symbol& other) const
{ return !(*this == other); }

bool dtS::Symbol::operator < (const Symbol& other) const
{ return (_code == other._code) ? _venue < other._venue : _code < other._code; }

dtS::operator QString() const { return full(); }

bool dtS::Symbol::operator == (const geo::Exchange venue) const { return _venue == venue; }
bool dtS::Symbol::operator == (const std::vector <geo::Exchange> list) const
{
    for (const auto& it : list)
        if (*this == it)
            return true;
    return false;
}


QString       dtS::venue()    const { return geo::exchange::venue   (_venue); }
QString       dtS::sufix()    const { return geo::exchange::sufix   (_venue); }
geo::Currency dtS::currency() const { return geo::exchange::currency(_venue); }

void dtS::set_venue(QString str) { set_venue(geo::exchange::from_venue_string(str)); }
void dtS::set_venue(geo::Exchange  e) { if (e != geo::Exchange::Unknown) _venue = e; }

geo::Exchange dtS::exchange() const { return _venue; }
QString dtS::full() const { return QString("%1.%2").arg(_code, sufix()); }
QString dtS::code() const { return _code; }

void dtS::set_code(QString code) { _code = code.toUpper(); }
void dtS::clear() { _code.clear(); _venue = geo::Exchange::Unknown; }
bool dtS::empty() const { return not exist() || _code.isEmpty(); }

bool dtS::lse_outer() const
{ return _venue == geo::Exchange::LSE && ! _code.isEmpty() &&  _code[0].isDigit(); }

bool dtS::lse_inner() const
{ return _venue == geo::Exchange::LSE && ! _code.isEmpty() && !_code[0].isDigit(); }

bool dtS::us()        const { return geo::exchange::us       (_venue); }
bool dtS::nyse()      const { return geo::exchange::nyse     (_venue); }
bool dtS::nasdaq()    const { return geo::exchange::nasdaq   (_venue); }
bool dtS::otc()       const { return geo::exchange::otc      (_venue); }
bool dtS::europe()    const { return geo::exchange::europe   (_venue); }
bool dtS::trash()     const { return geo::exchange::trash    (_venue); }
bool dtS::euromajor() const { return geo::exchange::euromajor(_venue); }
bool dtS::eurominor() const { return geo::exchange::eurominor(_venue); }
bool dtS::asia()      const { return geo::exchange::asia     (_venue); }
bool dtS::world()     const { return geo::exchange::world    (_venue); }
bool dtS::exist()     const { return geo::exchange::exist    (_venue); }

bool dtS::check_exchange(geo::Exchange ex) const { return _venue == ex; }
// bool dtS::check_exchange(QString ex) const
// { return !empty() && geo::exchange::from_string(ex) == _venue; }

namespace data::ticker {
    QDataStream& operator << (QDataStream& s, const Symbol& d) { return s << d._code << d._venue; }
    QDataStream& operator >> (QDataStream& s,       Symbol& d) { return s >> d._code >> d._venue; }

    QDebug operator << (QDebug dbg, const Symbol& symbol) {
        QDebugStateSaver saver(dbg);
        dbg.nospace() << "Symbol(" << symbol.code() << "." << symbol.venue() << ")";
        return dbg;
    }
}
