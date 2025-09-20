#ifndef SDK_ENUM_H
#define SDK_ENUM_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QDataStream>
#include <cstdint>

namespace sdk {
// ------------------------------------------------------------------------------------------------
// --------------------------- Date section -------------------------------------------------------
// ------------------------------------------------------------------------------------------------
enum class Quartel : uint8_t {
    First, Second, Third, Fourth,
    Annual
};
constexpr uint8_t operator + (Quartel m) { return static_cast <uint8_t>(m); }
namespace quartel {
    QString to_string(Quartel m);
    Quartel from_string(const QString& s);
    Quartel from_month(int month);

    QStringList all_names();
    std::vector <Quartel> all();
}
QString operator ~ (Quartel m);

QDataStream& operator << (QDataStream& out, const Quartel& m);
QDataStream& operator >> (QDataStream& in,        Quartel& m);



enum class Month : uint8_t {
    Unknown = 0,
    January, February, March,
    April,   May,      June,
    July,    August,   September,
    October, November, December
};
constexpr uint8_t operator + (Month m) { return static_cast <uint8_t>(m); }
namespace month {
    uint8_t to_int   (Month m);
    QString to_string(Month m);
    Month from_string(const QString& s);

    QStringList all_names();
    std::vector <Month> all();
}
QString operator ~ (Month m);

QDataStream& operator << (QDataStream& out, const Month& m);
QDataStream& operator >> (QDataStream& in,        Month& m);
// ================================================================================================




enum class HomeCategory : uint8_t {
    Unknown,
    Domestic,
    Foreign
};
constexpr uint8_t operator + (HomeCategory m) { return static_cast <uint8_t>(m); }
namespace home_category {
    QString        to_string(HomeCategory m);
    HomeCategory from_string(const QString& s);

    QStringList all_names();
    std::vector <HomeCategory> all();
}
QString operator ~ (HomeCategory m);

QDataStream& operator << (QDataStream& out, const Month& m);
QDataStream& operator >> (QDataStream& in,        Month& m);
// ---------------------------                  ---------------------------------------------------
// =========================== ^ Date section ^ ===================================================
// ================================================================================================



// ------------------------------------------------------------------------------------------------
// --------------------------- Geography section --------------------------------------------------
// ------------------------------------------------------------------------------------------------
enum class Continent : uint16_t {
    Europe = 0xC400,
    Africa = 0xC500,
    North_America = 0xC600,
    South_America = 0xC700,
    Oceania = 0xC800,
    Asia = 0xC900,   // historical grouping used in your project

    None = 0xC9FF
};
constexpr uint16_t operator + (Continent c) { return static_cast <uint16_t>(c); }
namespace continent {
    QString     to_string(Continent c);
    Continent   from_string(const QString& s);

    QStringList all_names();
    std::vector <Continent> all();
}
QString operator ~ (Continent c);

QDataStream& operator << (QDataStream& out, const Continent& c);
QDataStream& operator >> (QDataStream& in,        Continent& c);

enum class Region : uint16_t {
    // Europe
    WesternEurope = +Continent::Europe | 0b0100'0000, // 01.. ....
    CentralEurope =        WesternEurope | 0b00'0000, // 0100 ....
    BritishIsles  =        WesternEurope | 0b01'0000, // 0101 ....
    Benelux       =        WesternEurope | 0b10'0000, // 0110 ....
    Scandinavia   =        WesternEurope | 0b11'0000, // 0111 ....

    SouthernEurope = +Continent::Europe | 0b1000'0000, // 10.. ....
    ItaloAdriatic  =       SouthernEurope | 0b01'0000, // 1001 ....
    Balkans        =       SouthernEurope | 0b10'0000, // 1010 ....
    Iberia         =       SouthernEurope | 0b11'0000, // 1011 ....

    EasternEurope = +Continent::Europe | 0b1100'0000, // 11.. ....
    EuropeanBlackSea =     EasternEurope | 0b01'0000, // 1101 ....
    Visegrad         =     EasternEurope | 0b10'0000, // 1110 ....
    BalticStates     =     EasternEurope | 0b11'0000, // 1111 ....

    // Asia
    WesternAsia = +Continent::Asia | 0b0100'0000, // 01.. ....
    Arabia             = WesternAsia | 0b01'0000, // 0101 ....
    Caucasus           = WesternAsia | 0b10'0000, // 0110 ....
    MiddleEast         = WesternAsia | 0b11'0000, // 0111 ....

    SouthernAsia = +Continent::Asia | 0b1000'0000, // 10.. ....
    CentralAsia        = SouthernAsia | 0b01'0000, // 1001 ....
    Hindustan          = SouthernAsia | 0b10'0000, // 1010 ....
    GreaterKhorasan    = SouthernAsia | 0b11'0000, // 1011 ....

    EasternAsia = +Continent::Asia | 0b1100'0000, // 11.. ....
    SinoJapanese       = EasternAsia | 0b01'0000, // 1101 ....
    IndoChina          = EasternAsia | 0b10'0000, // 1110 ....
    MalayArchipelago   = EasternAsia | 0b11'0000, // 1111 ....

    // America
    AngloAmerica   = +Continent::North_America | 0b0100'0000,
    CentralAmerica = +Continent::North_America | 0b1000'0000,
    Caribbean      = +Continent::North_America | 0b1100'0000,
    SouthAmerica   = +Continent::South_America,

