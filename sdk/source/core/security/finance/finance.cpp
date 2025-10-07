#include "core/security/finance/finance.h"

sdk::Finance::Finance()
{
    //
}

sdk::QuartelData& sdk::Finance::quartel(const QDate& date)
{ return quartel(date.year(), sdk::quartel::from_month(date.month())); }

sdk::QuartelData& sdk::Finance::quartel(uint16_t year, Quartel q)
{
    for (auto& it : _quartel_data)
        if (it.year() == year && q == it.quartel())
            return it;

    _quartel_data->emplace_back(year, q);
    return _quartel_data->back();
}

bool sdk::Finance::hasQuartel(const QDate& date) const
{ return hasQuartel(date.year(), sdk::quartel::from_month(date.month())); }

bool sdk::Finance::hasQuartel(uint16_t year, Quartel q) const
{
    for (auto& it : _quartel_data)
        if (it.year() == year && q == it.quartel())
            return true;
    return false;
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Finance> d){
        if (d.subs()) s << io(d->_quartel_data, d)
                        << io(d->capital, d)
                        << io(d->estimates, d)
                        << io(d->fundamental, d);

        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Finance> d){
        if (d.subs()) s >> io(d->_quartel_data, d)
                        >> io(d->capital, d)
                        >> io(d->estimates, d)
                        >> io(d->fundamental, d);

        return s >> io(d->_track, d);
    }
}
