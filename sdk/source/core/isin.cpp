#include "core/isin.h"

sdk::Isin::Isin(const QString& isin) : Isin(isin.toLatin1()) { /* */  }
sdk::Isin::Isin(const QByteArray& isin)
{
    if (isin.size() == 12)
        _fill(isin.right(10), country::from_string(isin.left(2)));
}

sdk::Isin::Isin(const QByteArray& code, sdk::Country country)
{
    _fill(code, country);
}

void sdk::Isin::_fill(const QByteArray& code, sdk::Country country)
{
    if (code.size() != 10)
        return;

    // code
    uint64_t i64 = 0;
    for (int i = 0; i < 9; i++){
        uint8_t ch = code[i];
        if (ch >= '0' && ch <= '9') ch -= '0';
        else if (ch >= 'A' && ch <= 'Z') ch -= '7'; // '7' = 'A' - 10
        else if (ch >= 'a' && ch <= 'z') ch -= 'W'; // 'W' = 'a' - 10
        else return;
        i64 |= ch;
        i64 <<= 6;
    }
    uint8_t ch = code[9];
    if (ch >= '0' && ch <= '9') ch -= '0';
    else return;
    i64 |= static_cast <uint16_t> (ch) << 2;
    i64 <<= 4;
    std::memcpy(_.data(), &i64, 8);

    // set country
    uint16_t c = +country;
    std::memcpy(_.data() + 8, &c, 2);

    // set valid to true
    _[0] = static_cast <std::byte> (head_v() | valid_mask);
}

std::strong_ordering sdk::Isin::operator <=> (const Isin& other) const
{
    auto c1 = country();
    auto c2 = other.country();
    if (c1 != c2)
        return c1 <=> c2;

    for (int i = 7; i >= 1; i--){
        if (_[i] > other._[i]) return std::strong_ordering::less;
        if (_[i] < other._[i]) return std::strong_ordering::greater;
    }
    auto h1 =       head_v() & hash_mask;
    auto h2 = other.head_v() & hash_mask;
    return h1 <=> h2;
}

bool sdk::Isin::operator == (const Isin& other) const
{ return operator <=> (other) == std::strong_ordering::equal; }

sdk::Isin& sdk::Isin::operator = (const Isin& other)
{
    std::memcpy(_.data(), other._.data(), _.size());
    return *this;
}

sdk::Isin::operator QByteArray()          const { return full(); }
const QByteArray sdk::Isin::operator ~ () const { return full(); }
const QByteArray sdk::Isin::full() const
{ return code_str() + country::alpha2(country()).toLatin1(); }

sdk::Country sdk::Isin::country() const { return country::from(country_v()); }
uint64_t     sdk::Isin::code()    const { return get_long() & long_mask; }

const QByteArray sdk::Isin::code_str() const
{
    QByteArray data(10, '\0');
    data.reserve(10);
    uint64_t i64 = get_long() >> 6;
    uint8_t ch = i64 & 0b1111u;
    data[9] = ch + '0';
    i64 >>= 4;

    for (int i = 8; i >= 0; i--){
        ch = i64 & 0b11'1111u;
        if (ch >= 10) data[i] = '7' + ch; // '7' mean 'A' - 10
        else data[i] = '0' + ch;
        i64 >>= 6;
    }
    return data;
}

bool sdk::Isin::valid() const { return (head_v() &  valid_mask) != 0; }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Isin& d)
    { s.writeRawData(reinterpret_cast <const char*> (d._.data()), 10); return s; }

    QDataStream& operator >> (QDataStream& s, Isin& d)
    { s.readRawData(reinterpret_cast <char*> (d._.data()), 10); return s; }

    QDebug operator << (QDebug dbg, const Isin& isin) {
        QDebugStateSaver saver(dbg);
        dbg.nospace() << "Isin(" << sdk::country::alpha2(isin.country()) << isin.code_str() << ")";
        return dbg;
    }
}
