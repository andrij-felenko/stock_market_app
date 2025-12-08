#include "core/security/quarter/earnings.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Earnings> d){
        if (d.data()) s << d->_report_date
                        << d->_before_market
                        << d->_eps_actual
                        << d->_eps_estimate;

        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Earnings> d){
        if (d.data()) s >> d->_report_date
                        >> d->_before_market
                        >> d->_eps_actual
                        >> d->_eps_estimate;

        return s >> io(d->_track, d);
    }
}