    // Africa
    NorthAfrica = +Continent::Africa | 0b0000'0000, // 0... ....
    Maghreb             = NorthAfrica | 0b010'0000, // 001. ....
    NileValley          = NorthAfrica | 0b100'0000, // 010. ....

    SubSaharan  = +Continent::Africa | 0b1000'0000, // 1... ....
    Sahel                = SubSaharan | 0b001'0000, // 1001 ....
    GuineaCoast          = SubSaharan | 0b010'0000, // 1010 ....
    CongoBasin           = SubSaharan | 0b011'0000, // 1011 ....
    HornOfAfrica         = SubSaharan | 0b100'0000, // 1100 ....
    GreatLakes           = SubSaharan | 0b101'0000, // 1101 ....
    SouthernAfrica       = SubSaharan | 0b110'0000, // 1110 ....
    IndianOceanAfrica    = SubSaharan | 0b111'0000, // 1111 ....

    // Oceania
    AustralianAndNewZealand = +Continent::Oceania | 0b0010'0000, // 001. ....
    Melanesia               = +Continent::Oceania | 0b0100'0000, // 010. ....
    Micronesia              = +Continent::Oceania | 0b0110'0000, // 011. ....
    Polynesia               = +Continent::Oceania | 0b1000'0000, // 100. ....

    Unknown = +Continent::None
};
constexpr uint16_t operator + (Region s) { return static_cast <uint16_t>(s); }
namespace region {
    QString   to_string(Region r);
    Region  from_string(const QString& s);

    QStringList    all_names(Continent c = Continent::None);
    std::vector <Region> all(Continent c = Continent::None);
}
QString operator ~ (Region c);

QDataStream& operator << (QDataStream& out, const Region& r);
QDataStream& operator >> (QDataStream& in,        Region& r);


enum class Country : uint16_t {
    // Europe
    Ukraine  = +Region::EuropeanBlackSea | 0b0001,
    Moldova  = +Region::EuropeanBlackSea | 0b0010,
    Romania  = +Region::EuropeanBlackSea | 0b0011,
    Bulgaria = +Region::EuropeanBlackSea | 0b0100,

    Poland   = +Region::Visegrad | 0b0001,
    Czechia  = +Region::Visegrad | 0b0010,
    Slovakia = +Region::Visegrad | 0b0011,
    Hungary  = +Region::Visegrad | 0b0100,

    Estonia   = +Region::BalticStates | 0b0001,
    Latvia    = +Region::BalticStates | 0b0010,
    Lithuania = +Region::BalticStates | 0b0011,
    Belarus   = +Region::BalticStates | 0b0100,

    Androra   = +Region::Iberia | 0b0001,
    Portugal  = +Region::Iberia | 0b0010,
    Spain     = +Region::Iberia | 0b0011,
    Gibraltar = +Region::Iberia | 0b0100,

    Malta    = +Region::SouthernEurope | 0b0001,
    Cyprus   = +Region::SouthernEurope | 0b0010,

    Greece               = +Region::Balkans | 0b0001,
    Albania              = +Region::Balkans | 0b0010,
    NorthMacedonia       = +Region::Balkans | 0b0011,
    Serbia               = +Region::Balkans | 0b0100,
    BosniaAndHerzegovina = +Region::Balkans | 0b0101,
    Montenegro           = +Region::Balkans | 0b0110,
    Kosovo               = +Region::Balkans | 0b0111,

    Italy     = +Region::ItaloAdriatic | 0b0001,
    Croatia   = +Region::ItaloAdriatic | 0b0010,
    Slovenia  = +Region::ItaloAdriatic | 0b0011,
    SanMarino = +Region::ItaloAdriatic | 0b0100,
    Monaco    = +Region::ItaloAdriatic | 0b0101,
    Vatican   = +Region::ItaloAdriatic | 0b0110,

    Sweden    = +Region::Scandinavia | 0b0001,
    Norway    = +Region::Scandinavia | 0b0010,
    Finland   = +Region::Scandinavia | 0b0011,
    Iceland   = +Region::Scandinavia | 0b0100,
    Denmark   = +Region::Scandinavia | 0b0101,
    Greenland = +Region::Scandinavia | 0b0110,

    Belgium     = +Region::Benelux | 0b0001,
    Netherlands = +Region::Benelux | 0b0010,
    Luxembourg  = +Region::Benelux | 0b0011,

    UnitedKingdom = +Region::BritishIsles | 0b0001,
    IsleOfMan     = +Region::BritishIsles | 0b0010,
    Jersey        = +Region::BritishIsles | 0b0011,
    Guernsey      = +Region::BritishIsles | 0b0100,
    Ireland       = +Region::BritishIsles | 0b0101,

    France      = +Region::CentralEurope | 0b0001,
    Germany     = +Region::CentralEurope | 0b0010,
    Switzerland = +Region::CentralEurope | 0b0011,
    Austria     = +Region::CentralEurope | 0b0100,

    // America
    UnitedStates  = +Region::AngloAmerica | 0b0001,
    Canada        = +Region::AngloAmerica | 0b0010,

