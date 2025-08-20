#ifndef CURRENCY_NAME_H
#define CURRENCY_NAME_H

#include <QtCore/QString>
#include <QtCore/QList>
#include "data/geo/continent.h"

namespace currency {
    class Name;

    enum class Tag : uint16_t {
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
}

class currency::Name
{
public:
    static Name& instance();
    static QString to_short(Tag type);
    static QString to_short(QString full);
    static QString to_full(Tag type);
    static QString to_full(QString m_short);
    static Tag to_enum(QString currency);
    static Tag from_short(QString currency);
    static geo::Continent continent(QString name);
    static geo::Continent continent(Tag type);
    static QStringList allShort();

private:
    Name();
    Name(Tag const&) = delete;
    void operator = (Tag const&) = delete;

    struct TypeStruct {
        Tag       _enum;
        QString   _full;
        QString   _short;
        geo::Continent _continent;
    };

    QList <TypeStruct> _list;

    void _add(Tag type, QString full, QString short_,
              geo::Continent continent = geo::Continent::None);
    inline void _add_africa();
    inline void _add_east_asia();
    inline void _add_west_asia();
    inline void _add_europe();
    inline void _add_north_america();
    inline void _add_oceania();
    inline void _add_south_america();
};

QDataStream& operator << (QDataStream& out, const currency::Tag& tag);
QDataStream& operator >> (QDataStream& in,        currency::Tag& tag);

#endif
