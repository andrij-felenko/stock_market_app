#ifndef SDK_INSTRUMENT_ISIN_H
#define SDK_INSTRUMENT_ISIN_H

#include "sdk.h"

class sdk::Isin
{
public:
    Isin(const QByteArray& isin = "");
    Isin(const QByteArray& code, sdk::Country country);

    Isin& operator = (const Isin& other);
    std::strong_ordering operator <=> (const Isin& other) const;
    bool operator == (const Isin& other) const;

    const QByteArray full() const;
    const QByteArray operator ~ () const;

    sdk::Country country() const;
    QByteArray code() const;
    bool valid() const;

private:
    sdk::Country _country = sdk::Country::Unknown;
    QByteArray _code = "";

    friend QDataStream& operator << (QDataStream& s, const Isin& d);
    friend QDataStream& operator >> (QDataStream& s,       Isin& d);

    friend QDebug operator << (QDebug dbg, const Isin& symbol);
};

#endif // SDK_INSTRUMENT_ISIN_H