    Mexico     = +Region::CentralAmerica | 0b0001,
    Panama     = +Region::CentralAmerica | 0b0010,
    Nicaragua  = +Region::CentralAmerica | 0b0011,
    Honduras   = +Region::CentralAmerica | 0b0100,
    Guatemala  = +Region::CentralAmerica | 0b0101,
    ElSalvador = +Region::CentralAmerica | 0b0110,
    Belize     = +Region::CentralAmerica | 0b0111,

    Cuba              = +Region::Caribbean | 0b0001,
    CostaRica         = +Region::Caribbean | 0b0010,
    DominicanRepublic = +Region::Caribbean | 0b0011,
    PuertoRico        = +Region::Caribbean | 0b0100,
    Jamaica           = +Region::Caribbean | 0b0101,
    Bahamas           = +Region::Caribbean | 0b0110,
    CaymanIslands     = +Region::Caribbean | 0b0111,
    Bermuda           = +Region::Caribbean | 0b1000,
    Haiti             = +Region::Caribbean | 0b1001,
    Barbados          = +Region::Caribbean | 0b1010,
    Grenada           = +Region::Caribbean | 0b1011,
    VirginIslands     = +Region::Caribbean | 0b1100,

    Brazil    = +Region::SouthAmerica | 0b0001,
    Argentina = +Region::SouthAmerica | 0b0010,
    Chile     = +Region::SouthAmerica | 0b0011,
    Colombia  = +Region::SouthAmerica | 0b0100,
    Peru      = +Region::SouthAmerica | 0b0101,
    Uruguay   = +Region::SouthAmerica | 0b0110,
    Paraguay  = +Region::SouthAmerica | 0b0111,
    Bolivia   = +Region::SouthAmerica | 0b1000,
    Guyana    = +Region::SouthAmerica | 0b1010,
    Suriname  = +Region::SouthAmerica | 0b1011,
    Venezuela = +Region::SouthAmerica | 0b1100,
    Falklands = +Region::SouthAmerica | 0b1101,

    // Asia
    SaudiArabia        = +Region::Arabia | 0b0001,
    UnitedArabEmirates = +Region::Arabia | 0b0010,
    Oman               = +Region::Arabia | 0b0011,
    Yemen              = +Region::Arabia | 0b0100,
    Kuwait             = +Region::Arabia | 0b0101,
    Qatar              = +Region::Arabia | 0b0110,
    Bahrein            = +Region::Arabia | 0b0111,

    Armenia     = +Region::Caucasus | 0b0001,
    Georgia     = +Region::Caucasus | 0b0010,
    Azerbaijan  = +Region::Caucasus | 0b0011,

    Israel    = +Region::MiddleEast | 0b0001,
    Lebanon   = +Region::MiddleEast | 0b0010,
    Syria     = +Region::MiddleEast | 0b0011,
    Iraq      = +Region::MiddleEast | 0b0100,
    Jordan    = +Region::MiddleEast | 0b0101,
    Turkiye   = +Region::MiddleEast | 0b0110,
    Palestine = +Region::MiddleEast | 0b0111,

    Kazakhstan   = +Region::CentralAsia | 0b0001,
    Turkmenistan = +Region::CentralAsia | 0b0010,
    Kyrgyzstan   = +Region::CentralAsia | 0b0011,
    Tajikistan   = +Region::CentralAsia | 0b0100,
    Uzbekistan   = +Region::CentralAsia | 0b0101,

    India     = +Region::Hindustan | 0b0001,
    Nepal     = +Region::Hindustan | 0b0010,
    Maldives  = +Region::Hindustan | 0b0011,
    Bhutan    = +Region::Hindustan | 0b0100,
    Shrilanka = +Region::Hindustan | 0b0101,

    Afghanistan = +Region::GreaterKhorasan | 0b0001,
    Iran        = +Region::GreaterKhorasan | 0b0010,
    Pakistan    = +Region::GreaterKhorasan | 0b0011,

    Thailand = +Region::IndoChina | 0b0001,
    Vietnam  = +Region::IndoChina | 0b0010,
    Cambodia = +Region::IndoChina | 0b0011,
    Laos     = +Region::IndoChina | 0b0100,
    Myanmar  = +Region::IndoChina | 0b0101,

    Japan      = +Region::SinoJapanese | 0b0001,
    China      = +Region::SinoJapanese | 0b0010,
    Taiwan     = +Region::SinoJapanese | 0b0011,
    SouthKorea = +Region::SinoJapanese | 0b0100,
    HongKong   = +Region::SinoJapanese | 0b0101,
    NorthKorea = +Region::SinoJapanese | 0b0110,
    Mongolia   = +Region::SinoJapanese | 0b0111,
    Macau      = +Region::SinoJapanese | 0b1000,

    Brunei         = +Region::MalayArchipelago | 0b0001,
    Malaysia       = +Region::MalayArchipelago | 0b0010,
    Singapore      = +Region::MalayArchipelago | 0b0011,
    Philippines    = +Region::MalayArchipelago | 0b0100,
    Indonesia      = +Region::MalayArchipelago | 0b0101,
    EastTimor      = +Region::MalayArchipelago | 0b0110,


    // --- North Africa ---
    Morocco      = +Region::Maghreb | 0b0001,
    Algeria      = +Region::Maghreb | 0b0010,
    Tunisia      = +Region::Maghreb | 0b0011,
    Libya        = +Region::Maghreb | 0b0100,
    WesternSahara= +Region::Maghreb | 0b0101,

