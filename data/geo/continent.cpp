#include "continent.h"

using namespace geo;

// -------------------- Helpers --------------------
struct ContinentInfo {
    Continent value;
    const char* name;
};

struct SubregionInfo {
    const char* name;
    Subregion value;
    Continent parent;
};

// -------------------- Tables --------------------
static const ContinentInfo CONTINENTS[] = {
    { Continent::Europe,         "Europe" },
    { Continent::Africa,         "Africa" },
    { Continent::North_America,  "North America" },
    { Continent::South_America,  "South America" },
    { Continent::Oceania,        "Oceania" },
    { Continent::Asia,           "Asia" },
    { Continent::None,           "None" }
};

static const SubregionInfo SUBREGIONS[] = {
    // Europe
    { "Western Europe",      Subregion::WesternEurope,    Continent::Europe },
    { "Central Europe",      Subregion::CentralEurope,    Continent::Europe },
    { "British Isles",       Subregion::BritishIsles,     Continent::Europe },
    { "Benelux",             Subregion::Benelux,          Continent::Europe },
    { "Scandinavia",         Subregion::Scandinavia,      Continent::Europe },

    { "Southern Europe",     Subregion::SouthernEurope,   Continent::Europe },
    { "Italo-Adriatic",      Subregion::ItaloAdriatic,    Continent::Europe },
    { "Balkans",             Subregion::Balkans,          Continent::Europe },
    { "Iberia",              Subregion::Iberia,           Continent::Europe },

    { "Eastern Europe",      Subregion::EasternEurope,    Continent::Europe },
    { "European Black Sea",  Subregion::EuropeanBlackSea, Continent::Europe },
    { "Visegrad",            Subregion::Visegrad,         Continent::Europe },
    { "Baltic States",       Subregion::BalticsStates,    Continent::Europe },

    // Asia
    { "Western Asia",        Subregion::WesternAsia,      Continent::Asia },
    { "Arabia",              Subregion::Arabia,           Continent::Asia },
    { "Caucasus",            Subregion::Caucasus,         Continent::Asia },
    { "Middle East",         Subregion::MiddleEast,       Continent::Asia },

    { "Southern Asia",       Subregion::SouthernAsia,     Continent::Asia },
    { "Central Asia",        Subregion::CentalAsia,       Continent::Asia },
    { "Hindustan",           Subregion::Hindustan,        Continent::Asia },
    { "Great Khrasan",       Subregion::GreatKhrasan,     Continent::Asia },

    { "Eastern Asia",        Subregion::EasternAsia,      Continent::Asia },
    { "Sino-Japanese",       Subregion::SinoJapanese,     Continent::Asia },
    { "Indo-China",          Subregion::IndoChina,        Continent::Asia },
    { "Malay Archipelago",   Subregion::MalayArchipelago, Continent::Asia },

    // America
    { "Anglo-America",       Subregion::AngloAmerica,     Continent::North_America },
    { "Central America",     Subregion::CentralAmerica,   Continent::North_America },
    { "Caribbean",           Subregion::Caribbean,        Continent::North_America },
    { "South America",       Subregion::SouthAmerica,     Continent::South_America },

    // Africa
    { "North Africa",        Subregion::NorthAfrica,      Continent::Africa },
    { "Maghreb",             Subregion::Maghreb,          Continent::Africa },
    { "Nile Valley",         Subregion::NileValley,       Continent::Africa },

    { "Sub-Saharan Africa",  Subregion::SubSaharan,       Continent::Africa },
    { "Sahel",               Subregion::Sahel,            Continent::Africa },
    { "Guinea Coast",        Subregion::GuineaCoast,      Continent::Africa },
    { "Congo Basin",         Subregion::CongoBasin,       Continent::Africa },
    { "Horn of Africa",      Subregion::HornOfAfrica,     Continent::Africa },
    { "Great Lakes",         Subregion::GreatLakes,       Continent::Africa },
    { "Southern Africa",     Subregion::SouthernAfrica,   Continent::Africa },
    { "Indian Ocean Africa", Subregion::IndianOceanAfrica,Continent::Africa },

    // Oceania
    { "Australia & New Zealand", Subregion::AustralianAndNewZealand, Continent::Oceania },
    { "Melanesia",               Subregion::Melanesia,               Continent::Oceania },
    { "Micronesia",              Subregion::Micronesia,              Continent::Oceania },
    { "Polynesia",               Subregion::Polynesia,               Continent::Oceania },

    { "Unknown", Subregion::Unknown, Continent::None }
};

// -------------------- ContinentName --------------------
QString ContinentName::to_string(Continent c) {
    for (auto& ci : CONTINENTS)
        if (ci.value == c)
            return ci.name;
    return "Unknown";
}

Continent ContinentName::from_string(const QString& s) {
    QString low = s.trimmed().toLower();
    for (auto& ci : CONTINENTS)
        if (low == QString(ci.name).toLower())
            return ci.value;
    return Continent::None;
}

QStringList ContinentName::allNames() {
    QStringList list;
    for (auto& ci : CONTINENTS)
        if (ci.value != Continent::None)
            list << ci.name;
    return list;
}

QList<Continent> ContinentName::all() {
    QList<Continent> list;
    for (auto& ci : CONTINENTS)
        if (ci.value != Continent::None)
            list << ci.value;
    return list;
}

// -------------------- SubregionName --------------------
QString SubregionName::to_string(Subregion r) {
    for (auto& si : SUBREGIONS)
        if (si.value == r)
            return si.name;
    return "Unknown";
}

Subregion SubregionName::from_string(const QString& s) {
    QString low = s.trimmed().toLower();
    for (auto& si : SUBREGIONS)
        if (low == QString(si.name).toLower())
            return si.value;
    return Subregion::Unknown;
}

QStringList SubregionName::allNames() {
    QStringList list;
    for (auto& si : SUBREGIONS)
        if (si.value != Subregion::Unknown)
            list << si.name;
    return list;
}

QList<Subregion> SubregionName::all() {
    QList<Subregion> list;
    for (auto& si : SUBREGIONS)
        if (si.value != Subregion::Unknown)
            list << si.value;
    return list;
}

Continent SubregionName::continent(Subregion r) {
    for (auto& si : SUBREGIONS)
        if (si.value == r)
            return si.parent;
    return Continent::None;
}

QList<Subregion> SubregionName::ofContinent(Continent c) {
    QList<Subregion> list;
    for (auto& si : SUBREGIONS)
        if (si.parent == c && si.value != Subregion::Unknown)
            list << si.value;
    return list;
}

// -------------------- QDataStream operators --------------------
QDataStream& operator<<(QDataStream& out, const geo::Continent& c) {
    return out << static_cast<quint16>(c);
}
QDataStream& operator>>(QDataStream& in, geo::Continent& c) {
    quint16 v; in >> v;
    c = static_cast<geo::Continent>(v);
    return in;
}
QDataStream& operator<<(QDataStream& out, const geo::Subregion& r) {
    return out << static_cast<quint16>(r);
}
QDataStream& operator>>(QDataStream& in, geo::Subregion& r) {
    quint16 v; in >> v;
    r = static_cast<geo::Subregion>(v);
    return in;
}

QString operator ~(Continent c) { return ContinentName::to_string(c); }
QString operator ~(Subregion c) { return SubregionName::to_string(c); }
