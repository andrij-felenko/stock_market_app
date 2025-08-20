#include "geo.h"

namespace geo::continent {
    struct Meta {
        Continent _enum;
        QString _full;
    };

    const std::vector <Meta>& metadata();
}

namespace geo {
    QDataStream& operator << (QDataStream& out, const Continent& tag)
    { return out << uint8_t(static_cast <uint16_t> (tag) >> 8); }

    QDataStream& operator >> (QDataStream& in, Continent& tag) {
        uint8_t  val;
        in >> val;
        tag = static_cast <Continent>(val << 8);
        return in;
    }
}

QString operator ~ (geo::Continent c) { return geo::continent::to_string(c); }

QString geo::continent::to_string(Continent c)
{
    for (const auto &it : metadata())
        if (it._enum == c)
            return it._full;
    return "NaN";
}

geo::Continent geo::continent::from_string(const QString& s)
{
    for (const auto &it : metadata())
        if (it._full.compare(s, Qt::CaseInsensitive) == 0)
            return it._enum;
    return Continent::None;
}

QStringList geo::continent::all_names()
{
    QStringList ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._full);

    return ret;
}

std::vector <geo::Continent> geo::continent::all()
{
    std::vector <geo::Continent> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._enum);

    return ret;
}

const std::vector <geo::continent::Meta>& geo::continent::metadata()
{
    static std::vector <geo::continent::Meta> _;
    if (not _.empty())
        return _;

    _.emplace_back(Continent::Europe,        "Europe");
    _.emplace_back(Continent::Africa,        "Africa");
    _.emplace_back(Continent::North_America, "North America");
    _.emplace_back(Continent::South_America, "South America");
    _.emplace_back(Continent::Oceania,       "Oceania");
    _.emplace_back(Continent::Asia,          "Asia");

    return _;
}
