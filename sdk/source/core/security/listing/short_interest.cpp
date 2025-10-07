#include "core/security/listing/short_interest.h"

sdk::ShortInterest::ShortInterest()
{
    //
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const ShortInterest> d){
        if (d.data()) s << d->_shares << d->_shares_prior_month;
        return s << d->_track;
    }

    QDataStream& operator >> (QDataStream& s, Wire <ShortInterest> d){
        if (d.data()) s >> d->_shares >> d->_shares_prior_month;
        return s >> d->_track;
    }
}
