#ifndef SDK_UTIL_TRACKABLE_H
#define SDK_UTIL_TRACKABLE_H

// #include "sdk_def.h" // IWYU pragma: keep
#include <QtCore/QDataStream>
#include "dtime.h"

namespace sdk { class Trackable; }

class sdk::Trackable
{
public:
    Trackable() : _last_updated() {}

    sdk::DTime _last_updated;
    bool islock() const { return _last_updated.boolflag <0>(); }
    void   lock() { _last_updated.setBoolFlag <0> (true); }
    void unlock() { _last_updated.setBoolFlag <0> (false); }

    friend QDataStream& operator << (QDataStream& s, const Trackable& d);
    friend QDataStream& operator >> (QDataStream& s,       Trackable& d);
};

#endif // SDK_UTIL_TRACKABLE_H

/*
 * #include "common/dtime.h"

// bool mask, 0 is valid, 1 is not valid
constexpr  uint8_t ndata_mask()   { return 0b0111'1111; }
constexpr  uint8_t valid_mask()   { return 0b0100'0000; }
constexpr  uint8_t lock_mask()    { return 0b1000'0000; }
constexpr uint32_t date_lowmask() { return 0b0000'0111; }
constexpr uint32_t date_highmask()          { return 0b1111'1111'1111'1110'0000'0000'0000'0000; }
constexpr uint32_t time_mask()              { return 0b0000'0000'0000'0001'1111'1111'1111'1111; }

constexpr  uint8_t nvalid_mask()    { return !valid_mask(); }
constexpr  uint8_t nlock_mask()     { return !lock_mask(); }
constexpr uint32_t ndate_lowmask()  { return !date_lowmask(); }
constexpr uint32_t ndate_highmask() { return !date_highmask(); }
constexpr uint32_t ntime_mask()     { return !time_mask(); }

constexpr uint32_t date_shiftmask() { return 0b10'0100'0000'0000'0000'0000; } // 1747y-06m-07d

sdk::DTime::DTime()
{
    //
}

sdk::DTime::DTime(const QDateTime& dt)
{
    //
}

QDate sdk::DTime::date() const
{
    uint64_t jd = date_shiftmask() |
                  ((date_highmask() & _32) >> 17) |
                  ((date_lowmask()  &  _8) << 15);
    return QDate::fromJulianDay(static_cast <qint64> (jd));
}

bool sdk::DTime::setDate(const QDate& date)
{
    if (date.isValid()){
        setValid(false);
        return false;
    }

    uint64_t jd = static_cast <uint64_t> (date.toJulianDay());
    _32 &= (time_mask() | (jd << 17));
    _8  &= ndate_lowmask() | ((jd >> 15) & date_lowmask());

    setValid(true);
    return true;
}

QTime sdk::DTime::time() const
{
    return QTime::fromMSecsSinceStartOfDay(1000 * _32 & time_mask());
}

bool sdk::DTime::setTime(const QTime& time)
{
    if (time.isValid()){
        setValid(false);
        return false;
    }

    _32 &= date_highmask() | (time.msecsSinceStartOfDay() / 1000);
    setValid(true);
    return true;
}

sdk::DTime::operator QDateTime() const { return dateTime(); }
QDateTime sdk::DTime::dateTime() const { return QDateTime(date(), time()); }

bool sdk::DTime::setDateTime(const QDateTime& dt)
{
    bool dres = setDate(dt.date());
    bool tres = setTime(dt.time());
    setValid(dres & tres);
    return dres & tres;
}

bool sdk::DTime::valid() const { return (_8 & lock_mask()) == 0; }
void sdk::DTime::setValid(bool valid) {
    if (valid) _8 &= nvalid_mask();
    else       _8 |=  valid_mask();
}

bool sdk::DTime::islock() const { return (_8 & lock_mask()) == 0; }
void sdk::DTime::  lock() { _8 &= nlock_mask(); }
void sdk::DTime::unlock() { _8 |=  lock_mask(); }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const DTime& d){ return s << d._32 << d._8; }
    QDataStream& operator >> (QDataStream& s,       DTime& d){ return s >> d._32 >> d._8; }
}
*/
