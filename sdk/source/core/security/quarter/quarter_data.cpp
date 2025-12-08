#include "core/security/quarter/quarter_data.h"
#include "common/features.h"

sdk::QuartelData::QuartelData(uint16_t year, Quartel q) : _year(year), _quartel(q)
{
    //
}

sdk::Quartel sdk::QuartelData::quartel() const { return _quartel; }
uint16_t     sdk::QuartelData::year()    const { return _year; }

sdk::Currency sdk::QuartelData::currency() const
{
    return _currency;
}

void sdk::QuartelData::setCurrency(sdk::Currency new_c)
{
    if (_currency == new_c)
        return;

    _currency = new_c;
    balance._currency = new_c;
    cashflow._currency = new_c;
    incomes._currency = new_c;
    trend._currency = new_c;
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const QuartelData> d){
        if (d.data()) s << d->_year << d->_quartel << d->_currency;
        if (d.subs()) s << io(d->balance, d)
                        << io(d->cashflow, d)
                        << io(d->earning, d)
                        << io(d->incomes, d)
                        << io(d->trend, d);
        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <QuartelData> d){
        if (d.data()) s >> d->_year >> d->_quartel >> d->_currency;
        if (d.subs()) s >> io(d->balance, d)
                        >> io(d->cashflow, d)
                        >> io(d->earning, d)
                        >> io(d->incomes, d)
                        >> io(d->trend, d);
        return s >> io(d->_track, d);
    }
}
