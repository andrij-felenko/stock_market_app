#include "instrument/ticker.h"
#include "instrument/instrument.h"
#include "instrument/quotes.h"

sdk::Ticker::Ticker(uint16_t parent) : _parent(parent)
{
    //
}

sdk::Ticker::Ticker(const sdk::Symbol& symbol, uint16_t parent) : _parent(parent), _symbol(symbol)
{
    //
}

sdk::Ticker& sdk::Ticker::operator = (const Ticker& other)
{
    // TODO we need for all copy
    return *this;
}

void sdk::Ticker::save() { parent()->save(); }
sdk::Symbol sdk::Ticker::   symbol()     const { return _symbol; }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const sdk::Ticker& d){
        return s << d.corp_action
                 << d.dividend
                 << d.quotes
                 << d.short_interst
                 << d.valuation
                 << d._symbol;
    }

    QDataStream& operator >> (QDataStream& s, sdk::Ticker& d){
        return s >> d.corp_action
                 >> d.dividend
                 >> d.quotes
                 >> d.short_interst
                 >> d.valuation
                 >> d._symbol;
    }
}
