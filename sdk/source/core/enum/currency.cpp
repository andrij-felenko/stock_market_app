#include "core/enum.h"

namespace sdk::currency {
    struct Meta {
        Currency  _enum;
        QString   _full;
        QString   _short;
    };

    const std::vector <Meta>& metadata();
}

namespace sdk {
    QDataStream& operator << (QDataStream& out, const Currency& tag)
    { return out << static_cast <uint16_t > (tag); }

    QDataStream& operator >> (QDataStream& in, Currency& tag) {
        uint16_t  val;
        in >> val;
        tag = static_cast <Currency>(val);
        return in;
    }
}

int32_t sdk::currency::scale(sdk::Currency c)
{
    switch (c){
        case Currency::Japanese_Yen:        // JPY
        case Currency::South_Korean_Won:    // KRW (є також дубль нижче)
        case Currency::Korean_Won:          // KRW (дубль у enum)
        case Currency::Chinese_Yuan:        // CNY
        case Currency::Indian_Rupee:        // INR
        case Currency::Indonesian_Rupiah:   // IDR
        case Currency::Russian_Ruble:       // RUB
        case Currency::Colombian_Peso:      // COP
        case Currency::Chilean_Peso:        // CLP
        case Currency::Nigerian_Naira:      // NGN
        case Currency::Uzbekistani_Som:     // UZS
        case Currency::Argentine_Peso:      // ARS
        case Currency::Paraguayan_Guarani:  // PYG
        case Currency::Cambodian_Riel:      // KHR
        case Currency::Lao_Kip:             // LAK
        case Currency::Mongolian_Tughrik:   // MNT
        case Currency::Iraqi_Dinar:         // IQD
        case Currency::GreatBritain_Pence:  // GBP у пенсах → числа ×100
            return 1'000'000;
        case Currency::Lebanese_Pound:
        case Currency::Iranian_Rial:
        case Currency::Venezuelan_Bolivar:
        case Currency::Vietnamese_Dong:
            return 10'000'000;
        default:;
    }
    return 10'000;
}

QString operator & (sdk::Currency c) { return sdk::currency::to_short(c); }
QString operator ~ (sdk::Currency c) { return sdk::currency::to_full (c); }

QString sdk::currency::to_short(sdk::Currency type)
{
    for (const auto &it : metadata())
        if (it._enum == type)
            return it._short;
    return "NaN";
}

QString sdk::currency::to_short(QString full)
{
    for (const auto &it : metadata())
        if (it._full.compare(full, Qt::CaseInsensitive) == 0 ||
            it._short.compare(full, Qt::CaseInsensitive) == 0)
            return it._short;
    return "NaN";
}

QString sdk::currency::to_full(sdk::Currency type)
{
    for (const auto &it : metadata())
        if (it._enum == type)
            return it._full;
    return "NaN";
}

QString sdk::currency::to_full(QString m_short)
{
    for (const auto &it : metadata())
        if (it._short.compare(m_short, Qt::CaseInsensitive) == 0)
            return it._full;
    return "NaN";
}

sdk::Currency sdk::currency::to_enum(QString currency)
{
    for (const auto &it : metadata())
        if (it._full.compare(currency, Qt::CaseInsensitive) == 0 ||
            it._short.compare(currency, Qt::CaseInsensitive) == 0)
            return it._enum;
    return Currency::None;
}

sdk::Currency sdk::currency::from_short(QString currency)
{
    for (const auto &it : metadata())
        if (it._short.compare(currency, Qt::CaseInsensitive) == 0)
            return it._enum;

    return to_enum(currency);
}

QStringList sdk::currency::all_short()
{
    QStringList ret;
    for (const auto &it : metadata())
        ret.push_back(it._short);
    return ret;
}

