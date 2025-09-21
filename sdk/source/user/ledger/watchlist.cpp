#include "user/ledger/watchlist.h"
#include "common/features.h"

sdk::WatchList::WatchList(QString name) : _name(name)
{
    //
}


namespace sdk {
    QDataStream& operator << (QDataStream& s, const WatchList& d) {
        return s << d._name << d._list;
    }

    QDataStream& operator >> (QDataStream& s, WatchList& d) {
        return s >> d._name >> d._list;
    }
}
