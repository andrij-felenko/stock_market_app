#include "core/security/finance/finance.h"

sdk::QuartelData& sdk::Finance::quartel(uint16_t year, Quartel q)
{
    for (auto& it : _quartel_data)
        if (it.year() == year && q == it.quartel())
            return it;

    _quartel_data->emplace_back(year, q);
    return _quartel_data->back();
}

bool sdk::Finance::hasQuartel(uint16_t year, Quartel q) const
{
    for (auto& it : _quartel_data)
        if (it.year() == year && q == it.quartel())
            return true;
    return false;
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Finance> d){
        s << io(d->_quartel_data, d.recursive)
          << io(d->capital, d.recursive)
          << io(d->estimates, d.recursive)
          << io(d->fundamental, d.recursive);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Finance> d){
        s >> io(d->_quartel_data, d.recursive)
          >> io(d->capital, d.recursive)
          >> io(d->estimates, d.recursive)
          >> io(d->fundamental, d.recursive);
        return s;
    }
}
