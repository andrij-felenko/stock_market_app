#include "core/security/finance/fundamental.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Fundamental& d)
    { return s << d._ebitda << d._book_value << d._revenue_ttm << d._gross_profit_ttm; }

    QDataStream& operator >> (QDataStream& s, Fundamental& d)
    { return s >> d._ebitda >> d._book_value >> d._revenue_ttm >> d._gross_profit_ttm; }
}
