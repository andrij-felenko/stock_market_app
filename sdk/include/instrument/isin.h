#ifndef SDK_INSTRUMENT_ISIN_H
#define SDK_INSTRUMENT_ISIN_H

#include "sdk.h"

class sdk::Isin
{
public:
    Isin();
    Isin(const QByteArray& code, sdk::Country country);

    Isin& operator =  (const Isin& other);
    bool  operator == (const Isin& other) const;

private:
    sdk::Country _country;
    QByteArray _code;

    friend QDataStream& operator << (QDataStream& s, const Isin& d);
    friend QDataStream& operator >> (QDataStream& s,       Isin& d);

    friend QDebug operator << (QDebug dbg, const Isin& symbol);
};

#endif // SDK_INSTRUMENT_ISIN_H
