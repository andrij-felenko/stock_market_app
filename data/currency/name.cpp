#include "name.h"

using namespace currency;

Name &Name::instance()
{
    static Name ins;
    return ins;
}

QString Name::to_short(Tag type)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._enum == type)
            return it._short;
    return "NaN";
}

QString Name::to_short(QString full)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._full == full || it._short == full)
            return it._short;
    return "NaN";
}

QString Name::to_full(Tag type)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._enum == type)
            return it._full;
    return "NaN";
}

QString Name::to_full(QString m_short)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._short == m_short)
            return it._full;
    return "NaN";
}

Tag Name::to_enum(QString currency)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._full == currency || it._short == currency)
            return it._enum;
    return Tag::None;
}

Tag Name::from_short(QString currency)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._short == currency)
            return it._enum;
    return Tag::None;
}

Continent Name::continent(QString name)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._full == name || it._short == name)
            return it._continent;
    return Continent::None;
}

Continent Name::continent(Tag type)
{
    const auto &list = instance()._list;
    for (const auto &it : list)
        if (it._enum == type)
            return it._continent;
    return Continent::None;
}

QStringList Name::allShort()
{
    QStringList ret;
    const auto &list = instance()._list;
    for (const auto &it : list)
        ret.push_back(it._short);
    return ret;
}

void Name::_add_africa(){
    Continent cont = Continent::Africa;
    _add(Tag::Algerian_Dinar,       "Algerian Dinar",       "DZD", cont);
    _add(Tag::Angolan_Kwanza,       "Angolan Kwanza",       "AOA", cont);
    _add(Tag::Basotho_Loti,         "Basotho Loti",         "LSL", cont);
    _add(Tag::Botswana_Pula,        "Botswana Pula",        "BWP", cont);
    _add(Tag::Burundian_Franc,      "Burundian Franc",      "BIF", cont);
    _add(Tag::Cape_Verdean_Escudo,  "Cape Verdean Escudo",  "CVE", cont);
    _add(Tag::Central_African,      "Central African",      "XAF", cont);
    _add(Tag::CFA_Franc,            "CFA Franc",            "XOF", cont);
    _add(Tag::Comorian_Franc,       "Comorian Franc",       "KMF", cont);
    _add(Tag::Congolese_Franc,      "Congolese Franc",      "CDF", cont);
    _add(Tag::Djiboutian_Franc,     "Djiboutian Franc",     "DJF", cont);
    _add(Tag::Egyptian_Pound,       "Egyptian Pound",       "EGP", cont);
    _add(Tag::Eritrean_Nakfa,       "Eritrean Nakfa",       "ERN", cont);
    _add(Tag::Ethiopian_Birr,       "Ethiopian Birr",       "ETB", cont);
    _add(Tag::Gambian_Dalasi,       "Gambian Dalasi",       "GMD", cont);
    _add(Tag::Ghanaian_Cedi,        "Ghanaian Cedi",        "GHS", cont);
    _add(Tag::Guinean_Franc,        "Guinean Franc",        "GNF", cont);
    _add(Tag::Kenyan_Shilling,      "Kenyan Shilling",      "KES", cont);
    _add(Tag::Liberian_Dollar,      "Liberian Dollar",      "LRD", cont);
    _add(Tag::Libyan_Dinar,         "Libyan Dinar",         "LYD", cont);
    _add(Tag::Malagasy_Ariary,      "Malagasy Ariary",      "MGA", cont);
    _add(Tag::Malawian_Kwacha,      "Malawian Kwacha",      "MWK", cont);
    _add(Tag::Mauritanian_Ouguiya,  "Mauritanian Ouguiya",  "MRO", cont);
    _add(Tag::Mauritian_Rupee,      "Mauritian Rupee",      "MUR", cont);
    _add(Tag::Moroccan_Dirham,      "Moroccan Dirham",      "MAD", cont);
    _add(Tag::Mozambican_Metical,   "Mozambican Metical",   "MZN", cont);
    _add(Tag::Nigerian_Naira ,      "Nigerian Naira",       "NGN", cont);
    _add(Tag::Namibian_Dollar,      "Namibian Dollar",      "NAD", cont);
    _add(Tag::Rwandan_Franc,        "Rwandan Franc",        "RWF", cont);
    _add(Tag::Saint_Helenian_Pound, "Saint Helenian Pound", "SHP", cont);
    _add(Tag::Sao_Tomean_Dobra,     "Sao Tomean Dobra",     "STD", cont);
    _add(Tag::Seychellois_Rupee,    "Seychellois Rupee",    "SCR", cont);
    _add(Tag::Sierra_Leonean_Leone, "Sierra Leonean Leone", "SLL", cont);
    _add(Tag::Somali_Shilling,      "Somali Shilling",      "SOS", cont);
    _add(Tag::South_African_Rand,   "South African Rand",   "ZAR", cont);
    _add(Tag::Sudanese_Pound,       "Sudanese Pound",       "SDG", cont);
    _add(Tag::Swazi_Lilangeni,      "Swazi Lilangeni",      "SZL", cont);
    _add(Tag::Tanzanian_Shilling,   "Tanzanian Shilling",   "TZS", cont);
    _add(Tag::Tunisian_Dinar,       "Tunisian Dinar",       "TND", cont);
    _add(Tag::Ugandan_Shilling,     "Ugandan Shilling",     "UGX", cont);
    _add(Tag::Zambian_Kwacha,       "Zambian Kwacha",       "ZMW", cont);
    _add(Tag::Zimbabwean_Dollar,    "Zimbabwean Dollar",    "ZWL", cont);
}