    Egypt        = +Region::NileValley | 0b0001,
    Sudan        = +Region::NileValley | 0b0010,
    SouthSudan   = +Region::NileValley | 0b0011,

    Mauritania   = +Region::Sahel | 0b0001,
    Mali         = +Region::Sahel | 0b0010,
    Niger        = +Region::Sahel | 0b0011,
    Chad         = +Region::Sahel | 0b0100,
    BurkinaFaso  = +Region::Sahel | 0b0101,

    Senegal      = +Region::GuineaCoast | 0b0001,
    Gambia       = +Region::GuineaCoast | 0b0010,
    Guinea       = +Region::GuineaCoast | 0b0011,
    GuineaBissau = +Region::GuineaCoast | 0b0100,
    SierraLeone  = +Region::GuineaCoast | 0b0101,
    Liberia      = +Region::GuineaCoast | 0b0110,
    IvoryCoast   = +Region::GuineaCoast | 0b0111,
    Ghana        = +Region::GuineaCoast | 0b1000,
    Togo         = +Region::GuineaCoast | 0b1001,
    Benin        = +Region::GuineaCoast | 0b1010,
    Nigeria      = +Region::GuineaCoast | 0b1011,

    Cameroon     = +Region::CongoBasin | 0b0001,
    CentralAfricanRepublic = +Region::CongoBasin | 0b0010,
    EquatorialGuinea       = +Region::CongoBasin | 0b0011,
    Gabon                  = +Region::CongoBasin | 0b0100,
    RepublicOfCongo        = +Region::CongoBasin | 0b0101,
    DR_Congo               = +Region::CongoBasin | 0b0110,

    Ethiopia     = +Region::HornOfAfrica | 0b0001,
    Eritrea      = +Region::HornOfAfrica | 0b0010,
    Djibouti     = +Region::HornOfAfrica | 0b0011,
    Somalia      = +Region::HornOfAfrica | 0b0100,

    Uganda       = +Region::GreatLakes | 0b0001,
    Rwanda       = +Region::GreatLakes | 0b0010,
    Burundi      = +Region::GreatLakes | 0b0011,
    Tanzania     = +Region::GreatLakes | 0b0100,
    Kenya        = +Region::GreatLakes | 0b0101,

    Angola       = +Region::SouthernAfrica | 0b0001,
    Zambia       = +Region::SouthernAfrica | 0b0010,
    Zimbabwe     = +Region::SouthernAfrica | 0b0011,
    Malawi       = +Region::SouthernAfrica | 0b0100,
    Mozambique   = +Region::SouthernAfrica | 0b0101,
    Namibia      = +Region::SouthernAfrica | 0b0110,
    Botswana     = +Region::SouthernAfrica | 0b0111,
    SouthAfrica  = +Region::SouthernAfrica | 0b1000,
    Lesotho      = +Region::SouthernAfrica | 0b1001,
    Eswatini     = +Region::SouthernAfrica | 0b1010,

    Madagascar   = +Region::IndianOceanAfrica | 0b0001,
    Comoros      = +Region::IndianOceanAfrica | 0b0010,
    Seychelles   = +Region::IndianOceanAfrica | 0b0011,
    Mauritius    = +Region::IndianOceanAfrica | 0b0100,

    // Oceania
    Australia   = +Region::AustralianAndNewZealand | 0b0001,
    NewZealand  = +Region::AustralianAndNewZealand | 0b0010,

    PapuaNewGuinea = +Region::Melanesia | 0b0001,
    Fiji           = +Region::Melanesia | 0b0010,
    SolomonIslands = +Region::Melanesia | 0b0011,
    Vanuatu        = +Region::Melanesia | 0b0100,
    NewCaledonia   = +Region::Melanesia | 0b0101, // (фр. територія)

    Palau                       = +Region::Micronesia | 0b0001,
    FederatedStatesOfMicronesia = +Region::Micronesia | 0b0010,
    MarshallIslands             = +Region::Micronesia | 0b0011,
    Nauru                       = +Region::Micronesia | 0b0100,
    Kiribati                    = +Region::Micronesia | 0b0101,
    Guam                        = +Region::Micronesia | 0b0110, // (US territory)
    NorthernMarianaIslands      = +Region::Micronesia | 0b0111, // (US territory)

    Samoa           = +Region::Polynesia | 0b0001,
    Tonga           = +Region::Polynesia | 0b0010,
    Tuvalu          = +Region::Polynesia | 0b0011,
    CookIslands     = +Region::Polynesia | 0b0100, // (self-governing, NZ assoc.)
    Niue            = +Region::Polynesia | 0b0101, // (NZ assoc.)
    FrenchPolynesia = +Region::Polynesia | 0b0110,
    WallisAndFutuna = +Region::Polynesia | 0b0111,
    AmericanSamoa   = +Region::Polynesia | 0b1000, // (US territory)
    Tokelau         = +Region::Polynesia | 0b1001, // (NZ territory)

    // Unknown
    SpecailArea = 0xFFFE,
    Unknown = 0xFFFF
};
constexpr uint16_t operator + (Country c) { return static_cast <uint16_t>(c); }
namespace country {
    QString primary(Country c);
    QString longest(Country c);

