// exchange.cpp
#include "geo/geo.h"
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDataStream>
#include <QtCore/QList>
#include <QtCore/QDebug>

namespace sdk::exchange {
    struct Meta {
        Exchange e;
        Currency currency; // валюта торгів
        Country  country;  // місце розміщення біржі
        QString  sufix;    // короткий суфікс, що йде після крапки в тікері (AAPL.US → "US")
        QString  venue; // назва/код майданчика (наприклад "NYSE", "XETRA", "LSE"...)
        QString  name; // людська назва біржі
    };

    const std::vector <exchange::Meta>& metadata();
    inline quint8 bits(Exchange e) { return static_cast<quint8>(e); }
}

namespace sdk {
    QDataStream& operator << (QDataStream& out, const Exchange& tag)
    { return out << static_cast <uint8_t > (tag); }

    QDataStream& operator >> (QDataStream& in, Exchange& tag) {
        uint8_t  val;
        in >> val;
        tag = static_cast <Exchange>(val);
        return in;
    }
}

QString sdk::operator & (sdk::Exchange c) { return sdk::exchange::sufix(c); }
QString sdk::operator - (sdk::Exchange c) { return sdk::exchange::venue(c); }
QString sdk::operator ~ (sdk::Exchange c) { return sdk::exchange::name(c); }

bool sdk::exchange::exist(Exchange e)
{
    if (e == Exchange::Unknown)
        return false;

    for (const auto& m : metadata())
        if (m.e == e)
            return true;

    return false;
}

bool sdk::exchange::exist(Country c)
{
    for (const auto& m : metadata())
        if (m.country == c)
            return true;

    return false;
}

QString sdk::exchange::sufix(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.sufix;
    return "--";
}

QString sdk::exchange::venue(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.venue;
    return "----";
}

QString sdk::exchange::name(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.name;
    return "---";
}

sdk::Currency sdk::exchange::currency(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.currency;
    return Currency::None;
}

sdk::Country sdk::exchange::country(Exchange e)
{
    if (exist(e))
        for (const auto& m : metadata())
            if (m.e == e)
                return m.country;
    return Country::Unknown;
}

sdk::Exchange sdk::exchange::from_string(const QString& s)
{
    if (Exchange ret = from_venue_string(s); ret != Exchange::Unknown)
        return ret;

    for (const auto& m : metadata())
        if (m.name .compare(s, Qt::CaseInsensitive) == 0)
            return m.e;

    for (const auto& m : metadata())
        if (m.sufix.compare(s, Qt::CaseInsensitive) == 0)
            return m.e;

    return Exchange::Unknown;
}

sdk::Exchange sdk::exchange::from_venue_string(const QString& s)
{
    // qDebug() << Q_FUNC_INFO << s;
    for (const auto& m : metadata()){
        // qDebug() << m.venue << m.sufix << m.name;
        if (m.venue.compare(s, Qt::CaseInsensitive) == 0)
            return m.e;
    }
    return Exchange::Unknown;
}

// ------------------------------------------------------------------------------------------------
// Набори бірж (добірки для фільтрації)
// ------------------------------------------------------------------------------------------------
std::vector <sdk::Exchange> sdk::exchange::major_europe_sufix()
{
    // Західна Європа + головні німецькі/британські майданчики
    return {
        Exchange::Paris,
        Exchange::Amsterdam,
        Exchange::Luxemburg,
        Exchange::XETRA,
        Exchange::LSE,
        // (за твоєю попередньою логікою Berlin був у major — лишаю)
        Exchange::Berlin
    };
}
bool sdk::exchange::is_major_europe_sufix(sdk::Exchange e)
{
    for (const auto& it : major_europe_sufix())
        if (it == e)
            return true;
    return false;
}

