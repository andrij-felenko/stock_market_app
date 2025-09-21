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
    QDataStream& operator << (QDataStream& s, const Finance& d){
        return s << d._quartel_data << d.capital << d.estimates << d.fundamental;
    }

    QDataStream& operator >> (QDataStream& s, Finance& d){
        return s >> d._quartel_data >> d.capital >> d.estimates >> d.fundamental;
    }
}
