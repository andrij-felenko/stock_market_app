#include "user/account/account.h"


namespace sdk {
    QDataStream& operator << (QDS& s, const Account& d) { return s << d.ledger << d.profile; }
    QDataStream& operator >> (QDS& s,       Account& d) { return s >> d.ledger >> d.profile; }
}
