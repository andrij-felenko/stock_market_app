#include "user/ledger/ledger.h"
#include "common/features.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Ledger> d) {
        if (d.data()) s << io(d->_watchlists, d);
        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDataStream& s, Wire <Ledger> d) {
        if (d.data()) s >> io(d->_watchlists, d);
        return s >> io(d->_track, d);
    }

    QDataStream& operator << (QDataStream& s, const Ledger& d)
    { return s << Wire (d); }

    QDataStream& operator >> (QDataStream& s, Ledger& d)
    { return s >> Wire (d); }
}
