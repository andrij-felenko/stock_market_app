#ifndef SDK_UTIL_TRACKABLE_H
#define SDK_UTIL_TRACKABLE_H

// #include "sdk_def.h" // IWYU pragma: keep
#include <QtCore/QDateTime>
#include <QtCore/QDataStream>

namespace sdk { class Trackable; }

class sdk::Trackable
{
public:
    Trackable() : _last_updated(QDateTime()), locker(false) {}

    QDateTime _last_updated;
    bool locker;

    friend QDataStream& operator << (QDataStream& s, const Trackable& d);
    friend QDataStream& operator >> (QDataStream& s,       Trackable& d);
};

#endif // SDK_UTIL_TRACKABLE_H
