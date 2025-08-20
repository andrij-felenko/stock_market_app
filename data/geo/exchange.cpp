// exchange.cpp
#include "geo.h"
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDataStream>
#include <QtCore/QList>

namespace geo::exchange {
    struct Meta {
        Exchange e;
        Currency currency; // валюта торгів
        Country  country;  // місце розміщення біржі
        QString  sufix;    // короткий суфікс, що йде після крапки в тікері (AAPL.US → "US")
        QString  code; // назва/код майданчика (наприклад "NYSE", "XETRA", "LSE"...)
        QString  name; // людська назва біржі
    };

    static const std::vector <exchange::Meta>& metadata();
    static inline quint8 bits(Exchange e) { return static_cast<quint8>(e); }
}

namespace geo {
    QDataStream& operator << (QDataStream& out, const Exchange& tag)
    { return out << static_cast <uint8_t > (tag); }

    QDataStream& operator >> (QDataStream& in, Exchange& tag) {
        uint8_t  val;
        in >> val;
        tag = static_cast <Exchange>(val);
        return in;
    }
    QString operator~(Continent c) { return continent::to_string(c); }
}

QString operator & (geo::Exchange c) { return geo::exchange::sufix(c); }
QString operator - (geo::Exchange c) { return geo::exchange::code(c); }
QString operator ~ (geo::Exchange c) { return geo::exchange::name(c); }

bool geo::exchange::exist(Exchange e)
{
    if (e == Exchange::Unknown)
        return false;

    for (const auto& m : metadata())
        if (m.e == e)
            return true;

    return false;
}

QString geo::exchange::sufix(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.sufix;
    return "--";
}

QString geo::exchange::name(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.name;
    return "----";
}

QString geo::exchange::code(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.code;
    return "---";
}

geo::Currency geo::exchange::currency(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.currency;
    return Currency::None;
}

geo::Country geo::exchange::country(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.country;
    return Country::Unknown;
}

geo::Exchange geo::exchange::from_string(const QString& s)
{
    for (const auto& m : metadata())
        if (m.code.compare(s, Qt::CaseInsensitive) == 0 ||
            m.name.compare(s, Qt::CaseInsensitive) == 0)
            return m.e;
    for (const auto& m : metadata())
        if (m.sufix.compare(s, Qt::CaseInsensitive) == 0)
            return m.e;
    return Exchange::Unknown;
}

// ------------------------------------------------------------------------------------------------
// Набори бірж (добірки для фільтрації)
// ------------------------------------------------------------------------------------------------
std::vector <geo::Exchange> geo::exchange::major_europe_sufix()
{
    return { Exchange::PA, Exchange::AM, Exchange::XETRA, Exchange::BE, Exchange::LU };
}

std::vector <geo::Exchange> geo::exchange::minor_europe_sufix()
{
    return {
        Exchange::SW, Exchange::MC, Exchange::BR, Exchange::OL, Exchange::HE,
        Exchange::CO, Exchange::ST, Exchange::XSTO, Exchange::VI, Exchange::LS,
        Exchange::DUSEL, Exchange::HANOV, Exchange::MUNIC, Exchange::STUTG,
        Exchange::FRANK, Exchange::HUMBR, Exchange::PRAGA, Exchange::WARSH, Exchange::ATHENS
    };
}

std::vector <geo::Exchange> geo::exchange::other_worlds_sufix()
{
    return { Exchange::Taiwan, Exchange::Korea, Exchange::Australia,
             Exchange::Toronto, Exchange::NEO, Exchange::TSX, Exchange::TEL_AVIV };
}

std::vector <geo::Exchange> geo::exchange::us_sufix()
{
    return {
        Exchange::US, Exchange::NASDAQ, Exchange::NYSE, Exchange::NYSE_ARCA,
        Exchange::NYSE_MKT, Exchange::AMEX, Exchange::BATS,
        Exchange::PINK, Exchange::OTC_GREY, Exchange::OTC_CE, Exchange::OTC_BB,
        Exchange::OTC, Exchange::OTC_QB, Exchange::OTC_QX, Exchange::NMFQA,
        Exchange::OTC_MKTS, Exchange::OTC_MTKS
    };
}

std::vector <geo::Exchange> geo::exchange::all_exchange()
{
    std::vector <Exchange> ret;
    ret.reserve(int(metadata().size()));
    for (const auto& m : metadata())
        ret.push_back(m.e);
    return ret;
}

QStringList geo::exchange::all_exchange_short()
{
    QStringList ret;
    for (const auto& m : metadata())
        ret.push_back(m.name);
    ret.removeDuplicates();
    return ret;
}

