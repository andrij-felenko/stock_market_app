#ifndef GEO_COUNTRY_H
#define GEO_COUNTRY_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QDataStream>
#include <cstdint>
#include "continent.h"

namespace geo {

    enum class Country : uint16_t {
        // Europe
        Ukraine  = +Subregion::EuropeanBlackSea | 0b0001,
        Moldova  = +Subregion::EuropeanBlackSea | 0b0010,
        Romania  = +Subregion::EuropeanBlackSea | 0b0011,
        Bulgaria = +Subregion::EuropeanBlackSea | 0b0100,

        Poland   = +Subregion::Visegrad | 0b0001,
        Czechia  = +Subregion::Visegrad | 0b0010,
        Slovakia = +Subregion::Visegrad | 0b0011,
        Hungary  = +Subregion::Visegrad | 0b0100,

        Estonia   = +Subregion::BalticsStates | 0b0001,
        Latvia    = +Subregion::BalticsStates | 0b0010,
        Lithuania = +Subregion::BalticsStates | 0b0011,
        Belarus   = +Subregion::BalticsStates | 0b0100,

        Androra   = +Subregion::Iberia | 0b0001,
        Portugal  = +Subregion::Iberia | 0b0010,
        Spain     = +Subregion::Iberia | 0b0011,
        Gibraltar = +Subregion::Iberia | 0b0100,

        Malta    = +Subregion::SouthernEurope | 0b0001,
        Cyprus   = +Subregion::SouthernEurope | 0b0010,

        Greece               = +Subregion::Balkans | 0b0001,
        Albania              = +Subregion::Balkans | 0b0010,
        NorthMacedonia       = +Subregion::Balkans | 0b0011,
        Serbia               = +Subregion::Balkans | 0b0100,
        BosniaAndHerzegovina = +Subregion::Balkans | 0b0101,
        Montenegro           = +Subregion::Balkans | 0b0110,
        Kosovo               = +Subregion::Balkans | 0b0111,

        Italy     = +Subregion::ItaloAdriatic | 0b0001,
        Croatia   = +Subregion::ItaloAdriatic | 0b0010,
        Slovenia  = +Subregion::ItaloAdriatic | 0b0011,
        SanMarino = +Subregion::ItaloAdriatic | 0b0100,
        Monaco    = +Subregion::ItaloAdriatic | 0b0101,
        Vatican   = +Subregion::ItaloAdriatic | 0b0110,

        Sweden    = +Subregion::Scandinavia | 0b0001,
        Norway    = +Subregion::Scandinavia | 0b0010,
        Finland   = +Subregion::Scandinavia | 0b0011,
        Iceland   = +Subregion::Scandinavia | 0b0100,
        Denmark   = +Subregion::Scandinavia | 0b0101,
        Greenland = +Subregion::Scandinavia | 0b0110,

        Belgium     = +Subregion::Benelux | 0b0001,
        Netherlands = +Subregion::Benelux | 0b0010,
        Luxembourg  = +Subregion::Benelux | 0b0011,

        UnitedKingdom = +Subregion::BritishIsles | 0b0001,
        IsleOfMan     = +Subregion::BritishIsles | 0b0010,
        Jersey        = +Subregion::BritishIsles | 0b0011,
        Guernsey      = +Subregion::BritishIsles | 0b0100,
        Ireland       = +Subregion::BritishIsles | 0b0101,

        France      = +Subregion::CentralEurope | 0b0001,
        Germany     = +Subregion::CentralEurope | 0b0010,
        Switzerland = +Subregion::CentralEurope | 0b0011,
        Austria     = +Subregion::CentralEurope | 0b0100,

        // America
        UnitedStates  = +Subregion::AngloAmerica | 0b0001,
        Canada        = +Subregion::AngloAmerica | 0b0010,

        Mexico     = +Subregion::CentralAmerica | 0b0001,
        Panama     = +Subregion::CentralAmerica | 0b0010,
        Nicaragua  = +Subregion::CentralAmerica | 0b0011,
        Honduras   = +Subregion::CentralAmerica | 0b0100,
        Guatemala  = +Subregion::CentralAmerica | 0b0101,
        ElSalvador = +Subregion::CentralAmerica | 0b0110,
        Belize     = +Subregion::CentralAmerica | 0b0111,

