#ifndef SDK_UTIL_TRACKABLE_H
#define SDK_UTIL_TRACKABLE_H

// #include "sdk_def.h" // IWYU pragma: keep
#include <QtCore/QDataStream>
#include <QtCore/QDateTime>
#include "dtime.h"
// #include "common/features.h"

namespace sdk {
    class Trackable;

    template <typename T, typename T2, typename Flag> static inline std::optional <Flag>
    set_if(Trackable* ptr, T& field, const T2& value, Flag flag, T eps = T{});
}

class sdk::Trackable
{
public:
    Trackable() : _() { /* */ }

    bool islock() const;
    void   lock();
    void unlock();

    void refresh();
    void refresh(const QDateTime& dt);

    template <uint8_t i> requires (i <= 2)
    bool boolflag() const { return _.boolflag <i+1> (); }

    template <uint8_t i> requires (i <= 2)
    void setBoolFlag(bool value) { _.setBoolFlag <i+1> (value); }

    friend QDataStream& operator << (QDataStream& s, Wire <const Trackable> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Trackable> d);

    template <typename T, typename T2, typename Flag>
    friend inline std::optional <Flag>
    sdk::set_if(Trackable* ptr, T& field, const T2& value, Flag flag, T eps);

private:
    sdk::DTime _;
};

#endif // SDK_UTIL_TRACKABLE_H