std::vector <sdk::Exchange> sdk::exchange::minor_europe_sufix()
{
    // Інші західні, Скандинавія, регіональні DE, східна Європа
    return {
        // West EU (інші)
        Exchange::Swiss, Exchange::Madrid, Exchange::Brussels,
        Exchange::Vienna, Exchange::Lisbon, Exchange::Irish,
        Exchange::UK, Exchange::LondonIL,

        // Scandinavia
        Exchange::Oslo, Exchange::Helsinki, Exchange::Copenhagen,
        Exchange::Iceland, Exchange::Stockholm, Exchange::XSTO,

        // Germany regionals
        Exchange::Dusseldorf, Exchange::Hanover, Exchange::Munich,
        Exchange::Stuttgart,  Exchange::Frankfurt, Exchange::Humburg,

        // East EU
        Exchange::Prague, Exchange::Warsaw, Exchange::Athens,
        Exchange::Budapest, Exchange::Zagreb, Exchange::Bucharest
    };
}
bool sdk::exchange::is_minor_europe_sufix(sdk::Exchange e)
{
    for (const auto& it : minor_europe_sufix())
        if (it == e)
            return true;
    return false;
}

std::vector <sdk::Exchange> sdk::exchange::other_worlds_sufix()
{
    // Все поза США/ЄС: Америка (ex-US), Азія, Африка, Океанія, глобальні
    return {
        // America (ex-US)
        Exchange::Toronto, Exchange::NEO, Exchange::TSX,
        Exchange::Chilean, Exchange::BuenosAires, Exchange::SaoPaulo,
        Exchange::Mexico,  Exchange::Lima,

        // Asia
        Exchange::Taiwan, Exchange::TaiwanOTC, Exchange::Korea, Exchange::KOSDAQ,
        Exchange::Tel_aviv, Exchange::Shenzhen,  Exchange::Vietnam,   Exchange::Philippine,
        Exchange::Shanghai, Exchange::Jakarta,   Exchange::India,     Exchange::Karachi,
        Exchange::Thailand, Exchange::Colombo,   Exchange::Kuala,

        // Africa
        Exchange::VictoriaFalls, Exchange::Zimbabwe,    Exchange::Lusaka,
        Exchange::Uganda,        Exchange::Dar_es_Salaam, Exchange::Rwanda,
        Exchange::Botswana,      Exchange::Nigerian,    Exchange::Egyptian,
        Exchange::Malawi,        Exchange::Ghana,       Exchange::Nairobi,
        Exchange::Casablanca,    Exchange::Mauritius,   Exchange::Johannesburg,

        // Oceania
        Exchange::Australia,

        // Global / “world”
        Exchange::Global, Exchange::EuropeFund, Exchange::GovernmentBonds,
        Exchange::MoneyMarket, Exchange::Forex
    };
}
bool sdk::exchange::is_other_worlds_sufix(sdk::Exchange e)
{
    for (const auto& it : other_worlds_sufix())
        if (it == e)
            return true;
    return false;
}

std::vector <sdk::Exchange> sdk::exchange::us_sufix()
{
    // США: основні + OTC/PINK
    return {
        Exchange::US,
        Exchange::NASDAQ, Exchange::NYSE, Exchange::NYSE_ARCA,
        Exchange::NYSE_MKT, Exchange::AMEX, Exchange::BATS,

        // OTC / Pink
        Exchange::PINK, Exchange::OTC_GREY, Exchange::OTC_CE, Exchange::OTC_BB,
        Exchange::OTC,  Exchange::OTC_QB,   Exchange::OTC_QX, Exchange::NMFQA,
        Exchange::OTC_MKTS, Exchange::OTC_MTKS
    };
}

std::vector <sdk::Exchange> sdk::exchange::all_exchange()
{
    std::vector <Exchange> ret;
    ret.reserve(int(metadata().size()));
    for (const auto& m : metadata())
        ret.push_back(m.e);
    return ret;
}

QStringList sdk::exchange::all_exchange_venue()
{
    QStringList ret;
    for (const auto& m : metadata())
        ret.push_back(m.venue);
    ret.removeDuplicates();
    return ret;
}