const std::vector <sdk::currency::Meta>& sdk::currency::metadata()
{
    static std::vector <sdk::currency::Meta> _;
    if (not _.empty())
        return _;

    // Africa
    _.emplace_back(Currency::Algerian_Dinar,       "Algerian Dinar",       "DZD");
    _.emplace_back(Currency::Angolan_Kwanza,       "Angolan Kwanza",       "AOA");
    _.emplace_back(Currency::Basotho_Loti,         "Basotho Loti",         "LSL");
    _.emplace_back(Currency::Botswana_Pula,        "Botswana Pula",        "BWP");
    _.emplace_back(Currency::Burundian_Franc,      "Burundian Franc",      "BIF");
    _.emplace_back(Currency::Cape_Verdean_Escudo,  "Cape Verdean Escudo",  "CVE");
    _.emplace_back(Currency::Central_African,      "Central African",      "XAF");
    _.emplace_back(Currency::CFA_Franc,            "CFA Franc",            "XOF");
    _.emplace_back(Currency::Comorian_Franc,       "Comorian Franc",       "KMF");
    _.emplace_back(Currency::Congolese_Franc,      "Congolese Franc",      "CDF");
    _.emplace_back(Currency::Djiboutian_Franc,     "Djiboutian Franc",     "DJF");
    _.emplace_back(Currency::Egyptian_Pound,       "Egyptian Pound",       "EGP");
    _.emplace_back(Currency::Eritrean_Nakfa,       "Eritrean Nakfa",       "ERN");
    _.emplace_back(Currency::Ethiopian_Birr,       "Ethiopian Birr",       "ETB");
    _.emplace_back(Currency::Gambian_Dalasi,       "Gambian Dalasi",       "GMD");
    _.emplace_back(Currency::Ghanaian_Cedi,        "Ghanaian Cedi",        "GHS");
    _.emplace_back(Currency::Guinean_Franc,        "Guinean Franc",        "GNF");
    _.emplace_back(Currency::Kenyan_Shilling,      "Kenyan Shilling",      "KES");
    _.emplace_back(Currency::Liberian_Dollar,      "Liberian Dollar",      "LRD");
    _.emplace_back(Currency::Libyan_Dinar,         "Libyan Dinar",         "LYD");
    _.emplace_back(Currency::Malagasy_Ariary,      "Malagasy Ariary",      "MGA");
    _.emplace_back(Currency::Malawian_Kwacha,      "Malawian Kwacha",      "MWK");
    _.emplace_back(Currency::Mauritanian_Ouguiya,  "Mauritanian Ouguiya",  "MRO");
    _.emplace_back(Currency::Mauritian_Rupee,      "Mauritian Rupee",      "MUR");
    _.emplace_back(Currency::Moroccan_Dirham,      "Moroccan Dirham",      "MAD");
    _.emplace_back(Currency::Mozambican_Metical,   "Mozambican Metical",   "MZN");
    _.emplace_back(Currency::Nigerian_Naira ,      "Nigerian Naira",       "NGN");
    _.emplace_back(Currency::Namibian_Dollar,      "Namibian Dollar",      "NAD");
    _.emplace_back(Currency::Rwandan_Franc,        "Rwandan Franc",        "RWF");
    _.emplace_back(Currency::Saint_Helenian_Pound, "Saint Helenian Pound", "SHP");
    _.emplace_back(Currency::Sao_Tomean_Dobra,     "Sao Tomean Dobra",     "STD");
    _.emplace_back(Currency::Seychellois_Rupee,    "Seychellois Rupee",    "SCR");
    _.emplace_back(Currency::Sierra_Leonean_Leone, "Sierra Leonean Leone", "SLL");
    _.emplace_back(Currency::Somali_Shilling,      "Somali Shilling",      "SOS");
    _.emplace_back(Currency::South_African_Rand,   "South African Rand",   "ZAR");
    _.emplace_back(Currency::Sudanese_Pound,       "Sudanese Pound",       "SDG");
    _.emplace_back(Currency::Swazi_Lilangeni,      "Swazi Lilangeni",      "SZL");
    _.emplace_back(Currency::Tanzanian_Shilling,   "Tanzanian Shilling",   "TZS");
    _.emplace_back(Currency::Tunisian_Dinar,       "Tunisian Dinar",       "TND");
    _.emplace_back(Currency::Ugandan_Shilling,     "Ugandan Shilling",     "UGX");
    _.emplace_back(Currency::Zambian_Kwacha,       "Zambian Kwacha",       "ZMW");
    _.emplace_back(Currency::Zimbabwean_Dollar,    "Zimbabwean Dollar",    "ZWL");

    // West Asia
    _.emplace_back(Currency::Afghan_Afghani,      "Afghan Afghani",      "AFN");
    _.emplace_back(Currency::Armenian_Dram,       "Armenian Dram",       "AMD");
    _.emplace_back(Currency::Azerbaijan_Manat,    "Azerbaijan Manat",    "AZN");
    _.emplace_back(Currency::Bahraini_Dinar,      "Bahraini Dinar",      "BHD");
    _.emplace_back(Currency::Emirati_Dirham,      "Emirati Dirham",      "AED");
    _.emplace_back(Currency::Georgian_Lari,       "Georgian Lari",       "GEL");
    _.emplace_back(Currency::Jordanian_Dinar,     "Jordanian Dinar",     "JOD");
    _.emplace_back(Currency::Iraqi_Dinar,         "Iraqi Dinar",         "IQD");
    _.emplace_back(Currency::Iranian_Rial,        "Iranian Rial",        "IRR");
    _.emplace_back(Currency::Israeli_Shekel,      "Israeli Shekel",      "ILS");
    _.emplace_back(Currency::Kazakhstani_Tenge,   "Kazakhstani Tenge",   "KZT");
    _.emplace_back(Currency::Kuwaiti_Dinar,       "Kuwaiti Dinar",       "KWD");
    _.emplace_back(Currency::Lebanese_Pound,      "Lebanese Pound",      "LBP");
    _.emplace_back(Currency::Maldivian_Rufiyaa,   "Maldivian Rufiyaa",   "MVR");
    _.emplace_back(Currency::Omani_Rial,          "Omani Rial",          "OMR");
    _.emplace_back(Currency::Pakistani_Rupee,     "Pakistani Rupee",     "PKR");
    _.emplace_back(Currency::Qatari_Riyal,        "Qatari Riyal",        "QAR");
    _.emplace_back(Currency::Russian_Ruble,       "Russian Ruble",       "RUB");
    _.emplace_back(Currency::Saudi_Arabian_Riyal, "Saudi Arabian Riyal", "SAR");
    _.emplace_back(Currency::Syrian_Pound,        "Syrian Pound",        "SYP");
    _.emplace_back(Currency::Tajikistani_Somoni,  "Tajikistani Somoni",  "TJS");
    _.emplace_back(Currency::Turkish_Lira,        "Turkish Lira",        "TRY");
    _.emplace_back(Currency::Turkmenistani_Manat, "Turkmenistani Manat", "TMT");
    _.emplace_back(Currency::Uzbekistani_Som,     "Uzbekistani Som",     "UZS");
    _.emplace_back(Currency::Yemeni_Rial,         "Yemeni Rial",         "YER");

    // East Asia
    _.emplace_back(Currency::Bangladeshi_Taka,  "Bangladeshi Taka",   "BDT");
    _.emplace_back(Currency::Bhutanese_Ngultrum,"Bhutanese Ngultrum", "BTN");
    _.emplace_back(Currency::Bruneian_Dollar,   "Bruneian Dollar",    "BND");
    _.emplace_back(Currency::Burmese_Kyat,      "Burmese Kyat",       "MMK");
    _.emplace_back(Currency::Cambodian_Riel,    "Cambodian Riel",     "KHR");
    _.emplace_back(Currency::Chinese_Yuan,      "Chinese Yuan",       "CNY");
    _.emplace_back(Currency::Hong_Kong_Dollar,  "Hong Kong Dollar",   "HKD");
    _.emplace_back(Currency::Japanese_Yen,      "Japanese Yen",       "JPY");
    _.emplace_back(Currency::Indian_Rupee,      "Indian Rupee",       "INR");
    _.emplace_back(Currency::Indonesian_Rupiah, "Indonesian Rupiah",  "IDR");
    _.emplace_back(Currency::Kyrgyzstani_Som,   "Kyrgyzstani Som",    "KGS");
    _.emplace_back(Currency::Lao_Kip,           "Lao Kip",            "LAK");
    _.emplace_back(Currency::Macau_Pataca,      "Macau Pataca",       "MOP");
    _.emplace_back(Currency::Malaysian_Ringgit, "Malaysian Ringgit",  "MYR");
    _.emplace_back(Currency::Mongolian_Tughrik, "Mongolian Tughrik",  "MNT");
    _.emplace_back(Currency::Nepalese_Rupee,    "Nepalese Rupee",     "NPR");
    _.emplace_back(Currency::North_Korean_Won,  "North Korean Won",   "KPW");
    _.emplace_back(Currency::Philippine_Piso,   "Philippine Piso",    "PHP");
    _.emplace_back(Currency::Singapore_Dollar,  "Singapore Dollar",   "SGD");
    _.emplace_back(Currency::South_Korean_Won,  "South Korean Won",   "KRW");
    _.emplace_back(Currency::Sri_Lankan_Rupee,  "Sri Lankan Rupee",   "LKR");
    _.emplace_back(Currency::Taiwan_New_Dollar, "Taiwan New Dollar",  "TWD");
    _.emplace_back(Currency::Thai_Baht,         "Thai Baht",          "THB");
    _.emplace_back(Currency::Vietnamese_Dong,   "Vietnamese Dong",    "VND");
    _.emplace_back(Currency::Korean_Won,        "Korean Won",         "KRW");

    // Europe
    _.emplace_back(Currency::Albanian_Lek,              "Albanian Lek",              "ALL");
    _.emplace_back(Currency::Belarusian_Ruble,          "Belarusian Ruble",          "BYR");
    _.emplace_back(Currency::Bosnian_Convertible_Marka, "Bosnian Convertible Marka", "BAM");
    _.emplace_back(Currency::British_Pound,             "British Pound",             "GBP");
    _.emplace_back(Currency::GreatBritain_Pence,        "British Pence",             "GBX");
    _.emplace_back(Currency::Bulgarian_Lev,             "Bulgarian Lev",             "BGN");
    _.emplace_back(Currency::Croatian_Kuna,             "Croatian Kuna",             "HRK");
    _.emplace_back(Currency::Czech_Koruna,              "Czech Koruna",              "CZK");
    _.emplace_back(Currency::Danish_Krone,              "Danish Krone",              "DKK");
    _.emplace_back(Currency::Dutch_Guilder,             "Dutch Guilder",             "ANG");
    _.emplace_back(Currency::Euro,                      "Euro",                      "EUR");
    _.emplace_back(Currency::Gibraltar_Pound,           "Gibraltar Pound",           "GIP");
    _.emplace_back(Currency::Guernsey_Pound,            "Guernsey Pound",            "GGP");
    _.emplace_back(Currency::Hungarian_Forint,          "Hungarian Forint",          "HUF");
    _.emplace_back(Currency::Icelandic_Krona,           "Icelandic Krona",           "ISK");
    _.emplace_back(Currency::Isle_of_Man_Pound,         "Isle of Man Pound",         "IMP");
    _.emplace_back(Currency::Jersey_Pound,              "Jersey Pound",              "JEP");
    _.emplace_back(Currency::Macedonian_Denar,          "Macedonian Denar",          "MKD");
    _.emplace_back(Currency::Moldovan_Leu,              "Moldovan Leu",              "MDL");
    _.emplace_back(Currency::New_Belarusian_Ruble,      "New Belarusian Ruble",      "BYN");
    _.emplace_back(Currency::Norwegian_Krone,           "Norwegian Krone",           "NOK");
    _.emplace_back(Currency::Polish_Zloty,              "Polish Zloty",              "PLN");
    _.emplace_back(Currency::Romanian_Leu,              "Romanian Leu",              "RON");
    _.emplace_back(Currency::Serbian_Dinar,             "Serbian Dinar",             "RSD");
    _.emplace_back(Currency::Swedish_Krona,             "Swedish Krona",             "SEK");
    _.emplace_back(Currency::Swiss_Franc,               "Swiss Franc",               "CHF");
    _.emplace_back(Currency::Ukrainian_Hryvnia,         "Ukrainian Hryvnia",         "UAH");

    // North America
    _.emplace_back(Currency::Aruban_or_Dutch_Guilder,   "Aruban or Dutch Guilder",   "AWG");
    _.emplace_back(Currency::Bahamian_Dollar,           "Bahamian Dollar",           "BSD");
    _.emplace_back(Currency::Barbadian_or_Bajan_Dollar, "Barbadian or Bajan Dollar", "BBD");
    _.emplace_back(Currency::Belizean_Dollar,           "Belizean Dollar",           "BZD");
    _.emplace_back(Currency::Bermudian_Dollar,          "Bermudian Dollar",          "BMD");
    _.emplace_back(Currency::Canadian_Dollar,           "Canadian Dollar",           "CAD");
    _.emplace_back(Currency::Caymanian_Dollar,          "Caymanian Dollar",          "KYD");
    _.emplace_back(Currency::Costa_Rican_Colon,         "Costa Rican Colon",         "CRC");
    _.emplace_back(Currency::Cuban_Convertible_Peso,    "Cuban Convertible Peso",    "CUC");
    _.emplace_back(Currency::Cuban_Peso,                "Cuban Peso",                "CUP");
    _.emplace_back(Currency::Dominican_Peso,            "Dominican Peso",            "DOP");
    _.emplace_back(Currency::East_Caribbean_Dollar,     "East Caribbean Dollar",     "XCD");
    _.emplace_back(Currency::Guatemalan_Quetzal,        "Guatemalan Quetzal",        "GTQ");
    _.emplace_back(Currency::Haitian_Gourde,            "Haitian Gourde",            "HTG");
    _.emplace_back(Currency::Honduran_Lempira,          "Honduran Lempira",          "HNL");
    _.emplace_back(Currency::Jamaican_Dollar,           "Jamaican Dollar",           "JMD");
    _.emplace_back(Currency::Mexican_Peso,              "Mexican Peso",              "MXN");
    _.emplace_back(Currency::Nicaraguan_Cordoba,        "Nicaraguan Cordoba",        "NIO");
    _.emplace_back(Currency::Panamanian_Balboa,         "Panamanian Balboa",         "PAB");
    _.emplace_back(Currency::Salvadoran_Colon,          "Salvadoran Colon",          "SVC");
    _.emplace_back(Currency::Trinidadian_Dollar,        "Trinidadian Dollar",        "TTD");
    _.emplace_back(Currency::US_Dollar,                 "US Dollar",                 "USD");

    // South America
    _.emplace_back(Currency::Argentine_Peso,        "Argentine Peso",        "ARS");
    _.emplace_back(Currency::Bolivian_Boliviano,    "Bolivian Bolíviano",    "BOB");
    _.emplace_back(Currency::Brazilian_Real,        "Brazilian Real",        "BRL");
    _.emplace_back(Currency::Chilean_Peso,          "Chilean Peso",          "CLP");
    _.emplace_back(Currency::Colombian_Peso,        "Colombian Peso",        "COP");
    _.emplace_back(Currency::Falkland_Island_Pound, "Falkland Island Pound", "FKP");
    _.emplace_back(Currency::Guyanese_Dollar,       "Guyanese Dollar",       "GYD");
    _.emplace_back(Currency::Paraguayan_Guarani,    "Paraguayan Guarani",    "PYG");
    _.emplace_back(Currency::Peruvian_Sol,          "Peruvian Sol",          "PEN");
    _.emplace_back(Currency::Surinamese_Dollar,     "Surinamese Dollar",     "SRD");
    _.emplace_back(Currency::Venezuelan_Bolivar,    "Venezuelan Bolivar",    "VEF");
    _.emplace_back(Currency::Uruguayan_Peso,        "Uruguayan Peso",        "UYU");

    // Oceania
    _.emplace_back(Currency::Australian_Dollar,       "Australian Dollar",       "AUD");
    _.emplace_back(Currency::CFP_Franc,               "CFP Franc",               "XPF");
    _.emplace_back(Currency::Fijian_Dollar,           "Fijian Dollar",           "FJD");
    _.emplace_back(Currency::New_Zealand_Dollar,      "New Zealand Dollar",      "NZD");
    _.emplace_back(Currency::Ni_Vanuatu_Vatu,         "Ni-Vanuatu Vatu",         "VUV");
    _.emplace_back(Currency::Papua_New_Guinean_Kina,  "Papua New Guinean Kina",  "PGK");
    _.emplace_back(Currency::Samoan_Tala,             "Samoan Tala",             "WST");
    _.emplace_back(Currency::Solomon_Islander_Dollar, "Solomon Islander Dollar", "SBD");
    _.emplace_back(Currency::Tongan_Paanga,           "Tongan Pa\'anga",         "TOP");

    // Other
    _.emplace_back(Currency::Bitcoin,         "Bitcoin",         "BTC");
    _.emplace_back(Currency::IMF_Special,     "IMF Special",     "XDR");
    _.emplace_back(Currency::Gold_Ounce,      "Gold Ounce",      "XAU");
    _.emplace_back(Currency::Platinum_Ounce,  "Platinum Ounce",  "XPT");
    _.emplace_back(Currency::Palladium_Ounce, "Palladium Ounce", "XPD");
    _.emplace_back(Currency::Silver_Ounce,    "Silver Ounce",    "XAG");

    return _;
}
