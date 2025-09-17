#include "instrument/short_interest.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const ShortInterest& d){
        return s << static_cast <const Trackable&> (d)
               << d._shares
               << d._shares_prior_month;
    }

    QDataStream& operator >> (QDataStream& s, ShortInterest& d){
        return s >> static_cast <Trackable&> (d)
               >> d._shares
               >> d._shares_prior_month;
    }
}
