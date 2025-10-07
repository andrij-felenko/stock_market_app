#ifndef SDK_INSTRUMENT_ISIN_H
#define SDK_INSTRUMENT_ISIN_H

#include "sdk_def.h"

/*!
 * \brief The sdk::Isin class
 * ================================================================================================
 * 1111'1111'1111'1111 ~ [0xFFFF, 9-8] - Country
 * code[0]            1111'1100'0000'0000 ~ [0xFC00, 7-6] code[0]
 * code[1]            0000'0011'1111'0000 ~ [0x03F0, 7-6] code[1]
 * code[2]                      0000'1111'1100'0000 ~ [0x0FC0, 6-5] code[2]
 * code[3]                      0000'0000'0011'1111 ~ [0x003F, 6-5] code[3]
 * code[4]                                         1111'1100'0000'0000 ~ [0xFC00, 4-3]
 * code[5]                                         0000'0011'1111'0000 ~ [0x03F0, 4-3] code[5]
 * code[6]                                                   0000'1111'1100'0000 ~ [0x0FC0, 3-2]
 * code[7]                                                   0000'0000'0011'1111 ~ [0x003F, 3-2]
 * code[8] ____________________________________________________ [0xFC00, 1-0] ~ 1111'1100'0000'0000
 *                                                     checksum [0x03C0, 1-0] ~ 0000'0011'1100'0000
 *                                                        valid [0x0020, 1-0] ~ 0000'0000'0010'0000
 *                                                         free [0x001F, 1-0] ~ 0000'0000'0001'1111
 * ================================================================================================
 */
class sdk::Isin
{
public:
    Isin(const QString& isin);
    Isin(const QByteArray& isin = "");
    Isin(const QByteArray& code, sdk::Country country);

    Isin& operator = (const Isin& other);
    std::strong_ordering operator <=> (const Isin& other) const;
    bool operator == (const Isin& other) const;

    operator QByteArray() const;
    const QByteArray full() const;
    const QByteArray operator ~ () const;

    sdk::Country country() const;
    uint64_t code() const;
    const QByteArray code_str() const;
    bool valid() const;

private:
    std::array <std::byte, 10> _ = {};
    void _fill(const QByteArray& code, sdk::Country country);

    template <uint8_t offset> requires (offset <= 8) constexpr uint16_t get() const
    { uint16_t v; std::memcpy(&v, &_ + offset, 2); return v; }

    template <uint8_t offset> requires (offset <= 8) constexpr uint8_t get_ch() const
    { uint16_t v; std::memcpy(&v, &_ + offset, 1); return v; }

    constexpr uint8_t get_long() const { uint64_t v; std::memcpy(&v, &_, 8); return v; }

    constexpr static uint16_t hash_mask  = 0x03C0;
    constexpr static uint16_t valid_mask = 0b0010'0000u;
    constexpr static uint64_t long_mask = 0xFFFF'FFFC;

    constexpr uint16_t negative(uint16_t mask) { return static_cast <uint16_t> (~mask); }

    // template <uint8_t offset> requires (offset <= 8) void clean(uint16_t mask)
    // { std::memcpy(_.data(), get <offset> (negative(mask)), 2); }

    constexpr uint16_t country_v() const { return get    <8> (); }
    constexpr  uint8_t head_v()    const { return get_ch <0> (); }

    friend QDataStream& operator << (QDataStream& s, const Isin& d);
    friend QDataStream& operator >> (QDataStream& s,       Isin& d);

    friend QDebug operator << (QDebug dbg, const Isin& symbol);
};

#endif // SDK_INSTRUMENT_ISIN_H