    // ISO коди
    QString alpha2(Country c); // "US"
    QString alpha3(Country c); // "USA"

    // Розпізнавання з рядка: аліаси/повна назва/ISO
    Country from_string(const QString& any);

    Continent continent(Country c);
    Region region   (Country c);

    std::vector <Country> all(Region    r = Region::Unknown);
    std::vector <Country> all(Continent c = Continent::None);

    QStringList all_names(Region    r = Region::Unknown);
    QStringList all_names(Continent c = Continent::None);

    QStringList all_alpha2(Region    r = Region::Unknown);
    QStringList all_alpha2(Continent c = Continent::None);

    QStringList all_alpha3(Region    r = Region::Unknown);
    QStringList all_alpha3(Continent c = Continent::None);
}
QString  operator - (Country c);
QString  operator & (Country c); // "US"
QString  operator ~ (Country c); // "USA"

QDataStream& operator << (QDataStream& out, const Country& c);
QDataStream& operator >> (QDataStream& in,        Country& c);
// ---------------------------                       ----------------------------------------------
// =========================== ^ Geography section ^ ==============================================
// ================================================================================================



// ------------------------------------------------------------------------------------------------
// --------------------------- Economy section ----------------------------------------------------
// ------------------------------------------------------------------------------------------------
enum class Currency : uint16_t {
    // Africa
    South_African_Rand = 0xCA00, // ZAR
    Kenyan_Shilling,      // KES
    Egyptian_Pound,       // EGP
    Moroccan_Dirham,      // MAD
    CFA_Franc,            // XOF
    Nigerian_Naira ,      // NGN
    Tunisian_Dinar,       // TND
    Ugandan_Shilling,     // UGX
    Central_African,      // XAF
    Libyan_Dinar,         // LYD
    Mauritian_Rupee,      // MUR
    Ghanaian_Cedi,        // GHS
    Angolan_Kwanza,       // AOA
    Tanzanian_Shilling,   // TZS
    Zimbabwean_Dollar,    // ZWD
    Botswana_Pula,        // BWP
    Algerian_Dinar,       // DZD
    Ethiopian_Birr,       // ETB
    Namibian_Dollar,      // NAD
    Sudanese_Pound,       // SDG
    Malagasy_Ariary,      // MGA
    Mozambican_Metical,   // MZN
    Seychellois_Rupee,    // SCR
    Sierra_Leonean_Leone, // SLL
    Malawian_Kwacha,      // MWK
    Gambian_Dalasi,       // GMD
    Burundian_Franc,      // BIF
    Somali_Shilling,      // SOS
    Guinean_Franc,        // GNF
    Congolese_Franc,      // CDF
    Sao_Tomean_Dobra,     // STD
    Basotho_Loti,         // LSL
    Liberian_Dollar,      // LRD
    Mauritanian_Ouguiya,  // MRU
    Swazi_Lilangeni,      // SZL
    Cape_Verdean_Escudo,  // CVE
    Rwandan_Franc,        // RWF
    Djiboutian_Franc,     // DJF
    Comorian_Franc,       // KMF
    Eritrean_Nakfa,       // ERN
    Saint_Helenian_Pound, // SHP
    Zambian_Kwacha,       // ZMW

    // Asia
    Indian_Rupee = 0xCB00,  // INR
    Singapore_Dollar,      // SGD
    Malaysian_Ringgit,     // MYR
    Japanese_Yen,          // JPY
    Chinese_Yuan,          // CNY
    Thai_Baht,             // THB
    Emirati_Dirham,        // AED
    Hong_Kong_Dollar,      // HKD
    Turkish_Lira,          // TRY
    Philippine_Piso,       // PHP
    Indonesian_Rupiah,     // IDR
    Saudi_Arabian_Riyal,   // SAR
    South_Korean_Won,      // KRW
    Iraqi_Dinar,           // IQD
    Armenian_Dram,         // AMD
    Azerbaijan_Manat,      // AZN
    Georgian_Lari,         // GEL
    Kuwaiti_Dinar,         // KWD
    Russian_Ruble,         // RUB
    Pakistani_Rupee,       // PKR
    Israeli_Shekel,        // ILS
    Qatari_Riyal,          // QAR
    Omani_Rial,            // OMR
    Taiwan_New_Dollar,     // TWD
    Vietnamese_Dong,       // VND
    Jordanian_Dinar,       // JOD
    Bahraini_Dinar,        // BHD
    Sri_Lankan_Rupee,      // LKR
    Bangladeshi_Taka,      // BDT
    Uzbekistani_Som,       // UZS
    Iranian_Rial,          // IRR
    Syrian_Pound,          // SYP
    Afghan_Afghani,        // AFN
    Lebanese_Pound,        // LBP
    Nepalese_Rupee,        // NPR
    Lao_Kip,               // LAK
    Bruneian_Dollar,       // BND
    Macau_Pataca,          // MOP
    Kazakhstani_Tenge,     // KZT
    Yemeni_Rial,           // YER
    Maldivian_Rufiyaa,     // MVR
    Mongolian_Tughrik,     // MNT
    Tajikistani_Somoni,    // TJS
    North_Korean_Won,      // KPW
    Burmese_Kyat,          // MMK
    Kyrgyzstani_Som,       // KGS
    Cambodian_Riel,        // KHR
    Bhutanese_Ngultrum,    // BTN
    Turkmenistani_Manat,   // TMT
    Korean_Won,            // KRW

