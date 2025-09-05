#ifndef SDK_UTIL_TRACKABLE_H
#define SDK_UTIL_TRACKABLE_H

// #include "sdk.h" // IWYU pragma: keep
#include <QtCore/QDateTime>

namespace sdk { class Trackable; }

class sdk::Trackable
{
public:
    Trackable() : _last_updated(QDateTime()), locker(false) {}

    QDateTime _last_updated;
    bool locker;
};

#endif // SDK_UTIL_TRACKABLE_H