        Cuba              = +Subregion::Caribbean | 0b0001,
        CostaRica         = +Subregion::Caribbean | 0b0010,
        DominicanRepublic = +Subregion::Caribbean | 0b0011,
        PuertoRico        = +Subregion::Caribbean | 0b0100,
        Jamaica           = +Subregion::Caribbean | 0b0101,
        Bahamas           = +Subregion::Caribbean | 0b0110,
        CaymanIslands     = +Subregion::Caribbean | 0b0111,
        Bermuda           = +Subregion::Caribbean | 0b1000,
        Haiti             = +Subregion::Caribbean | 0b1001,
        Barbados          = +Subregion::Caribbean | 0b1010,
        Grenada           = +Subregion::Caribbean | 0b1011,

        Brazil    = +Subregion::SouthAmerica | 0b0001,
        Argentina = +Subregion::SouthAmerica | 0b0010,
        Chile     = +Subregion::SouthAmerica | 0b0011,
        Colombia  = +Subregion::SouthAmerica | 0b0100,
        Peru      = +Subregion::SouthAmerica | 0b0101,
        Uruguay   = +Subregion::SouthAmerica | 0b0110,
        Paraguay  = +Subregion::SouthAmerica | 0b0111,
        Bolivia   = +Subregion::SouthAmerica | 0b1000,
        Guyana    = +Subregion::SouthAmerica | 0b1010,
        Suriname  = +Subregion::SouthAmerica | 0b1011,
        Venezuela = +Subregion::SouthAmerica | 0b1100,
        Falklands = +Subregion::SouthAmerica | 0b1101,

        // Asia
        SaudiArabia        = +Subregion::Arabia | 0b0001,
        UnitedArabEmirates = +Subregion::Arabia | 0b0010,
        Oman               = +Subregion::Arabia | 0b0011,
        Yemen              = +Subregion::Arabia | 0b0100,
        Kuwait             = +Subregion::Arabia | 0b0101,
        Qatar              = +Subregion::Arabia | 0b0110,
        Bahrein            = +Subregion::Arabia | 0b0111,

        Armenia     = +Subregion::Caucasus | 0b0001,
        Georgia     = +Subregion::Caucasus | 0b0010,
        Azerbaijan  = +Subregion::Caucasus | 0b0011,

        Israel    = +Subregion::MiddleEast | 0b0001,
        Lebanon   = +Subregion::MiddleEast | 0b0010,
        Syria     = +Subregion::MiddleEast | 0b0011,
        Iraq      = +Subregion::MiddleEast | 0b0100,
        Jordan    = +Subregion::MiddleEast | 0b0101,
        Turkiye   = +Subregion::MiddleEast | 0b0110,
        Palestine = +Subregion::MiddleEast | 0b0111,

        Kazakhstan   = +Subregion::CentalAsia | 0b0001,
        Turkmenistan = +Subregion::CentalAsia | 0b0010,
        Kyrgyzstan   = +Subregion::CentalAsia | 0b0011,
        Tajikistan   = +Subregion::CentalAsia | 0b0100,
        Uzbekistan   = +Subregion::CentalAsia | 0b0101,

        India     = +Subregion::Hindustan | 0b0001,
        Nepal     = +Subregion::Hindustan | 0b0010,
        Maldives  = +Subregion::Hindustan | 0b0011,
        Bhutan    = +Subregion::Hindustan | 0b0100,
        Shrilanka = +Subregion::Hindustan | 0b0101,

        Afghanistan = +Subregion::GreatKhrasan | 0b0001,
        Iran        = +Subregion::GreatKhrasan | 0b0010,
        Pakistan    = +Subregion::GreatKhrasan | 0b0011,

