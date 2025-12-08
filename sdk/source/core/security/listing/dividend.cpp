#include "core/security/listing/dividend.h"

sdk::Dividend::Dividend()
{
    //
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Dividend> d){
        if (d.data()) s << d->_date << d->_ex_date
                        << d->_per_share
                        << d->_yield
                        << d->_forward_annual_yield
                        << d->_forward_annual_rate
                        << d->_payout_ratio;

        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Dividend> d){
        if (d.data()) s >> d->_date << d->_ex_date
                        >> d->_per_share
                        >> d->_yield
                        >> d->_forward_annual_yield
                        >> d->_forward_annual_rate
                        >> d->_payout_ratio;

        return s >> io(d->_track, d);
    }
}
