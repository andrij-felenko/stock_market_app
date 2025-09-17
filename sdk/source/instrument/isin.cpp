#include "instrument/isin.h"

// CH0`468`525`222
sdk::Isin::Isin(const QByteArray& isin)
{
    if (isin.size() == 12){
        _country = sdk::country::from_string(isin.left(2));
        _code = isin.right(10);
    }
}

sdk::Isin::Isin(const QByteArray& code, sdk::Country country)
{
    if (code.size() == 10){
        _country = country;
        _code = code;
    }
}

std::strong_ordering sdk::Isin::operator <=> (const Isin& other) const
{
    const int c = _code.compare(other._code, Qt::CaseSensitive);
    if (c < 0)  return std::strong_ordering::less;
    if (c > 0)  return std::strong_ordering::greater;
    return _country <=> other._country; // enum class порівнюється як int
}

bool sdk::Isin::operator == (const Isin& other) const
{ return _code == other._code && _country == other._country; }

sdk::Isin& sdk::Isin::operator = (const Isin& other)
{
    _code = other._code;
    _country = other._country;
    return *this;
}

sdk::Country sdk::Isin::country() const { return _country; }
QByteArray   sdk::Isin::code()    const { return _code; }

bool sdk::Isin::valid() const { return _code.size() == 10 && _country != sdk::Country::Unknown; }

namespace sdk {
    QDataStream& operator << (QDataStream& s, const Isin& d) { return s << d._code << d._country; }
    QDataStream& operator >> (QDataStream& s,       Isin& d) { return s >> d._code >> d._country; }

    QDebug operator << (QDebug dbg, const Isin& isin) {
        QDebugStateSaver saver(dbg);
        dbg.nospace() << "Isin(" << sdk::country::alpha2(isin._country) << isin._code << ")";
        return dbg;
    }
}
