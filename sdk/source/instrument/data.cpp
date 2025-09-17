#include "instrument/data.h"
#include "utilities/features.h"

sdk::Data::Data(uint16_t parent) : _parent(parent)
{
    _list.reserve(10);
}

std::size_t sdk::Data::listingsCount() const noexcept { return _list.size(); }


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
    for (const auto& it : _list)
        ret.push_back(it._symbol);
    return ret;
}

sdk::Ticker& sdk::Data::addTicker(const Symbol& symbol)
{
    Ticker ticker(symbol, _parent);
    _list.push_back(std::move(ticker));
    return _list.back();
}

void sdk::Data::update_parent()
{
    for (auto& it : _list)
        it._parent = _parent;
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Data& d){
        sdk::list_to_stream(s, d._list);
        return s << d.legal << d.meta << d.finance;
    }

    QDataStream& operator >> (QDataStream& s, Data& d){
        sdk::list_from_stream(s, d._list);
        d.update_parent();
        return s >> d.legal >> d.meta >> d.finance;
    }
}