    // Europe
    Euro = 0xCC00,      // EUR
    British_Pound,     // GBP
    Swiss_Franc,       // CHF
    Hungarian_Forint,  // HUF
    Swedish_Krona,     // SEK
    Norwegian_Krone,   // NOK
    Danish_Krone,      // DKK
    Polish_Zloty,      // PLN
    Czech_Koruna,      // CZK
    Ukrainian_Hryvnia, // UAH
    Romanian_Leu,      // RON
    Belarusian_Ruble,  // BYR
    New_Belarusian_Ruble, // BYN
    Croatian_Kuna,     // HRK,
    Bulgarian_Lev,     // BGN
    Icelandic_Krona,   // ISK
    Albanian_Lek,      // ALL
    Bosnian_Convertible_Marka, // BAM
    Serbian_Dinar,     // RSD
    Gibraltar_Pound,   // GIP
    Moldovan_Leu,      // MDL
    Macedonian_Denar,  // MKD
    Dutch_Guilder,     // ANG
    Seborgan_Luigino,  // SPL
    Jersey_Pound,      // JEP
    Isle_of_Man_Pound, // IMP
    Guernsey_Pound,    // GGP
    GreatBritain_Pence,// GBP

    // North America
    US_Dollar = 0xCD00,  // USD
    Canadian_Dollar,    // CAD
    Mexican_Peso,       // MXN
    Dominican_Peso,     // DOP,
    Costa_Rican_Colon,  // CRC,
    Jamaican_Dollar,    // JMD
    Trinidadian_Dollar,         // TTD
    East_Caribbean_Dollar,      // XCD
    Guatemalan_Quetzal,         // GTQ
    Barbadian_or_Bajan_Dollar,  // BBD
    Cuban_Convertible_Peso,     // CUC
    Honduran_Lempira,           // HNL
    Nicaraguan_Cordoba,         // NIO
    Bermudian_Dollar,           // BMD
    Panamanian_Balboa,          // PAB
    Caymanian_Dollar,           // KYD
    Belizean_Dollar,            // BZD
    Haitian_Gourde,             // HTG
    Cuban_Peso,                 // CUP
    Salvadoran_Colon,           // SVC
    Bahamian_Dollar,            // BSD
    IMF_Special,                // XDR
    Aruban_or_Dutch_Guilder,    // AWG

    // Oceania
    Australian_Dollar = 0xCE00, // AUD
    New_Zealand_Dollar,        // NZD
    Fijian_Dollar,             // FJD
    CFP_Franc,                 // XPF
    Papua_New_Guinean_Kina,    // PGK
    Solomon_Islander_Dollar,   // SBD
    Tongan_Paanga,             // TOP
    Ni_Vanuatu_Vatu,           // VUV
    Samoan_Tala,               // WST
    Tuvaluan_Dollar,           // TVD

    // South America
    Brazilian_Real = 0xCF00, // BRL
    Colombian_Peso,         // COP
    Chilean_Peso,           // CLP
    Argentine_Peso,         // ARS
    Peruvian_Sol,           // PEN
    Venezuelan_Bolivar,     // VES
    Uruguayan_Peso,         // UYU
    Bolivian_Boliviano,     // BOB
    Paraguayan_Guarani,     // PYG
    Guyanese_Dollar,        // GYD
    Surinamese_Dollar,      // SRD
    Falkland_Island_Pound,  // FKP

    None = 0xCFF0,
    Bitcoin,         // BTC
    Gold_Ounce,      // XAU
    Silver_Ounce,    // XAG
    Platinum_Ounce,  // XPT
    Palladium_Ounce, // XPD
};
constexpr uint16_t operator + (Currency c) { return static_cast <uint16_t>(c); }
namespace currency {
    QString to_short(Currency type);
    QString to_short(QString full);

    QString to_full(Currency type);
    QString to_full(QString m_short);

    Currency to_enum(QString currency);
    Currency from_short(QString currency);

    QStringList all_short();

    int32_t scale(Currency c);
}
QString operator & (Currency c);
QString operator ~ (Currency c);

QDataStream& operator << (QDataStream& out, const Currency& tag);
QDataStream& operator >> (QDataStream& in,        Currency& tag);


enum class Exchange : uint8_t {
    // ----------------------------------------------------------------------------------------
    US        = 0b0000'0000, // 000.'.... // US
    NASDAQ    = US | 0b0001, // 0000'0...
    NYSE      = US | 0b1000, // 0000'1...
    NYSE_ARCA = NYSE| 0b001,
    NYSE_MKT  = NYSE| 0b010,
    AMEX      = NYSE| 0b011,
    BATS      = NYSE| 0b100,

    PINK         = 0b1'0000, // 0001'0...
    OTC_GREY = PINK | 0b001,
    OTC_CE   = PINK | 0b010,
    OTC_BB   = PINK | 0b011,