        Thailand = +Subregion::IndoChina | 0b0001,
        Vietnam  = +Subregion::IndoChina | 0b0010,
        Cambodia = +Subregion::IndoChina | 0b0011,
        Laos     = +Subregion::IndoChina | 0b0100,
        Myanmar  = +Subregion::IndoChina | 0b0101,

        Japan      = +Subregion::SinoJapanese | 0b0001,
        China      = +Subregion::SinoJapanese | 0b0010,
        Taiwan     = +Subregion::SinoJapanese | 0b0011,
        SouthKorea = +Subregion::SinoJapanese | 0b0100,
        HongKong   = +Subregion::SinoJapanese | 0b0101,
        NorthKorea = +Subregion::SinoJapanese | 0b0110,
        Mongolia   = +Subregion::SinoJapanese | 0b0111,
        Macau      = +Subregion::SinoJapanese | 0b1000,

        Brunei         = +Subregion::MalayArchipelago | 0b0001,
        Malaysia       = +Subregion::MalayArchipelago | 0b0010,
        Singapore      = +Subregion::MalayArchipelago | 0b0011,
        Philippines    = +Subregion::MalayArchipelago | 0b0100,
        Indonesia      = +Subregion::MalayArchipelago | 0b0101,
        EastTimor      = +Subregion::MalayArchipelago | 0b0110,


        // --- North Africa ---
        Morocco      = +Subregion::Maghreb | 0b0001,
        Algeria      = +Subregion::Maghreb | 0b0010,
        Tunisia      = +Subregion::Maghreb | 0b0011,
        Libya        = +Subregion::Maghreb | 0b0100,
        WesternSahara= +Subregion::Maghreb | 0b0101,

        Egypt        = +Subregion::NileValley | 0b0001,
        Sudan        = +Subregion::NileValley | 0b0010,
        SouthSudan   = +Subregion::NileValley | 0b0011,

        Mauritania   = +Subregion::Sahel | 0b0001,
        Mali         = +Subregion::Sahel | 0b0010,
        Niger        = +Subregion::Sahel | 0b0011,
        Chad         = +Subregion::Sahel | 0b0100,
        BurkinaFaso  = +Subregion::Sahel | 0b0101,

        Senegal      = +Subregion::GuineaCoast | 0b0001,
        Gambia       = +Subregion::GuineaCoast | 0b0010,
        Guinea       = +Subregion::GuineaCoast | 0b0011,
        GuineaBissau = +Subregion::GuineaCoast | 0b0100,
        SierraLeone  = +Subregion::GuineaCoast | 0b0101,
        Liberia      = +Subregion::GuineaCoast | 0b0110,
        IvoryCoast   = +Subregion::GuineaCoast | 0b0111,
        Ghana        = +Subregion::GuineaCoast | 0b1000,
        Togo         = +Subregion::GuineaCoast | 0b1001,
        Benin        = +Subregion::GuineaCoast | 0b1010,
        Nigeria      = +Subregion::GuineaCoast | 0b1011,

        Cameroon     = +Subregion::CongoBasin | 0b0001,
        CentralAfricanRepublic = +Subregion::CongoBasin | 0b0010,
        EquatorialGuinea       = +Subregion::CongoBasin | 0b0011,
        Gabon                  = +Subregion::CongoBasin | 0b0100,
        RepublicOfCongo        = +Subregion::CongoBasin | 0b0101,
        DR_Congo               = +Subregion::CongoBasin | 0b0110,

        Ethiopia     = +Subregion::HornOfAfrica | 0b0001,
        Eritrea      = +Subregion::HornOfAfrica | 0b0010,
        Djibouti     = +Subregion::HornOfAfrica | 0b0011,
        Somalia      = +Subregion::HornOfAfrica | 0b0100,

        Uganda       = +Subregion::GreatLakes | 0b0001,
        Rwanda       = +Subregion::GreatLakes | 0b0010,
        Burundi      = +Subregion::GreatLakes | 0b0011,
        Tanzania     = +Subregion::GreatLakes | 0b0100,
        Kenya        = +Subregion::GreatLakes | 0b0101,

