#include "core/security/listing/dividend.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Dividend> d){
        s << d->_date << d->_ex_date
          << d->_per_share
          << d->_yield
          << d->_forward_annual_yield
          << d->_forward_annual_rate
          << d->_payout_ratio;
        if (d.recursive) s << static_cast <const Trackable&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Dividend> d){
        s >> d->_date << d->_ex_date
          >> d->_per_share
          >> d->_yield
          >> d->_forward_annual_yield
          >> d->_forward_annual_rate
          >> d->_payout_ratio;
        if (d.recursive) s >> static_cast <Trackable&> (d.ref);
        return s;
    }
}