    OTC      = 0b0001'1000, // 0001'1...
    OTC_QB   = OTC | 0b001,
    OTC_QX   = OTC | 0b010,
    NMFQA    = OTC | 0b011,
    OTC_MKTS = OTC | 0b100, // 0001'11..
    OTC_MTKS = OTC | 0b101,

    // ----------------------------------------------------------------------------------------
    America          = 0b0010'0000, // 001. .... america
    Toronto     = America | 0b0001,
    NEO         = America | 0b0010,
    TSX         = America | 0b0011,
    Chilean     = America | 0b0100,
    BuenosAires = America | 0b0101,
    SaoPaulo    = America | 0b0110,
    Mexico      = America | 0b0111,
    Lima        = America | 0b1000,

    Africa              = 0b0100'0000, // 010.'.... africa
    VictoriaFalls = Africa | 0b0'0001,
    Zimbabwe      = Africa | 0b0'0010,
    Lusaka        = Africa | 0b0'0011,
    Uganda        = Africa | 0b0'0100,
    Dar_es_Salaam = Africa | 0b0'0101,
    Rwanda        = Africa | 0b0'0110,
    Botswana      = Africa | 0b0'0111,
    Nigerian      = Africa | 0b0'1000,
    Egyptian      = Africa | 0b0'1001,
    Malawi        = Africa | 0b0'1010,
    Ghana         = Africa | 0b0'1011,
    Nairobi       = Africa | 0b0'1100,
    Casablanca    = Africa | 0b0'1101,
    Mauritius     = Africa | 0b0'1110,
    Johannesburg  = Africa | 0b0'1111,

    Asia           = 0b0110'0000, // 011.'.... asia
    Taiwan     = Asia | 0b0'0001,
    TaiwanOTC  = Asia | 0b0'0010,
    Korea      = Asia | 0b0'0011,
    KOSDAQ     = Asia | 0b0'0100,
    Tel_aviv   = Asia | 0b0'0101,
    Shenzhen   = Asia | 0b0'0110,
    Vietnam    = Asia | 0b0'0111,
    Philippine = Asia | 0b0'1000,
    Shanghai   = Asia | 0b0'1001,
    Jakarta    = Asia | 0b0'1010,
    India      = Asia | 0b0'1011,
    Karachi    = Asia | 0b0'1100,
    Thailand   = Asia | 0b0'1101,
    Colombo    = Asia | 0b0'1110,
    Kuala      = Asia | 0b0'1111,

    // ----------------------------------------------------------------------------------------
    Global             = 0b1001'0000, // 1000 .... Global exchanges
    EuropeFund      = Global | 0b001,
    GovernmentBonds = Global | 0b010,
    MoneyMarket     = Global | 0b011,
    Forex           = Global | 0b100,

    Unknown = 0b1001'0000, // 1001 ....

    Oceania   =      0b1011'1000, // 1011'.... oceania
    Australia = Oceania |  0b001,
    // ----------------------------------------------------------------------------------------
    EU             = 0b1100'0000, // 11..'....
    EU_WEST     = EU | 0b00'0000, // 1100'.... // west europe
    Paris     = EU_WEST | 0b0001,
    Amsterdam = EU_WEST | 0b0010,
    Luxemburg = EU_WEST | 0b0011,
    Swiss     = EU_WEST | 0b0100,
    Madrid    = EU_WEST | 0b0101,
    Brussels  = EU_WEST | 0b0110,
    Vienna    = EU_WEST | 0b0111,
    Lisbon    = EU_WEST | 0b1000,
    Irish     = EU_WEST | 0b1001,
    UK        = EU_WEST | 0b1010,
    LondonIL  = EU_WEST | 0b1011,
    LSE       = EU_WEST | 0b1100,

    EU_EAST     = EU | 0b01'0000, // 1101 .... // east europe
    Prague    = EU_EAST | 0b0001,
    Warsaw    = EU_EAST | 0b0010,
    Athens    = EU_EAST | 0b0011,
    Budapest  = EU_EAST | 0b0100,
    Zagreb    = EU_EAST | 0b0101,
    Bucharest = EU_EAST | 0b0110,

    DE      = EU | 0b10'0000, // 1110'.... // Germany 2
    Dusseldorf = DE | 0b0001,
    Hanover    = DE | 0b0010,
    Munich     = DE | 0b0011,
    Stuttgart  = DE | 0b0100,
    Frankfurt  = DE | 0b0101,
    Humburg    = DE | 0b0110,
    Berlin     = DE | 0b0111,
    XETRA      = DE | 0b1000,

    Scand      = EU | 0b10'0000, // 1111'.... // Scandinavia
    Oslo       = Scand | 0b0001,
    Helsinki   = Scand | 0b0010,
    Copenhagen = Scand | 0b0011,
    Iceland    = Scand | 0b0100,
    Sweden     = Scand | 0b1100, // 1111'11.. // Sweden
    Stockholm  = Sweden |  0b01,
    XSTO       = Sweden |  0b10,
    // ----------------------------------------------------------------------------------------
};
using ExchangeList = std::vector <Exchange>;
constexpr uint16_t operator + (Exchange e) { return static_cast <uint16_t>(e); }
namespace exchange {
    bool     exist      (Country c);
    bool     exist      (Exchange e);
    QString  sufix      (Exchange e);
    QString  name       (Exchange e);
    QString  venue      (Exchange e);
    Currency currency   (Exchange e);
    Country  country    (Exchange e);
    Exchange from_string(const QString& s);
    Exchange from_venue_string(const QString& s);

