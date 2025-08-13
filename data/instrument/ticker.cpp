#include "ticker.h"
#include "../instrument.h"
#include "../instrument/quotes.h"

using namespace data;

data::Ticker::Ticker(bool primary, Instrument* parent) : QObject(parent), _primary(primary)
{
    _quotes = new Quotes(this);
    connect(this, &Ticker::update_data, parent, &Instrument::save);
}

bool        Ticker::is_primary() const { return _primary; }
void        Ticker::save()             { emit update_data(); }
Quotes*     Ticker::quotes()     const { return _quotes; }
Instrument* Ticker::instrument() const { return static_cast <Instrument*> (parent()); }

QString       Ticker::currency_str() const {return currency::Name::to_full(currency()); }
currency::Tag Ticker::    currency() const { return _currency; }
void          Ticker:: setCurrency(const currency::Tag& new_currency)
{
    if (_currency == new_currency)
        return;
    _currency = new_currency;
    emit currencyChanged(_currency);
}

QString Ticker::   country() const { return _country; }
void    Ticker::setCountry(const QString& new_country)
{
    if (_country == new_country)
        return;
    _country = new_country;
    emit countryChanged(_country);
}

QString Ticker::   exchange() const { return _exchange; }
void    Ticker::setExchange(const QString& new_exchange)
{
    if (_exchange == new_exchange)
        return;
    _exchange = new_exchange;
    emit exchangeChanged(_exchange);
}

ticker::Symbol Ticker::   symbol()     const { return _symbol; }
QString        Ticker::   symbol_str() const { return _symbol.full(); }
void           Ticker::setSymbol(QString new_symbol) { set_symbol(new_symbol); }

void Ticker::set_symbol(QString new_symbol)          { set_symbol(ticker::Symbol(new_symbol)); }
void Ticker::set_symbol(QString code, QString exch)  { set_symbol(ticker::Symbol(code, exch)); }
void Ticker::set_symbol(QString code, ExchangeEnum e){ set_symbol(ticker::Symbol(code, e)); }

void Ticker::set_symbol(ticker::Symbol symbol)
{
    if (_symbol == symbol)
        return;
    _symbol = symbol;
    emit exchangeChanged(symbol.exchange_str());
    emit symbolChanged(symbol.full());
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Ticker& d) {
        return s << *d._quotes << d._currency << d._country << d._exchange << d._symbol;
    }

    QDataStream& operator >> (QDataStream& s, Ticker& d) {
        return s >> *d._quotes >> d._currency >> d._country >> d._exchange >> d._symbol;
    }
}
