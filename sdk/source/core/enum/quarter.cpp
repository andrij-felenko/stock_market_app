#include "core/enum.h"

namespace sdk::quartel {
    struct Meta {
        Quartel _enum;
        QString _full;
    };

    const std::vector <Meta>& metadata();
}

namespace sdk {
    QDataStream& operator << (QDataStream& out, const Quartel& q)
    { return out << uint8_t(static_cast <uint16_t> (q)); }

    QDataStream& operator >> (QDataStream& in, Quartel& q) {
        uint8_t  val;
        in >> val;
        q = static_cast <Quartel>(val);
        return in;
    }
}

QString sdk::operator ~ (sdk::Quartel q) { return sdk::quartel::to_string(q); }

QString sdk::quartel::to_string(Quartel q)
{
    for (const auto &it : metadata())
        if (it._enum == q)
            return it._full;
    return "NaN";
}

sdk::Quartel sdk::quartel::from_string(const QString& s)
{
    for (const auto &it : metadata())
        if (it._full.compare(s, Qt::CaseInsensitive) == 0)
            return it._enum;
    return Quartel::Annual;
}

sdk::Quartel sdk::quartel::from_month(int month)
{
    if (month >= 1 && month <= 12)
        return static_cast <Quartel> (month / 4 + 1);
    return Quartel::Annual;
}

QStringList sdk::quartel::all_names()
{
    QStringList ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._full);

    return ret;
}

std::vector <sdk::Quartel> sdk::quartel::all()
{
    std::vector <sdk::Quartel> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._enum);

    return ret;
}

const std::vector <sdk::quartel::Meta>& sdk::quartel::metadata()
{
    static std::vector <sdk::quartel::Meta> _;
    if (not _.empty())
        return _;

    _.emplace_back(Quartel::First,  "Q1");
    _.emplace_back(Quartel::Second, "Q2");
    _.emplace_back(Quartel::Third,  "Q3");
    _.emplace_back(Quartel::Fourth, "Q4");
    _.emplace_back(Quartel::Annual, "Annual");

    return _;
}