    bool is_major_europe_sufix(Exchange e);
    bool is_minor_europe_sufix(Exchange e);
    bool is_other_worlds_sufix(Exchange e);
    std::vector <Exchange> major_europe_sufix();
    std::vector <Exchange> minor_europe_sufix();
    std::vector <Exchange> other_worlds_sufix();
    std::vector <Exchange> us_sufix();
    std::vector <Exchange> all_exchange();
    QStringList all_exchange_venue();

    bool us       (Exchange e);
    bool nyse     (Exchange e);
    bool nasdaq   (Exchange e);
    bool otc      (Exchange e);
    bool europe   (Exchange e);
    bool trash    (Exchange e);
    bool euromajor(Exchange e);
    bool eurominor(Exchange e);
    bool asia     (Exchange e);
    bool world    (Exchange e);
    bool global   (Exchange e);
    bool oceania  (Exchange e);
    bool africa   (Exchange e);
    bool america  (Exchange e);
}
QString operator & (Exchange c); // return sufix (short symbols that set in the end of stock)
QString operator - (Exchange c); // return venue (short name of exchange)
QString operator ~ (Exchange c); // return name

QDataStream& operator << (QDataStream& out, const Currency& tag);
QDataStream& operator >> (QDataStream& in,        Currency& tag);


enum class Instype : uint8_t {
    Unknown        = 0b0000,
    CommonStock    = 0b0010,
    PreferredStock = 0b0011,
    ETC            = 0b0100,
    ETF            = 0b0101,
    Fund           = 0b0110,
    MutualFund     = 0b0111,
    Unit           = 0b1000,
    Bond           = 0b1010,
    Index          = 0b1100,
    Note           = 0b1110,
    CapitalNotes   = 0b1111,
};
constexpr uint8_t operator + (Instype e) { return static_cast <uint8_t>(e); }
namespace instype {
    Instype from_string(const QString& s);
    QString to_string(Instype e);
    QString file_name(Instype e);
    std::vector <Instype> all();
    QStringList all_names();
}
QString operator ~ (Instype e);

QDataStream& operator << (QDataStream& out, const Instype& tag);
QDataStream& operator >> (QDataStream& in,        Instype& tag);
// -------------------------                     --------------------------------------------------
// ========================= ^ Economy section ^ ==================================================
// ================================================================================================


enum FieldType {
    Ticker_code,
    Quotes_beta,

    Meta_type,
    Meta_name,
    Meta_sector,
    Meta_industry,
    Meta_gic_sector,
    Meta_gic_group,
    Meta_gic_industry,
    Meta_gic_subindustry,
    Meta_home_category,
    Meta_description,
    Meta_officers,
    Meta_phone_number,
    Meta_website,
    Meta_logo,
    Meta_logo_url,
    Meta_logo_full,
    Meta_fulltime_employees,

    Legal_openFigi,
    Legal_isin,
    Legal_lei,
    Legal_cik,
    Legal_fiscalYearEnd,
    Legal_ein,
    Legal_cusip,
    Legal_ipoDate,
    Legal_internationalDomestic,
    Legal_delisted,
    Legal_address,
    Legal_address_json,

    Fundam_ebitda,
    Fundam_bookvalue,
    Fundam_recent_quart,
    Fundam_revenue_ttm,
    Fundam_gross_profit_ttm,
    Fundam_annual_earnings,
    Fundam_balance, // TODO separate it
    Fundam_cashflow, // TODO separate it
    Fundam_incomes, // TODO separate it
    Fundam_earnings, // TODO separate it

    Est_wallstr_target_price,
    Est_eps_curr_year,
    Est_eps_next_year,
    Est_eps_curr_quart,
    Est_eps_next_quart,
    Est_trend,

    Analyst_rate,
    Analyst_targetprice,
    Analyst_strongbuy,
    Analyst_buy,
    Analyst_hold,
    Analyst_sell,
    Analyst_strongsell,

    Dividend_per_share,
    Dividend_yield,
    Dividend_fw_anno_rate,
    Dividend_fw_anno_yield,
    Dividend_payout_ratio,
    Dividend_date,
    Dividend_exdate,

    Fin_earnings_share,

    Cap_shares_outstand,
    Cap_shares_float,
    Cap_percent_of_insiders,
    Cap_percent_institution,
    Cap_outstand_shares,

    Short_shares,
    Short_shares_priormonth,

    Corp_last_split_factor,
    Corp_split_date,

    Earnings_q,
    Balance_q,
    CashFlow_q,
    Incomes_q,
    Trend_q,

    Stock_new,
    Stock_count,
    Stock_price,
    Stock_list,

    Transaction_is_buy,
    Transaction_price,
    Transaction_count,
    Transaction_broker,
    Transaction_dtime,
};

enum class DataType {
    InstrumentData,
    Legal,
    Meta,
    Fundamental,
    Capital,
    Estimates,

    // Ticker inside
    Dividend,
    Quotes,
    ShortInterest,
    CorporateAction,
    Valuation,

    // quartel
    Balance,
    CashFlow,
    Earnings,
    Incomes,
    Trend,
};
}
#endif // SDK_ENUM_H
