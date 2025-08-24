#include "ticker.h"
#include "../instrument.h"
#include "../instrument/quotes.h"

using namespace data;

data::Ticker::Ticker(Instrument* parent) : QObject(parent)
{
    _quotes = new Quotes(this);
    connect(this, &Ticker::signal_save, parent, &Instrument::save);
    connect(this, &Ticker::signal_save, this,   &Ticker::signal_update);
}

data::Ticker& Ticker::operator =(const Ticker& other)
{
    *_quotes = *other._quotes;
    set_symbol(other.symbol());
    return *this;
}

bool        Ticker::is_primary() const { return instrument()->primary_ticker() == this; }
void        Ticker::save()             { emit signal_save(); }
Quotes*     Ticker::quotes()     const { return _quotes; }
Instrument* Ticker::instrument() const { return static_cast <Instrument*> (parent()); }

QString       Ticker::currency_str() const { return geo::currency::to_full(currency()); }
geo::Currency Ticker::    currency() const { return _symbol.currency(); }
QString Ticker::   country() const { return instrument()->identity()->country_str(); }
QString Ticker::   exchange() const { return _symbol.venue(); }

ticker::Symbol Ticker::   symbol()     const { return _symbol; }
QString        Ticker::   symbol_str() const { return _symbol.full(); }
void           Ticker::setSymbol(QString new_symbol) { set_symbol(new_symbol); }

void Ticker::set_symbol(QString new_symbol)          { set_symbol(ticker::Symbol(new_symbol)); }
void Ticker::set_symbol(QString code, QString exch)  { set_symbol(ticker::Symbol(code, exch)); }
void Ticker::set_symbol(QString code, geo::Exchange e){ set_symbol(ticker::Symbol(code, e)); }

void Ticker::set_symbol(ticker::Symbol symbol)
{
    if (_symbol == symbol)
        return;
    _symbol = symbol;
    emit exchangeChanged(symbol.venue());
    emit symbolChanged(symbol.full());
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Ticker& d)
    { return s << *d._quotes << d._symbol; }

    QDataStream& operator >> (QDataStream& s, Ticker& d)
    { return s >> *d._quotes >> d._symbol; }
}
