#include "common/trackable.h"

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Trackable& d) { return s << d._last_updated; }
    QDataStream& operator >> (QDataStream& s,       Trackable& d) { return s >> d._last_updated; }
}
