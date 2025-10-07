#include "core/security/data.h"
#include "common/features.h"
#include "loader.h"

sdk::Data::Data(uint16_t parent) : _parent(parent)
{
    tickers->reserve(10);
}

sdk::Instrument* sdk::Data::parent() const { return Nexus.market()->findInstrument(_parent); }
sdk::Instrument* sdk::Data::instrument() const { return parent(); }

void sdk::Data::save() const
{
    instrument()->save();
}

void sdk::Data::load()
{
    instrument()->load();
}

std::vector <sdk::Symbol> sdk::Data::tickersSymbolList() const
{
    std::vector <sdk::Symbol> ret;
    ret.reserve(tickers.size());
    for (const auto& it : tickers.range())
        ret.push_back(it._symbol);
    return ret;
}

sdk::Ticker& sdk::Data::addTicker(const Symbol& symbol)
{
    Ticker ticker(symbol, _parent);
    tickers->push_back(std::move(ticker));
    return tickers->back();
}

void sdk::Data::update_parent()
{
    for (auto& it : tickers)
        it._parent = _parent;
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Data> d){
        if (d.subs()) s << io(d->legal, d)
                        << io(d->meta, d)
                        << io(d->finance, d)
                        << io(d->tickers, d);
        return s << d->_track;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Data> d){
        if (d.subs()) s >> io(d->legal, d)
                        >> io(d->meta, d)
                        >> io(d->finance, d)
                        >> io(d->tickers, d);

        d->update_parent();
        return s >> d->_track;
    }
}
