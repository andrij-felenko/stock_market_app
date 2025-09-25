#include "core/security/legal.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Legal> d){
        s << d->_open_figi << d->_lei << d->_cik << d->_ein << d->_cusip
          << d->_ipo << d->_international_domestic << d->_fiscal_yearend
          << d->_delisted << d->_address << d->_address_json;
        if (d.recursive) s << static_cast <const Trackable&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Legal> d){
        s >> d->_open_figi >> d->_lei >> d->_cik >> d->_ein >> d->_cusip
          >> d->_ipo >> d->_international_domestic >> d->_fiscal_yearend
          >> d->_delisted >> d->_address >> d->_address_json;
        if (d.recursive) s >> static_cast <Trackable&> (d.ref);
        return s;
    }
}