void Name::_add_east_asia(){
    Continent cont = Continent::East_Asia;
    _add(Tag::Bangladeshi_Taka,  "Bangladeshi Taka",   "BDT", cont);
    _add(Tag::Bhutanese_Ngultrum,"Bhutanese Ngultrum", "BTN", cont);
    _add(Tag::Bruneian_Dollar,   "Bruneian Dollar",    "BND", cont);
    _add(Tag::Burmese_Kyat,      "Burmese Kyat",       "MMK", cont);
    _add(Tag::Cambodian_Riel,    "Cambodian Riel",     "KHR", cont);
    _add(Tag::Chinese_Yuan,      "Chinese Yuan",       "CNY", cont);
    _add(Tag::Hong_Kong_Dollar,  "Hong Kong Dollar",   "HKD", cont);
    _add(Tag::Japanese_Yen,      "Japanese Yen",       "JPY", cont);
    _add(Tag::Indian_Rupee,      "Indian Rupee",       "INR", cont);
    _add(Tag::Indonesian_Rupiah, "Indonesian Rupiah",  "IDR", cont);
    _add(Tag::Kyrgyzstani_Som,   "Kyrgyzstani Som",    "KGS", cont);
    _add(Tag::Lao_Kip,           "Lao Kip",            "LAK", cont);
    _add(Tag::Macau_Pataca,      "Macau Pataca",       "MOP", cont);
    _add(Tag::Malaysian_Ringgit, "Malaysian Ringgit",  "MYR", cont);
    _add(Tag::Mongolian_Tughrik, "Mongolian Tughrik",  "MNT", cont);
    _add(Tag::Nepalese_Rupee,    "Nepalese Rupee",     "NPR", cont);
    _add(Tag::North_Korean_Won,  "North Korean Won",   "KPW", cont);
    _add(Tag::Philippine_Piso,   "Philippine Piso",    "PHP", cont);
    _add(Tag::Singapore_Dollar,  "Singapore Dollar",   "SGD", cont);
    _add(Tag::South_Korean_Won,  "South Korean Won",   "KRW", cont);
    _add(Tag::Sri_Lankan_Rupee,  "Sri Lankan Rupee",   "LKR", cont);
    _add(Tag::Taiwan_New_Dollar, "Taiwan New Dollar",  "TWD", cont);
    _add(Tag::Thai_Baht,         "Thai Baht",          "THB", cont);
    _add(Tag::Vietnamese_Dong,   "Vietnamese Dong",    "VND", cont);
}

void Name::_add_west_asia(){
    Continent cont = Continent::West_Asia;
    _add(Tag::Afghan_Afghani,      "Afghan Afghani",      "AFN", cont);
    _add(Tag::Armenian_Dram,       "Armenian Dram",       "AMD", cont);
    _add(Tag::Azerbaijan_Manat,    "Azerbaijan Manat",    "AZN", cont);
    _add(Tag::Bahraini_Dinar,      "Bahraini Dinar",      "BHD", cont);
    _add(Tag::Emirati_Dirham,      "Emirati Dirham",      "AED", cont);
    _add(Tag::Georgian_Lari,       "Georgian Lari",       "GEL", cont);
    _add(Tag::Jordanian_Dinar,     "Jordanian Dinar",     "JOD", cont);
    _add(Tag::Iraqi_Dinar,         "Iraqi Dinar",         "IQD", cont);
    _add(Tag::Iranian_Rial,        "Iranian Rial",        "IRR", cont);
    _add(Tag::Israeli_Shekel,      "Israeli Shekel",      "ILS", cont);
    _add(Tag::Kazakhstani_Tenge,   "Kazakhstani Tenge",   "KZT", cont);
    _add(Tag::Kuwaiti_Dinar,       "Kuwaiti Dinar",       "KWD", cont);
    _add(Tag::Lebanese_Pound,      "Lebanese Pound",      "LBP", cont);
    _add(Tag::Maldivian_Rufiyaa,   "Maldivian Rufiyaa",   "MVR", cont);
    _add(Tag::Omani_Rial,          "Omani Rial",          "OMR", cont);
    _add(Tag::Pakistani_Rupee,     "Pakistani Rupee",     "PKR", cont);
    _add(Tag::Qatari_Riyal,        "Qatari Riyal",        "QAR", cont);
    _add(Tag::Russian_Ruble,       "Russian Ruble",       "RUB", cont);
    _add(Tag::Saudi_Arabian_Riyal, "Saudi Arabian Riyal", "SAR", cont);
    _add(Tag::Syrian_Pound,        "Syrian Pound",        "SYP", cont);
    _add(Tag::Tajikistani_Somoni,  "Tajikistani Somoni",  "TJS", cont);
    _add(Tag::Turkish_Lira,        "Turkish Lira",        "TRY", cont);
    _add(Tag::Turkmenistani_Manat, "Turkmenistani Manat", "TMT", cont);
    _add(Tag::Uzbekistani_Som,     "Uzbekistani Som",     "UZS", cont);
    _add(Tag::Yemeni_Rial,         "Yemeni Rial",         "YER", cont);
}

