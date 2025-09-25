#include "common/dtime.h"

constexpr  uint8_t valid_mask()   { return 0b1000'0000; }
constexpr  uint8_t date_lowmask() { return 0b0000'0111; }
constexpr uint32_t date_highmask()          { return 0b1111'1111'1111'1110'0000'0000'0000'0000; }
constexpr uint32_t time_mask()              { return 0b0000'0000'0000'0001'1111'1111'1111'1111; }

constexpr uint8_t nvalid_mask()    { return static_cast <uint8_t> (~valid_mask()); }
constexpr uint8_t ndate_lowmask()  { return static_cast <uint8_t> (~date_lowmask()); }

constexpr uint32_t date_shiftmask() { return 0b10'0100'0000'0000'0000'0000; } // 1747y-06m-07d

sdk::DTime::DTime() { /* */ }

sdk::DTime::DTime(const QDateTime& dt) : DTime() { setDateTime(dt); }

QDate sdk::DTime::date() const
{
    uint64_t jd = date_shiftmask() |
                  ((date_highmask() & get32()) >> 17) |
                  (static_cast <uint32_t> (date_lowmask()  &  get8()) << 15);
    return QDate::fromJulianDay(static_cast <qint64> (jd));
}

bool sdk::DTime::setDate(const QDate& date)
{
    if (not date.isValid()){
        setValid(false);
        return false;
    }

    uint64_t jd = static_cast <uint64_t> (date.toJulianDay());
    set32((get32() & time_mask()) | static_cast <uint32_t> (jd << 17));
    set8(static_cast <uint8_t> (get8() & ndate_lowmask()) |
         static_cast <uint8_t> ((jd >> 15) & date_lowmask()));

    setValid(true);
    return true;
}

QTime sdk::DTime::time() const
{
    return QTime::fromMSecsSinceStartOfDay(1000 * (get32() & time_mask()));
}

bool sdk::DTime::setTime(const QTime& time)
{
    if (not time.isValid()){
        setValid(false);
        return false;
    }

    set32((get32() & date_highmask()) | ((time.msecsSinceStartOfDay() / 1000) & time_mask()));
    setValid(true);
    return true;
}

sdk::DTime::operator QDateTime() const { return dateTime(); }
QDateTime sdk::DTime::dateTime() const { return QDateTime(date(), time()); }

bool sdk::DTime::setDateTime(const QDateTime& dt)
{
    bool dres = setDate(dt.date());
    bool tres = setTime(dt.time());
    setValid(dres && tres);
    return dres && tres;
}

bool sdk::DTime::valid() const { return (get8() & valid_mask()) != 0; }
void sdk::DTime::setValid(bool valid) {
    if (valid) set8(get8() |  valid_mask());
    else       set8(get8() & nvalid_mask());
}

namespace sdk {
    QDataStream& operator << (QDataStream& s, const DTime& d)
    { s.writeRawData(reinterpret_cast<const char*> (d._.data()), 5); return s; }

    QDataStream& operator >> (QDataStream& s, DTime& d)
    { s.readRawData(reinterpret_cast <char*> (d._.data()), 5); return s; }
}
