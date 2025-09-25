#include "core/security/finance/fundamental.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Fundamental> d){
        s << d->_ebitda << d->_book_value << d->_revenue_ttm << d->_gross_profit_ttm;
        if (d.recursive) s << static_cast <const Trackable&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Fundamental> d){
        s >> d->_ebitda >> d->_book_value >> d->_revenue_ttm >> d->_gross_profit_ttm;
        if (d.recursive) s >> static_cast <Trackable&> (d.ref);
        return s;
    }
}
