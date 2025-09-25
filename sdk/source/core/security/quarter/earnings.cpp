#include "core/security/quarter/earnings.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Earnings> d){
        s << d->_report_date
          << d->_before_market
          << d->_eps_actual
          << d->_eps_estimate;
        if (d.recursive) s << static_cast <const Earnings&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Earnings> d){
        s >> d->_report_date
          >> d->_before_market
          >> d->_eps_actual
          >> d->_eps_estimate;
        if (d.recursive) s >> static_cast <Earnings&> (d.ref);
        return s;
    }
}
