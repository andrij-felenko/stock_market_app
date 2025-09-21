#include "core/security/listing/dividend.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Dividend& d){
        return s << static_cast <const Trackable&> (d)
                 << d._date << d._ex_date
                 << d._per_share
                 << d._yield
                 << d._forward_annual_yield
                 << d._forward_annual_rate
                 << d._payout_ratio;
    }

    QDataStream& operator >> (QDataStream& s, Dividend& d){
        return s >> static_cast <Trackable&> (d)
                 >> d._date << d._ex_date
                 >> d._per_share
                 >> d._yield
                 >> d._forward_annual_yield
                 >> d._forward_annual_rate
                 >> d._payout_ratio;
    }
}