        Angola       = +Subregion::SouthernAfrica | 0b0001,
        Zambia       = +Subregion::SouthernAfrica | 0b0010,
        Zimbabwe     = +Subregion::SouthernAfrica | 0b0011,
        Malawi       = +Subregion::SouthernAfrica | 0b0100,
        Mozambique   = +Subregion::SouthernAfrica | 0b0101,
        Namibia      = +Subregion::SouthernAfrica | 0b0110,
        Botswana     = +Subregion::SouthernAfrica | 0b0111,
        SouthAfrica  = +Subregion::SouthernAfrica | 0b1000,
        Lesotho      = +Subregion::SouthernAfrica | 0b1001,
        Eswatini     = +Subregion::SouthernAfrica | 0b1010,

        Madagascar   = +Subregion::IndianOceanAfrica | 0b0001,
        Comoros      = +Subregion::IndianOceanAfrica | 0b0010,
        Seychelles   = +Subregion::IndianOceanAfrica | 0b0011,
        Mauritius    = +Subregion::IndianOceanAfrica | 0b0100,

        // Oceania
        Australia   = +Subregion::AustralianAndNewZealand | 0b0001,
        NewZealand  = +Subregion::AustralianAndNewZealand | 0b0010,

        PapuaNewGuinea = +Subregion::Melanesia | 0b0001,
        Fiji           = +Subregion::Melanesia | 0b0010,
        SolomonIslands = +Subregion::Melanesia | 0b0011,
        Vanuatu        = +Subregion::Melanesia | 0b0100,
        NewCaledonia   = +Subregion::Melanesia | 0b0101, // (фр. територія)

        Palau                       = +Subregion::Micronesia | 0b0001,
        FederatedStatesOfMicronesia = +Subregion::Micronesia | 0b0010,
        MarshallIslands             = +Subregion::Micronesia | 0b0011,
        Nauru                       = +Subregion::Micronesia | 0b0100,
        Kiribati                    = +Subregion::Micronesia | 0b0101,
        Guam                        = +Subregion::Micronesia | 0b0110, // (US territory)
        NorthernMarianaIslands      = +Subregion::Micronesia | 0b0111, // (US territory)

        Samoa           = +Subregion::Polynesia | 0b0001,
        Tonga           = +Subregion::Polynesia | 0b0010,
        Tuvalu          = +Subregion::Polynesia | 0b0011,
        CookIslands     = +Subregion::Polynesia | 0b0100, // (self-governing, NZ assoc.)
        Niue            = +Subregion::Polynesia | 0b0101, // (NZ assoc.)
        FrenchPolynesia = +Subregion::Polynesia | 0b0110,
        WallisAndFutuna = +Subregion::Polynesia | 0b0111,
        AmericanSamoa   = +Subregion::Polynesia | 0b1000, // (US territory)
        Tokelau         = +Subregion::Polynesia | 0b1001, // (NZ territory)

        // Unknown
        Unknown = 0xFFFF
    };

    struct CountryName {
        static QString primary(Country c);
        static QString longest(Country c);

        // ISO коди
        static QString alpha2    (Country c); // "US"
        static QString alpha3    (Country c); // "USA"

        // Розпізнавання з рядка: аліаси/повна назва/ISO
        static Country from_string(const QString& any);
        static Country from_alpha2(const QString& a2);
        static Country from_alpha3(const QString& a3);
        static Country from_full  (const QString& full);

        // Географія
        static Continent continent(Country c);
        static Subregion region   (Country c);

        // Утиліти
        static QList<Country> all();
    };

    uint16_t operator + (Country c) { return static_cast <uint16_t>(c); }
    QString  operator - (Country c) { return CountryName::alpha3(c); }
    QString  operator & (Country c) { return CountryName::alpha2(c); }
    QString  operator ~ (Country c) { return CountryName::primary(c); }
} // namespace geo

// QDataStream для зберігання/читання у файл
QDataStream& operator << (QDataStream& out, const geo::Country& c);
QDataStream& operator >> (QDataStream& in,        geo::Country& c);

#endif // GEO_COUNTRY_H