void Name::_add_europe()
{
    Continent cont = Continent::Europe;
    _add(Tag::Albanian_Lek,              "Albanian Lek",              "ALL", cont);
    _add(Tag::Belarusian_Ruble,          "Belarusian Ruble",          "BYR", cont);
    _add(Tag::Bosnian_Convertible_Marka, "Bosnian Convertible Marka", "BAM", cont);
    _add(Tag::British_Pound,             "British Pound",             "GBP", cont);
    _add(Tag::Bulgarian_Lev,             "Bulgarian Lev",             "BGN", cont);
    _add(Tag::Croatian_Kuna,             "Croatian Kuna",             "HRK", cont);
    _add(Tag::Czech_Koruna,              "Czech Koruna",              "CZK", cont);
    _add(Tag::Danish_Krone,              "Danish Krone",              "DKK", cont);
    _add(Tag::Dutch_Guilder,             "Dutch Guilder",             "ANG", cont);
    _add(Tag::Euro,                      "Euro",                      "EUR", cont);
    _add(Tag::Gibraltar_Pound,           "Gibraltar Pound",           "GIP", cont);
    _add(Tag::Guernsey_Pound,            "Guernsey Pound",            "GGP", cont);
    _add(Tag::Hungarian_Forint,          "Hungarian Forint",          "HUF", cont);
    _add(Tag::Icelandic_Krona,           "Icelandic Krona",           "ISK", cont);
    _add(Tag::Isle_of_Man_Pound,         "Isle of Man Pound",         "IMP", cont);
    _add(Tag::Jersey_Pound,              "Jersey Pound",              "JEP", cont);
    _add(Tag::Macedonian_Denar,          "Macedonian Denar",          "MKD", cont);
    _add(Tag::Moldovan_Leu,              "Moldovan Leu",              "MDL", cont);
    _add(Tag::New_Belarusian_Ruble,      "New Belarusian Ruble",      "BYN", cont);
    _add(Tag::Norwegian_Krone,           "Norwegian Krone",           "NOK", cont);
    _add(Tag::Polish_Zloty,              "Polish Zloty",              "PLN", cont);
    _add(Tag::Romanian_Leu,              "Romanian Leu",              "RON", cont);
    _add(Tag::Serbian_Dinar,             "Serbian Dinar",             "RSD", cont);
    _add(Tag::Swedish_Krona,             "Swedish Krona",             "SEK", cont);
    _add(Tag::Swiss_Franc,               "Swiss Franc",               "CHF", cont);
    _add(Tag::Ukrainian_Hryvnia,         "Ukrainian Hryvnia",         "UAH", cont);
}

