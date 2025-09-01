#include "instrument/symbol.h"
#include <QStringList>
#include <QtCore/QDebug>

sdk::Symbol::Symbol(const QByteArray& code, sdk::Exchange exchange)
    : _code(code), _venue(exchange)
{
    // _();
}

sdk::Symbol::Symbol(QString code, sdk::Exchange exchange)
    : Symbol(code.toLatin1(), exchange)
{
    // _();
}

sdk::Symbol::Symbol(QString code, QString exch)
    : Symbol(code.toLatin1(), sdk::Exchange::Unknown)
{
    // _();
    set_venue(exch);
}

sdk::Symbol::Symbol(const QString& full) : _code(""), _venue(sdk::Exchange::Unknown)
{
    // _();

    // qDebug() << Q_FUNC_INFO << full;
    QStringList list = full.split(".");
    if (list.length() == 1){
        set_code(full);
        set_venue(sdk::Exchange::US);
    }
    else if (list.length() == 2){
        set_code (list[0]);
        set_venue(list[1]);
    }
}

// void data::ticker::Symbol::_()
// {
//     connect(this, &Symbol:: codeChanged, this, &Symbol::fullChanged);
//     connect(this, &Symbol::venueChanged, this, &Symbol::fullChanged);
// }

using dtS = sdk::Symbol;

bool dtS::operator == (const Symbol& other) const
{ return _venue == other._venue && _code == other._code; }

bool dtS::Symbol::operator != (const Symbol& other) const
{ return !(*this == other); }

bool dtS::Symbol::operator < (const Symbol& other) const
{ return (_code == other._code) ? _venue < other._venue : _code < other._code; }

dtS::operator QString() const { return full(); }

bool dtS::Symbol::operator == (const sdk::Exchange venue) const { return _venue == venue; }
bool dtS::Symbol::operator == (const std::vector <sdk::Exchange> list) const
{
    for (const auto& it : list)
        if (*this == it)
            return true;
    return false;
}


QString       dtS::venue()    const { return sdk::exchange::venue   (_venue); }
QString       dtS::sufix()    const { return sdk::exchange::sufix   (_venue); }
sdk::Currency dtS::currency() const { return sdk::exchange::currency(_venue); }

void dtS::set_venue(QString str) { set_venue(sdk::exchange::from_venue_string(str)); }
void dtS::set_venue(sdk::Exchange  e)
{
    if (e == _venue) return;
    _venue = e;
    // emit venueChanged();
}
void dtS::set_code(QString code)
{
    if (_code.compare(code.toLatin1(), Qt::CaseInsensitive) == 0) return;
    _code = code.toUpper().toLatin1();
    // emit codeChanged();
}
void dtS::clear()
{
    set_code("");
    set_venue(sdk::Exchange::Unknown);
}

sdk::Symbol& sdk::Symbol::operator =(const Symbol& other)
{
    set_code(other._code);
    set_venue(other._venue);
    return *this;
}

sdk::Exchange dtS::exchange() const { return _venue; }
QString dtS::full_venue() const { return QString("%1.%2").arg(_code, venue()); }
QString dtS::full() const { return QString("%1.%2").arg(_code, sufix()); }
QString dtS::code() const { return _code; }
bool dtS::empty() const { return not exist() || _code.isEmpty(); }

bool dtS::lse_outer() const
{ return _venue == sdk::Exchange::LSE && ! _code.isEmpty() && _code[0] >= '0' && _code[0] <= '9'; }

bool dtS::lse_inner() const
{ return _venue == sdk::Exchange::LSE && ! _code.isEmpty() &&
            !(_code[0] >= '0' && _code[0] <= '9'); }

bool dtS::us()        const { return sdk::exchange::us       (_venue); }
bool dtS::nyse()      const { return sdk::exchange::nyse     (_venue); }
bool dtS::nasdaq()    const { return sdk::exchange::nasdaq   (_venue); }
bool dtS::otc()       const { return sdk::exchange::otc      (_venue); }
bool dtS::europe()    const { return sdk::exchange::europe   (_venue); }
bool dtS::trash()     const { return sdk::exchange::trash    (_venue); }
bool dtS::euromajor() const { return sdk::exchange::euromajor(_venue); }
bool dtS::eurominor() const { return sdk::exchange::eurominor(_venue); }
bool dtS::asia()      const { return sdk::exchange::asia     (_venue); }
bool dtS::world()     const { return sdk::exchange::world    (_venue); }
bool dtS::exist()     const { return sdk::exchange::exist    (_venue); }

bool dtS::check_exchange(sdk::Exchange ex) const { return _venue == ex; }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Symbol& d) { return s << d._code << d._venue; }
    QDataStream& operator >> (QDataStream& s,       Symbol& d) { return s >> d._code >> d._venue; }

    QDebug operator << (QDebug dbg, const Symbol& symbol) {
        QDebugStateSaver saver(dbg);
        dbg.nospace() << "Symbol(" << symbol.code() << "." << symbol.venue() << ")";
        return dbg;
    }
}
