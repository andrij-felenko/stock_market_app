#include "instrument/capital.h"
#include "utilities/features.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Capital& d){
        sdk::list_to_stream(s, d._outstand_shares);
        return s << d._shares_outstanding << d._shares_float
                 << d._percent_of_insiders << d._percent_institution;
    }

    QDataStream& operator >> (QDataStream& s, Capital& d){
        sdk::list_from_stream(s, d._outstand_shares);
        return s >> d._shares_outstanding >> d._shares_float
                 >> d._percent_of_insiders >> d._percent_institution;
    }

    QDataStream& operator << (QDataStream& s, const Capital::SharesPoint& d)
    { return s << d.shares << d.year << d.quartel; }

    QDataStream& operator >> (QDataStream& s, Capital::SharesPoint& d)
    { return s >> d.shares >> d.year >> d.quartel; }
}