void Name::_add_north_america()
{
    Continent cont = Continent::North_America;
    _add(Tag::Aruban_or_Dutch_Guilder,   "Aruban or Dutch Guilder",   "AWG", cont);
    _add(Tag::Bahamian_Dollar,           "Bahamian Dollar",           "BSD", cont);
    _add(Tag::Barbadian_or_Bajan_Dollar, "Barbadian or Bajan Dollar", "BBD", cont);
    _add(Tag::Belizean_Dollar,           "Belizean Dollar",           "BZD", cont);
    _add(Tag::Bermudian_Dollar,          "Bermudian Dollar",          "BMD", cont);
    _add(Tag::Canadian_Dollar,           "Canadian Dollar",           "CAD", cont);
    _add(Tag::Caymanian_Dollar,          "Caymanian Dollar",          "KYD", cont);
    _add(Tag::Costa_Rican_Colon,         "Costa Rican Colon",         "CRC", cont);
    _add(Tag::Cuban_Convertible_Peso,    "Cuban Convertible Peso",    "CUC", cont);
    _add(Tag::Cuban_Peso,                "Cuban Peso",                "CUP", cont);
    _add(Tag::Dominican_Peso,            "Dominican Peso",            "DOP", cont);
    _add(Tag::East_Caribbean_Dollar,     "East Caribbean Dollar",     "XCD", cont);
    _add(Tag::Guatemalan_Quetzal,        "Guatemalan Quetzal",        "GTQ", cont);
    _add(Tag::Haitian_Gourde,            "Haitian Gourde",            "HTG", cont);
    _add(Tag::Honduran_Lempira,          "Honduran Lempira",          "HNL", cont);
    _add(Tag::Jamaican_Dollar,           "Jamaican Dollar",           "JMD", cont);
    _add(Tag::Mexican_Peso,              "Mexican Peso",              "MXN", cont);
    _add(Tag::Nicaraguan_Cordoba,        "Nicaraguan Cordoba",        "NIO", cont);
    _add(Tag::Panamanian_Balboa,         "Panamanian Balboa",         "PAB", cont);
    _add(Tag::Salvadoran_Colon,          "Salvadoran Colon",          "SVC", cont);
    _add(Tag::Trinidadian_Dollar,        "Trinidadian Dollar",        "TTD", cont);
    _add(Tag::US_Dollar,                 "US Dollar",                 "USD", cont);
}

void Name::_add_oceania()
{
    Continent cont = Continent::Oceania;
    _add(Tag::Australian_Dollar,       "Australian Dollar",       "AUD", cont);
    _add(Tag::CFP_Franc,               "CFP Franc",               "XPF", cont);
    _add(Tag::Fijian_Dollar,           "Fijian Dollar",           "FJD", cont);
    _add(Tag::New_Zealand_Dollar,      "New Zealand Dollar",      "NZD", cont);
    _add(Tag::Ni_Vanuatu_Vatu,         "Ni-Vanuatu Vatu",         "VUV", cont);
    _add(Tag::Papua_New_Guinean_Kina,  "Papua New Guinean Kina",  "PGK", cont);
    _add(Tag::Samoan_Tala,             "Samoan Tala",             "WST", cont);
    _add(Tag::Solomon_Islander_Dollar, "Solomon Islander Dollar", "SBD", cont);
    _add(Tag::Tongan_Paanga,           "Tongan Pa\'anga",         "TOP", cont);
}

void Name::_add_south_america()
{
    Continent cont = Continent::South_America;
    _add(Tag::Argentine_Peso,        "Argentine Peso",        "ARS", cont);
    _add(Tag::Bolivian_Boliviano,    "Bolivian Bolíviano",    "BOB", cont);
    _add(Tag::Brazilian_Real,        "Brazilian Real",        "BRL", cont);
    _add(Tag::Chilean_Peso,          "Chilean Peso",          "CLP", cont);
    _add(Tag::Colombian_Peso,        "Colombian Peso",        "COP", cont);
    _add(Tag::Falkland_Island_Pound, "Falkland Island Pound", "FKP", cont);
    _add(Tag::Guyanese_Dollar,       "Guyanese Dollar",       "GYD", cont);
    _add(Tag::Paraguayan_Guarani,    "Paraguayan Guarani",    "PYG", cont);
    _add(Tag::Peruvian_Sol,          "Peruvian Sol",          "PEN", cont);
    _add(Tag::Surinamese_Dollar,     "Surinamese Dollar",     "SRD", cont);
    _add(Tag::Venezuelan_Bolivar,    "Venezuelan Bolivar",    "VEF", cont);
    _add(Tag::Uruguayan_Peso,        "Uruguayan Peso",        "UYU", cont);
}

Name::Name()
{
    _add_africa();
    _add_east_asia();
    _add_west_asia();
    _add_europe();
    _add_north_america();
    _add_oceania();
    _add_south_america();

    _add(Tag::Bitcoin,         "Bitcoin",         "BTC");
    _add(Tag::IMF_Special,     "IMF Special",     "XDR");
    _add(Tag::Gold_Ounce,      "Gold Ounce",      "XAU");
    _add(Tag::Platinum_Ounce,  "Platinum Ounce",  "XPT");
    _add(Tag::Palladium_Ounce, "Palladium Ounce", "XPD");
    _add(Tag::Silver_Ounce,    "Silver Ounce",    "XAG");
}

void Name::_add(Tag type, QString full, QString short_, Continent continent)
{
    TypeStruct temp;
    temp._enum = type;
    temp._full = full;
    temp._short = short_;
    temp._continent = continent;
    _list.push_back(temp);
}
