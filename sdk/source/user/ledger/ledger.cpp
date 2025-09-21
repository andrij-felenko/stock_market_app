#include "user/ledger/ledger.h"
#include "common/features.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Ledger& d) {
        return s << d._watchlists;
    }

    QDataStream& operator >> (QDataStream& s, Ledger& d) {
        return s >> d._watchlists;
    }
}
