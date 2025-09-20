#include "instrument/legal.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Legal& d){
        return s << d._open_figi << d._lei << d._cik << d._ein << d._cusip
                 << d._ipo << d._international_domestic << d._fiscal_yearend
                 << d._delisted << d._address << d._address_json;
    }

    QDataStream& operator >> (QDataStream& s, Legal& d){
        return s >> d._open_figi >> d._lei >> d._cik >> d._ein >> d._cusip
                 >> d._ipo >> d._international_domestic >> d._fiscal_yearend
                 >> d._delisted >> d._address >> d._address_json;
    }
}
