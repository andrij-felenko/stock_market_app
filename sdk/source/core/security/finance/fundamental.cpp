#include "core/security/finance/fundamental.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Fundamental> d){
        if (d.data()) s << d->_ebitda << d->_book_value << d->_revenue_ttm << d->_gross_profit_ttm;
        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Fundamental> d){
        if (d.data()) s >> d->_ebitda >> d->_book_value >> d->_revenue_ttm >> d->_gross_profit_ttm;
        return s >> io(d->_track, d);
    }
}
