#include "core/security/listing/ticker.h"
#include "core/security/instrument.h"
#include "core/security/listing/quotes.h"
#include "loader.h"

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

sdk::Instrument* sdk::Ticker::parent() const { return Nexus.market()->findInstrument(_parent); }
sdk::Instrument* sdk::Ticker::instrument() const { return parent(); }

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const sdk::Ticker> d){
        return s << io(d->corp_action, d.recursive)
                 << io(d->dividend, d.recursive)
                 << io(d->quotes, d.recursive)
                 << io(d->short_interst, d.recursive)
                 // << d.valuation
                 << d->_symbol;
    }

    QDataStream& operator >> (QDataStream& s, Wire <sdk::Ticker> d){
        return s >> io(d->corp_action, d.recursive)
                 >> io(d->dividend, d.recursive)
                 >> io(d->quotes, d.recursive)
                 >> io(d->short_interst, d.recursive)
                 // >> d.valuation
                 >> d->_symbol;
    }
}
