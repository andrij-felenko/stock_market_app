#include "user/account/account.h"


namespace sdk {
    QDataStream& operator << (QDS& s, Wire <const Account> d){
        if (d.data()) s << io(d->ledger, d) << io(d->profile, d);
        return s << io(d->_track, d);
    }

    QDataStream& operator >> (QDS& s, Wire <Account> d){
        if (d.data()) s >> io(d->ledger, d) >> io(d->profile, d);
        return s >> io(d->_track, d);
    }

    QDataStream& operator << (QDS& s, const Account& d) { return s << Wire (d); }
    QDataStream& operator >> (QDS& s,       Account& d) { return s >> Wire (d); }
}
