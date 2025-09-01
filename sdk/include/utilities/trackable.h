#ifndef SDK_UTIL_TRACKABLE_H
#define SDK_UTIL_TRACKABLE_H

// #include "sdk.h" // IWYU pragma: keep
#include <QtCore/QDateTime>

namespace sdk { class Trackable; }

class sdk::Trackable
{
public:
    Trackable();

    QDateTime _last_updated;
};

#endif // SDK_UTIL_TRACKABLE_H
