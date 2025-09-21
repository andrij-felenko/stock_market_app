#include "core/security/data.h"
#include "common/features.h"

sdk::Data::Data(uint16_t parent) : _parent(parent)
{
    tickers->reserve(10);
}

void sdk::Data::save() const
{
    //
}

void sdk::Data::load()
{
    //
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
    QDataStream& operator << (QDataStream& s, const Data& d){
        return s << d.legal << d.meta << d.finance << d.tickers;
    }

    QDataStream& operator >> (QDataStream& s, Data& d){
        s >> d.legal >> d.meta >> d.finance >> d.tickers;
        d.update_parent();
        return s;
    }
}
