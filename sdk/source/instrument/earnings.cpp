#include "instrument/earnings.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Earnings& d)
    { return s << d._report_date << d._before_market << d._eps_actual << d._eps_estimate; }

    QDataStream& operator >> (QDataStream& s, Earnings& d)
    { return s >> d._report_date >> d._before_market >> d._eps_actual >> d._eps_estimate; }
}
