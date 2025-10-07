#ifndef SDK_COMMON_DTIME_H
#define SDK_COMMON_DTIME_H

#include <QtCore/QDateTime>
#include <QtCore/QDataStream>
#include <array>

namespace sdk {
    class DTime;
    template <class T> class Wire;
}

class sdk::DTime
{
    constexpr static uint8_t  core_mask() { return 0b1000'0111; }
    constexpr static uint8_t  free_mask() { return 0b0111'1000; }
    constexpr static uint8_t nfree_mask() { return static_cast <uint8_t> (~free_mask()); }

    constexpr static uint8_t n_mask(uint8_t mask) { return static_cast <uint8_t> (~mask); }

    template <uint8_t i> requires (i <= 3)
    constexpr static uint8_t boolmask() { return 0b1000u << i; }

    template <uint8_t i> requires (i <= 3)
    constexpr static uint8_t nboolmask() { return static_cast <uint8_t> (~boolmask <i> ()); }


    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    constexpr static uint8_t mask() { return ((1u << size) - 1u) << (3 + i); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    constexpr static uint8_t nmask() { return static_cast <uint8_t> (~mask <i, size> ()); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t cut(uint8_t value) { return ((1u << size) - 1u) & value; }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t cut_v(uint8_t value) { return value & mask <i, size>(); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t read_v(uint8_t value) { return cut_v <i, size>(value) >> (3 + i); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    inline static uint8_t write_v(uint8_t value) { return cut <i, size> (value) << (3 + i); }

public:
    DTime();
    DTime(const QDate& d);
    DTime(const QDateTime& dt);

    [[nodiscard]] QDate date() const;
    bool setDate(const QDate& date);

    [[nodiscard]] QTime time() const;
    bool setTime(const QTime& time);

    operator QDate() const;
    operator QTime() const;
    operator QDateTime() const;

    [[nodiscard]] QDateTime dateTime() const;
    bool setDateTime(const QDateTime& dt);

    [[nodiscard]] bool valid() const;

    template <uint8_t i> requires (i <= 3)
    bool boolflag() const { return (boolmask <i>() & get8()) != 0; }

    template <uint8_t i> requires (i <= 3)
    void setBoolFlag(bool value) {
        if (value) set8(get8() & nboolmask <i>());
        else       set8(get8() |  boolmask <i>());
    }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    uint8_t flag() const { return read_v <i, size> (get8()); }

    template <uint8_t i, uint8_t size> requires (i + size <= 3)
    void setFlag(uint8_t value) { set8((get8() & nmask <i, size>()) | write_v <i, size>(value)); }

private:
    void set32(uint32_t v) { std::memcpy(_.data(), &v, 4); }
    uint32_t get32() const {
        uint32_t v;
        std::memcpy(&v, _.data(), 4);
        return v;
    }

    void set8(uint8_t v) { _[4] = static_cast <std::byte> (v); }
    uint8_t get8() const { return static_cast <uint8_t> (_[4]); }

    std::array <std::byte, 5> _ = {};

    void setValid(bool valid);

    friend QDataStream& operator << (QDataStream& s, Wire <const DTime> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      DTime> d);

    friend QDataStream& operator << (QDataStream& s, const DTime& d);
    friend QDataStream& operator >> (QDataStream& s,       DTime& d);
};

#endif // SDK_COMMON_DTIME_H
