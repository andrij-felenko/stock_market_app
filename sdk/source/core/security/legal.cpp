#include "core/security/legal.h"

sdk::Legal::Legal()
{
    //
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Legal> d){
        s << d->_open_figi << d->_lei << d->_cik << d->_ein << d->_cusip
          << d->_ipo << d->_international_domestic << d->_fiscal_yearend
          << d->_address << d->_address_json;
        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Legal> d){
        if (d.data()) s >> d->_open_figi >> d->_lei >> d->_cik >> d->_ein >> d->_cusip
                        >> d->_ipo >> d->_international_domestic >> d->_fiscal_yearend
                        >> d->_address >> d->_address_json;
        return s >> io(d->_track, d);
    }
}
