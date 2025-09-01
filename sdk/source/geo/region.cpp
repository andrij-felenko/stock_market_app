#include "geo/geo.h"

namespace sdk::region {
    struct Meta {
        Region  _enum;
        QString _full;
    };

    const std::vector <Meta>& metadata();
}

namespace geo {
    QDataStream& operator << (QDataStream& out, const Region& tag)
    { return out << static_cast <uint16_t > (tag); }

    QDataStream& operator >> (QDataStream& in, Region& tag) {
        uint16_t  val;
        in >> val;
        tag = static_cast <Region> (val);
        return in;
    }
}

QString sdk::operator ~ (sdk::Region c) { return sdk::region::to_string(c); }

QString sdk::region::to_string(sdk::Region type)
{
    for (const auto &it : metadata())
        if (it._enum == type)
            return it._full;
    return "NaN";
}

sdk::Region sdk::region::from_string(const QString& currency)
{
    for (const auto &it : metadata())
        if (it._full.compare(currency, Qt::CaseInsensitive) == 0)
            return it._enum;
    return Region::Unknown;
}

QStringList sdk::region::all_names(Continent c)
{
    QStringList ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        if (c == Continent::None || (+it._enum & 0xFF00) == +c)
            ret.push_back(it._full);
    return ret;
}

std::vector <sdk::Region> sdk::region::all(Continent c)
{
    std::vector <sdk::Region> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        if (c == Continent::None || (+it._enum & 0xFF00) == +c)
            ret.push_back(it._enum);
    return ret;
}

const std::vector <sdk::region::Meta>& sdk::region::metadata()
{
    static std::vector <sdk::region::Meta> _;
    if (not _.empty())
        return _;

    // Europe
    _.emplace_back(Region::CentralEurope, "Central Europe");
    _.emplace_back(Region::BritishIsles,  "British Isles");
    _.emplace_back(Region::Benelux,       "Benelux");
    _.emplace_back(Region::Scandinavia,   "Scandinavia");

    _.emplace_back(Region::ItaloAdriatic, "Italo-Adriatic");
    _.emplace_back(Region::Balkans,       "Balkans");
    _.emplace_back(Region::Iberia,        "Iberia");

    _.emplace_back(Region::EuropeanBlackSea, "European Black Sea");
    _.emplace_back(Region::Visegrad,         "Visegrad");
    _.emplace_back(Region::BalticStates,     "Baltic States");

    // Asia
    _.emplace_back(Region::Arabia,      "Arabia");
    _.emplace_back(Region::Caucasus,    "Caucasus");
    _.emplace_back(Region::MiddleEast,  "Middle East");

    _.emplace_back(Region::CentralAsia,     "Central Asia");
    _.emplace_back(Region::Hindustan,       "Hindustan");
    _.emplace_back(Region::GreaterKhorasan, "Great Khrasan");

    _.emplace_back(Region::SinoJapanese,    "Sino-Japanese");
    _.emplace_back(Region::IndoChina,       "Indo-China");
    _.emplace_back(Region::MalayArchipelago,"Malay Archipelago");

    // America
    _.emplace_back(Region::AngloAmerica,   "Anglo-America");
    _.emplace_back(Region::CentralAmerica, "Central America");
    _.emplace_back(Region::Caribbean,      "Caribbean");
    _.emplace_back(Region::SouthAmerica,   "South America");

    // Africa
    _.emplace_back(Region::Maghreb,    "Maghreb");
    _.emplace_back(Region::NileValley, "Nile Valley");

    _.emplace_back(Region::Sahel,             "Sahel");
    _.emplace_back(Region::GuineaCoast,       "Guinea Coast");
    _.emplace_back(Region::CongoBasin,        "Congo Basin");
    _.emplace_back(Region::HornOfAfrica,      "Horn of Africa");
    _.emplace_back(Region::GreatLakes,        "Great Lakes");
    _.emplace_back(Region::SouthernAfrica,    "Southern Africa");
    _.emplace_back(Region::IndianOceanAfrica, "Indian Ocean Africa");

    // Oceania
    _.emplace_back(Region::AustralianAndNewZealand, "Australia and New Zealand");
    _.emplace_back(Region::Melanesia,               "Melanesia");
    _.emplace_back(Region::Micronesia,              "Micronesia");
    _.emplace_back(Region::Polynesia,               "Polynesia");

    // Super regions
    _.emplace_back(Region::WesternEurope,  "Western Europe");
    _.emplace_back(Region::SouthernEurope, "Southern Europe");
    _.emplace_back(Region::EasternEurope,  "Eastern Europe");

    _.emplace_back(Region::WesternAsia,  "Western Asia");
    _.emplace_back(Region::SouthernAsia, "Southern Asia");
    _.emplace_back(Region::EasternAsia,  "Eastern Asia");

    _.emplace_back(Region::NorthAfrica, "North Africa");
    _.emplace_back(Region::SubSaharan,  "Sub-Saharan Africa");

    return _;
}