// ------------------------------------------------------------------------------------------------
// Класифікатори (бітові маски 1:1 з твоїм Symbol::Exchange)
// ------------------------------------------------------------------------------------------------
namespace ex = geo::exchange;
bool ex::us       (Exchange e) { return exist(e) && (bits(e) & 0b1100'0000) == 0; }
bool ex::nyse     (Exchange e) { return exist(e) && (bits(e) & 0b1111'1000) == +Exchange::NYSE; }
bool ex::nasdaq   (Exchange e) { return e == Exchange::NASDAQ; }
bool ex::otc      (Exchange e) { return exist(e) && (bits(e) & 0b1111'0000) == +Exchange::OTC; }
bool ex::europe   (Exchange e) { return exist(e) && (bits(e) & 0b1000'0000) == +Exchange::EU; }
bool ex::trash    (Exchange e) { return exist(e) && (bits(e) & 0b1111'0000) == +Exchange::PINK; }
bool ex::euromajor(Exchange e) { return exist(e) && (bits(e) & 0b1100'0000) == +Exchange::EU1; }
bool ex::eurominor(Exchange e) { return exist(e) && (bits(e) & 0b1100'0000) == +Exchange::EU2; }
bool ex::asia     (Exchange e) { return exist(e) && (bits(e) & 0b1110'0000) == +Exchange::Asia; }
bool ex::world    (Exchange e) { return exist(e) && (bits(e) & 0b1100'0000) == +Exchange::World; }

static const std::vector <geo::exchange::Meta>& geo::exchange::metadata()
{
    static std::vector <exchange::Meta> _;
    if (not _.empty())
        return _;

    // --- Oceania & World (поза США/ЄС) ---
    _.emplace_back(Exchange::Australia, Currency::US_Dollar, Country::Australia,
                   "AU", "AU", "Australian Securities Exchange");

    _.emplace_back(Exchange::Toronto, Currency::Canadian_Dollar, Country::Canada,
                   "TO", "TO", "Toronto Stock Exchange");

    _.emplace_back(Exchange::NEO, Currency::Canadian_Dollar, Country::Canada,
                   "NEO",   "NEO",      "NEO Exchange");

    _.emplace_back(Exchange::TSX, Currency::Canadian_Dollar, Country::Canada,
                   "V", "V", "TSX Venture Exchange");

    _.emplace_back(Exchange::Taiwan, Currency::Taiwan_New_Dollar, Country::Taiwan,
                   "TW", "TW", "Taipei / Taiwan Stock Exchange");

    _.emplace_back(Exchange::Korea, Currency::Korean_Won, Country::SouthKorea,
                   "KO", "KO", "Korea Stock Exchange");

    _.emplace_back(Exchange::TEL_AVIV, Currency::Israeli_Shekel, Country::Israel,
                   "TA", "TA", "Tel Aviv Stock Exchange");

    // --- Європа (EU1 / EU2 / Scandinavia) ---
    _.emplace_back(Exchange::AM, Currency::Euro, Country::Netherlands,
                   "AS", "AS", "Euronext Amsterdam");

    _.emplace_back(Exchange::BE, Currency::Euro, Country::Germany,
                   "BE", "BE", "Börse Berlin");

    _.emplace_back(Exchange::XETRA, Currency::Euro, Country::Germany,
                   "XETRA", "XETRA", "Deutsche Börse");

    _.emplace_back(Exchange::BR, Currency::Euro, Country::Belgium,
                   "BR", "BR", "Euronext Brussels");

    _.emplace_back(Exchange::LSE, Currency::GreatBritain_Pence, Country::UnitedKingdom,
                   "L", "LSE", "London Stock Exchange");

    _.emplace_back(Exchange::CO, Currency::Danish_Krone, Country::Denmark,
                   "CO", "CO", "Copenhagen Stock Exchange");

    _.emplace_back(Exchange::MC, Currency::Euro, Country::Spain,
                   "MC", "MC", "Bolsa de Madrid");

    _.emplace_back(Exchange::HE, Currency::Euro, Country::Finland,
                   "FI", "HE", "Helsinki");

    _.emplace_back(Exchange::OL, Currency::Norwegian_Krone, Country::Norway,
                   "OL", "OL", "Oslo Stock Exchange");

    _.emplace_back(Exchange::PA, Currency::Euro, Country::France,
                   "PA", "PA", "Euronext Paris");

    _.emplace_back(Exchange::ST, Currency::Swedish_Krona, Country::Sweden,
                   "ST", "ST", "Stockholm Stock Exchange");

    _.emplace_back(Exchange::XSTO, Currency::Swedish_Krona, Country::Sweden,
                   "ST", "XSTO", "Nasdaq Stockholm (XSTO)");

    _.emplace_back(Exchange::SW, Currency::Swiss_Franc, Country::Switzerland,
                   "SW", "SW", "SIX Swiss Exchange");

    _.emplace_back(Exchange::LU, Currency::Euro, Country::Luxembourg,
                   "LU", "LU", "Luxembourg Stock Exchange");

    _.emplace_back(Exchange::HUMBR, Currency::Euro, Country::Germany,
                   "HM", "HM", "Hamburg Exchange");

    _.emplace_back(Exchange::DUSEL, Currency::Euro, Country::Germany,
                   "DU", "DU", "Dusseldorf Exchange");

    _.emplace_back(Exchange::HANOV, Currency::Euro, Country::Germany,
                   "HA", "HA", "Hanover Exchange");

    _.emplace_back(Exchange::MUNIC, Currency::Euro, Country::Germany,
                   "MU", "MU", "Munich Exchange");

    _.emplace_back(Exchange::STUTG, Currency::Euro, Country::Germany,
                   "STU", "STU", "Stuttgart Exchange");

    _.emplace_back(Exchange::FRANK, Currency::Euro, Country::Germany,
                   "F", "F", "Frankfurt Exchange");

    _.emplace_back(Exchange::VI, Currency::Euro, Country::Austria,
                   "VI", "VI", "Vienna Exchange");

    _.emplace_back(Exchange::LS, Currency::Euro, Country::Portugal,
                   "LS", "LS", "Euronext Lisbon");

    _.emplace_back(Exchange::PRAGA, Currency::Czech_Koruna, Country::Czechia,
                   "PR", "PR", "Prague Stock Exchange");

    _.emplace_back(Exchange::WARSH, Currency::Polish_Zloty, Country::Poland,
                   "WAR", "WAR", "Warsaw Stock Exchange");

    _.emplace_back(Exchange::ATHENS, Currency::Euro, Country::Greece,
                   "AT", "AT", "Athens Exchange");

    // --- США / OTC ---
    _.emplace_back(Exchange::US, Currency::US_Dollar, Country::UnitedStates,
                   "US", "US", "");

    _.emplace_back(Exchange::NYSE, Currency::US_Dollar, Country::UnitedStates,
                   "US", "NYSE", "New York Stock Exchange");

    _.emplace_back(Exchange::NYSE_ARCA, Currency::US_Dollar, Country::UnitedStates,
                   "US", "NYSE ARCA", "NYSE Arca (ETFs, ETPs)");

    _.emplace_back(Exchange::NASDAQ, Currency::US_Dollar, Country::UnitedStates,
                   "US", "NASDAQ", "NASDAQ");

    _.emplace_back(Exchange::BATS, Currency::US_Dollar, Country::UnitedStates,
                   "US", "BATS", "BATS (ECN)");

    _.emplace_back(Exchange::AMEX, Currency::US_Dollar, Country::UnitedStates,
                   "US", "AMEX", "American Stock Exchange");

    _.emplace_back(Exchange::NYSE_MKT, Currency::US_Dollar, Country::UnitedStates,
                   "US", "NYSE MKT", "NYSE American (ex-AMEX)");

    _.emplace_back(Exchange::OTC, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTC", "OTC Markets (umbrella)");

    _.emplace_back(Exchange::OTC_QB, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTCQB", "OTC QB");

    _.emplace_back(Exchange::OTC_QX, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTCQX", "OTC QX");

    _.emplace_back(Exchange::NMFQA, Currency::US_Dollar, Country::UnitedStates,
                   "US", "NMFQS", "OTC tier (legacy code)");

    _.emplace_back(Exchange::OTC_MKTS, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTCMKTS", "OTC Markets");

    _.emplace_back(Exchange::OTC_MTKS, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTCMTKS", "OTC Markets (alt name)");

    _.emplace_back(Exchange::PINK, Currency::US_Dollar, Country::UnitedStates,
                   "US", "PINK", "OTC Pink");

    _.emplace_back(Exchange::OTC_GREY, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTCGREY", "OTC Grey");

    _.emplace_back(Exchange::OTC_BB, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTCBB", "OTC Bulletin Board (defunct)");

    _.emplace_back(Exchange::OTC_CE, Currency::US_Dollar, Country::UnitedStates,
                   "US", "OTCCE", "OTC Caveat Emptor");

    return _;
}
