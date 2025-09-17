#include "instrument/corporate_action.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const CorporateAction& d){
        return s << static_cast <const Trackable&> (d)
               << d._last_split_factor
               << d._last_split_date;
    }

    QDataStream& operator >> (QDataStream& s, CorporateAction& d){
        return s >> static_cast <Trackable&> (d)
               >> d._last_split_factor
               >> d._last_split_date;
    }
}
