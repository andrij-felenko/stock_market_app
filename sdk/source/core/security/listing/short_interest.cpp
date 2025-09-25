#include "core/security/listing/short_interest.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const ShortInterest> d){
        s << d->_shares << d->_shares_prior_month;
        if (d.recursive) s << static_cast <const Trackable&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <ShortInterest> d){
        s >> d->_shares >> d->_shares_prior_month;
        if (d.recursive) s >> static_cast <Trackable&> (d.ref);
        return s;
    }
}
