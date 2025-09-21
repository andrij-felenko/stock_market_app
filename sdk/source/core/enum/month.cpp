#include "core/enum.h"

namespace sdk::month {
    struct Meta {
        Month _enum;
        QString _full;
    };

    const std::vector <Meta>& metadata();
}

namespace sdk {
    QDataStream& operator << (QDataStream& out, const Month& m)
    { return out << uint8_t(static_cast <uint8_t> (m)); }

    QDataStream& operator >> (QDataStream& in, Month& m) {
        uint8_t  val;
        in >> val;
        m = static_cast <Month> (val);
        return in;
    }
}

QString sdk::operator ~ (sdk::Month c) { return sdk::month::to_string(c); }

uint8_t sdk::month::to_int(Month c){ return static_cast <uint8_t> (c); }
QString sdk::month::to_string(Month c)
{
    for (const auto &it : metadata())
        if (it._enum == c)
            return it._full;
    return "NaN";
}

sdk::Month sdk::month::from_string(const QString& s)
{
    for (const auto &it : metadata())
        if (it._full.compare(s, Qt::CaseInsensitive) == 0)
            return it._enum;
    return Month::Unknown;
}

QStringList sdk::month::all_names()
{
    QStringList ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._full);

    return ret;
}

std::vector <sdk::Month> sdk::month::all()
{
    std::vector <sdk::Month> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._enum);

    return ret;
}

const std::vector <sdk::month::Meta>& sdk::month::metadata()
{
    static std::vector <sdk::month::Meta> _;
    if (not _.empty())
        return _;

    _.emplace_back(Month::January,   "January");
    _.emplace_back(Month::February,  "February");
    _.emplace_back(Month::March,     "March");
    _.emplace_back(Month::April,     "April");
    _.emplace_back(Month::May,       "May");
    _.emplace_back(Month::June,      "June");
    _.emplace_back(Month::July,      "July");
    _.emplace_back(Month::August,    "August");
    _.emplace_back(Month::September, "September");
    _.emplace_back(Month::October,   "October");
    _.emplace_back(Month::November,  "November");
    _.emplace_back(Month::December,  "December");

    return _;
}
