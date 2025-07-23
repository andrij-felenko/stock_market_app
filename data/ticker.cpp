#include "ticker.h"
#include "instrument.h"

using namespace data;
using namespace data::ticker;

data::Ticker::Ticker(bool primary, Instrument* parent) : QObject(parent), _primary(primary)
{
    _quotes = new ticker::Quotes(this);
}

void        Ticker::save()             { emit update_data(); }
Quotes*     Ticker::quotes()     const { return _quotes; }
Instrument* Ticker::instrument() const { return static_cast <Instrument*> (parent()); }

QString       Ticker::currency_str() const {return currency::Name::to_full(currency()); }
currency::Tag Ticker::    currency() const { return _currency; }
void          Ticker::set_currency(const currency::Tag& new_currency)
{
    if (_currency == new_currency)
        return;
    _currency = new_currency;
    emit currencyChanged(_currency);
}

QString Ticker::    country() const { return _country; }
void    Ticker::set_country(const QString& new_country)
{
    if (_country == new_country)
        return;
    _country = new_country;
    emit countryChanged(_country);
}

QString Ticker::    exchange() const { return _exchange; }
void    Ticker::set_exchange(const QString& new_exchange)
{
    if (_exchange == new_exchange)
        return;
    _exchange = new_exchange;
    emit exchangeChanged(_exchange);
}

QString Ticker::    symbol() const { return _symbol; }
void    Ticker::set_symbol(QString new_symbol)
{
    if (new_symbol.split(".").length() == 1)
        new_symbol += ".US";

    if (_symbol == new_symbol)
        return;
    _symbol = new_symbol;
    emit exchangeChanged(_symbol);
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Ticker& d) {
        return s << *d._quotes << d._currency << d._country << d._exchange << d._symbol;
    }

    QDataStream& operator >> (QDataStream& s, Ticker& d) {
        return s >> *d._quotes >> d._currency >> d._country >> d._exchange >> d._symbol;
    }
}