// ------------------------------------------------------------------------------------------------
// Класифікатори (бітові маски 1:1 з твоїм Symbol::Exchange)
// ------------------------------------------------------------------------------------------------
namespace ex = sdk::exchange;
bool ex::us       (Exchange e){ return exist(e) && (bits(e) & 0b1110'0000) == +Exchange::US; }
bool ex::nyse     (Exchange e){ return exist(e) && (bits(e) & 0b1111'1000) == +Exchange::NYSE; }
bool ex::nasdaq   (Exchange e){ return e == Exchange::NASDAQ; }
bool ex::otc      (Exchange e){ return exist(e) && (bits(e) & 0b1111'1000) == +Exchange::OTC; }
bool ex::europe   (Exchange e){ return exist(e) && (bits(e) & 0b1100'0000) == +Exchange::EU; }
bool ex::trash    (Exchange e){ return exist(e) && (bits(e) & 0b1111'1000) == +Exchange::PINK; }
bool ex::euromajor(Exchange e){ return exist(e) && is_major_europe_sufix(e); }
bool ex::eurominor(Exchange e){ return exist(e) && is_minor_europe_sufix(e); }
bool ex::global   (Exchange e){ return exist(e) && (bits(e) & 0b1111'0000) == +Exchange::Global; }
bool ex::asia     (Exchange e){ return exist(e) && (bits(e) & 0b1110'0000) == +Exchange::Asia; }
bool ex::africa   (Exchange e){ return exist(e) && (bits(e) & 0b1110'0000) == +Exchange::Africa; }
bool ex::america  (Exchange e){ return exist(e) && (bits(e) & 0b1110'0000) == +Exchange::America; }
bool ex::oceania  (Exchange e){ return exist(e) && (bits(e) & 0b1111'0000) == +Exchange::Oceania; }

bool ex::world(Exchange e) {
    return exist(e) && (asia(e) || africa(e) || america(e) || oceania(e));
}

const std::vector <sdk::exchange::Meta>& sdk::exchange::metadata()
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

    _.emplace_back(Exchange::Taiwan, Currency::Taiwan_New_Dollar, Country::Taiwan,
                   "TWO", "TWO", "Taiwan OTC Exchange");

    _.emplace_back(Exchange::Korea, Currency::Korean_Won, Country::SouthKorea,
                   "KQ", "KQ", "KOSDAQ");

    _.emplace_back(Exchange::KOSDAQ, Currency::Korean_Won, Country::SouthKorea,
                   "KO", "KO", "Korea Stock Exchange");

    _.emplace_back(Exchange::Philippine, Currency::Philippine_Piso, Country::Philippines,
                   "PSE", "PSE", "Philippine Stock Exchange");

    _.emplace_back(Exchange::Shanghai, Currency::Chinese_Yuan, Country::China,
                   "SHG", "SHG", "Shanghai Stock Exchange");

    _.emplace_back(Exchange::Jakarta, Currency::Indonesian_Rupiah, Country::Indonesia,
                   "JK", "JK", "Jakarta Exchange");

    _.emplace_back(Exchange::India, Currency::Indian_Rupee, Country::India,
                   "NSE", "NSE", "National Stock Exchange of India");

    _.emplace_back(Exchange::Karachi, Currency::Pakistani_Rupee, Country::Pakistan,
                   "KAR", "KAR", "Karachi Stock Exchange");

    _.emplace_back(Exchange::Thailand, Currency::Thai_Baht, Country::Thailand,
                   "BK", "BK", "Thailand Exchange");

    _.emplace_back(Exchange::Colombo, Currency::Sri_Lankan_Rupee, Country::Shrilanka,
                   "CM", "CM", "Colombo Stock Exchange");

    _.emplace_back(Exchange::Kuala, Currency::Malaysian_Ringgit, Country::Malaysia,
                   "KLSE", "KLSE", "Kuala Lumpur Exchange");

    _.emplace_back(Exchange::Tel_aviv, Currency::Israeli_Shekel, Country::Israel,
                   "TA", "TA", "Tel Aviv Stock Exchange");

    _.emplace_back(Exchange::Shenzhen, Currency::Chinese_Yuan, Country::China,
                   "SHE", "SHE", "Shenzhen Stock Exchange");

    _.emplace_back(Exchange::Vietnam, Currency::Vietnamese_Dong, Country::Vietnam,
                   "VN", "VN", "Vietnam Exchange");

    _.emplace_back(Exchange::VictoriaFalls, Currency::Zimbabwean_Dollar, Country::Zimbabwe,
                   "VFEX", "VFEX", "Victoria Falls Stock Exchange");

    _.emplace_back(Exchange::Zimbabwe, Currency::Zimbabwean_Dollar, Country::Zimbabwe,
                   "XZIM", "XZIM", "Zimbabwe Stock Exchange");

    _.emplace_back(Exchange::Lusaka, Currency::Zambian_Kwacha, Country::Zambia,
                   "LUSE", "LUSE", "Lusaka Stock Exchange");

    _.emplace_back(Exchange::Uganda, Currency::Ugandan_Shilling, Country::Uganda,
                   "USE", "USE", "Uganda Securities Exchange");

    _.emplace_back(Exchange::Dar_es_Salaam, Currency::Tanzanian_Shilling, Country::Tanzania,
                   "DSE", "DSE", "Dar es Salaam Stock Exchange");

    _.emplace_back(Exchange::Rwanda, Currency::Rwandan_Franc, Country::Rwanda,
                   "RSE", "RSE", "Rwanda Stock Exchange");

    _.emplace_back(Exchange::Botswana, Currency::Botswana_Pula, Country::Botswana,
                   "XBOT", "XBOT", "Botswana Stock Exchange");

    _.emplace_back(Exchange::Nigerian, Currency::Nigerian_Naira, Country::Nigeria,
                   "XNSA", "XNSA", "Nigerian Stock Exchange");

    _.emplace_back(Exchange::Egyptian, Currency::Egyptian_Pound, Country::Egypt,
                   "EGX", "EGX", "Egyptian Exchange");

    _.emplace_back(Exchange::Malawi, Currency::Malawian_Kwacha, Country::Malawi,
                   "MSE", "MSE", "Malawi Stock Exchange");

    _.emplace_back(Exchange::Ghana, Currency::Ghanaian_Cedi, Country::Ghana,
                   "GSE", "GSE", "Ghana Stock Exchange");

    _.emplace_back(Exchange::Nairobi, Currency::Kenyan_Shilling, Country::Kenya,
                   "XNAI", "XNAI", "Nairobi Securities Exchange");

    _.emplace_back(Exchange::Johannesburg, Currency::South_African_Rand, Country::SouthAfrica,
                   "JSE", "JSE", "Johannesburg Exchange");

    _.emplace_back(Exchange::Casablanca, Currency::Moroccan_Dirham, Country::Morocco,
                   "BC", "BC", "Casablanca Stock Exchange");

    _.emplace_back(Exchange::Mauritius, Currency::Mauritian_Rupee, Country::Mauritius,
                   "SEM", "SEM", "Stock Exchange of Mauritius");

    _.emplace_back(Exchange::Chilean, Currency::Chilean_Peso, Country::Chile,
                   "SN", "SN", "Chilean Stock Exchange");

    _.emplace_back(Exchange::BuenosAires, Currency::Argentine_Peso, Country::Argentina,
                   "BA", "BA", "Buenos Aires Exchange");

    _.emplace_back(Exchange::SaoPaulo, Currency::Brazilian_Real, Country::Brazil,
                   "SA", "SA", "Sao Paulo Exchange");

    _.emplace_back(Exchange::Mexico, Currency::Mexican_Peso, Country::Mexico,
                   "MX", "MX", "Mexican Exchange");

    _.emplace_back(Exchange::Lima, Currency::Peruvian_Sol, Country::Peru,
                   "LIM", "LIM", "Bolsa de Valores de Lima");

    // --- Європа (EU1 / EU2 / Scandinavia) ---
    _.emplace_back(Exchange::Amsterdam, Currency::Euro, Country::Netherlands,
                   "AS", "AS", "Euronext Amsterdam");

    _.emplace_back(Exchange::Berlin, Currency::Euro, Country::Germany,
                   "BE", "BE", "Börse Berlin");

    _.emplace_back(Exchange::XETRA, Currency::Euro, Country::Germany,
                   "XETRA", "XETRA", "Deutsche Börse");

    _.emplace_back(Exchange::Brussels, Currency::Euro, Country::Belgium,
                   "BR", "BR", "Euronext Brussels");

    _.emplace_back(Exchange::LSE, Currency::GreatBritain_Pence, Country::UnitedKingdom,
                   "L", "LSE", "London Stock Exchange");

    _.emplace_back(Exchange::Copenhagen, Currency::Danish_Krone, Country::Denmark,
                   "CO", "CO", "Copenhagen Stock Exchange");

    _.emplace_back(Exchange::Madrid, Currency::Euro, Country::Spain,
                   "MC", "MC", "Bolsa de Madrid");

    _.emplace_back(Exchange::Helsinki, Currency::Euro, Country::Finland,
                   "FI", "HE", "Helsinki");

    _.emplace_back(Exchange::Oslo, Currency::Norwegian_Krone, Country::Norway,
                   "OL", "OL", "Oslo Stock Exchange");

    _.emplace_back(Exchange::Paris, Currency::Euro, Country::France,
                   "PA", "PA", "Euronext Paris");

    _.emplace_back(Exchange::Stockholm, Currency::Swedish_Krona, Country::Sweden,
                   "ST", "ST", "Stockholm Stock Exchange");

    _.emplace_back(Exchange::XSTO, Currency::Swedish_Krona, Country::Sweden,
                   "ST", "XSTO", "Nasdaq Stockholm (XSTO)");

    _.emplace_back(Exchange::Swiss, Currency::Swiss_Franc, Country::Switzerland,
                   "SW", "SW", "SIX Swiss Exchange");

    _.emplace_back(Exchange::Luxemburg, Currency::Euro, Country::Luxembourg,
                   "LU", "LU", "Luxembourg Stock Exchange");

    _.emplace_back(Exchange::Humburg, Currency::Euro, Country::Germany,
                   "HM", "HM", "Hamburg Exchange");

    _.emplace_back(Exchange::Dusseldorf, Currency::Euro, Country::Germany,
                   "DU", "DU", "Dusseldorf Exchange");

    _.emplace_back(Exchange::Hanover, Currency::Euro, Country::Germany,
                   "HA", "HA", "Hanover Exchange");

    _.emplace_back(Exchange::Munich, Currency::Euro, Country::Germany,
                   "MU", "MU", "Munich Exchange");

    _.emplace_back(Exchange::Stuttgart, Currency::Euro, Country::Germany,
                   "STU", "STU", "Stuttgart Exchange");

    _.emplace_back(Exchange::Frankfurt, Currency::Euro, Country::Germany,
                   "F", "F", "Frankfurt Exchange");

    _.emplace_back(Exchange::Vienna, Currency::Euro, Country::Austria,
                   "VI", "VI", "Vienna Exchange");

    _.emplace_back(Exchange::Lisbon, Currency::Euro, Country::Portugal,
                   "LS", "LS", "Euronext Lisbon");

    _.emplace_back(Exchange::Prague, Currency::Czech_Koruna, Country::Czechia,
                   "PR", "PR", "Prague Stock Exchange");

    _.emplace_back(Exchange::Warsaw, Currency::Polish_Zloty, Country::Poland,
                   "WAR", "WAR", "Warsaw Stock Exchange");

    _.emplace_back(Exchange::Athens, Currency::Euro, Country::Greece,
                   "AT", "AT", "Athens Exchange");

    _.emplace_back(Exchange::Irish, Currency::Euro, Country::Ireland,
                   "IR", "IR", "Irish Exchange");

    _.emplace_back(Exchange::Iceland, Currency::Icelandic_Krona, Country::Iceland,
                   "IC", "IC", "Iceland Exchange");

    _.emplace_back(Exchange::Budapest, Currency::Hungarian_Forint, Country::Hungary,
                   "BUD", "BUD", "Budapest Stock Exchange");

    _.emplace_back(Exchange::Zagreb, Currency::Euro, Country::Croatia,
                   "ZSE", "ZSE", "Zagreb Stock Exchange");

    _.emplace_back(Exchange::Bucharest, Currency::Romanian_Leu, Country::Romania,
                   "RO", "RO", "Bucharest Stock Exchange");

    _.emplace_back(Exchange::EuropeFund, Currency::Euro, Country::Unknown,
                   "EUFUND", "REUFUNDO", "Europe Fund Virtual Exchange");

    _.emplace_back(Exchange::LondonIL, Currency::US_Dollar, Country::UnitedKingdom,
                   "IL", "IL", "London IL");

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

    _.emplace_back(Exchange::GovernmentBonds, Currency::US_Dollar, Country::Unknown,
                   "GBOND", "GBOND", "Government Bonds");

    _.emplace_back(Exchange::MoneyMarket, Currency::US_Dollar, Country::Unknown,
                   "MONEY", "MONEY", "Money Market Virtual Exchange");

    _.emplace_back(Exchange::Forex, Currency::None, Country::Unknown,
                   "FOREX", "FOREX", "FOREX");

    return _;
}
