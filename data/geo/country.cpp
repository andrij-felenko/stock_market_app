#include "country.h"
#include <unordered_map>

namespace geo {
    struct CountryMeta {
        Country _enum;
        const char* alpha2;
        const char* alpha3;
        QStringList aliases;

        CountryMeta(Country e, const char* a2, const char* a3,
                    std::initializer_list<const char*> list) : _enum(e), alpha2(a2), alpha3(a3)
            {
                for (auto s : list)
                    aliases << QString::fromUtf8(s);
            }
    };
}

using gC = geo::Country;

// ------------------- STATIC MAP -------------------
static const std::unordered_map <geo::Country, geo::CountryMeta>& Meta() {
    static std::unordered_map <geo::Country, geo::CountryMeta> meta;
    if (not meta.empty())
        return meta;

    auto push = [&](geo::Country e, const char* a2, const char* a3, auto... aliases){
        meta.emplace(e, geo::CountryMeta(e, a2, a3, {aliases...}));
    };

    // ----------------------- Europe -------------------------------------------------------------
    push(gC::Ukraine, "UA", "UKR", "Ukraine", "Ukrayina");
    push(gC::Moldova, "MD", "MDA", "Moldova", "Republic of Moldova");
    push(gC::Romania, "RO", "ROU", "Romania");
    push(gC::Bulgaria, "BG", "BGR", "Bulgaria");

    push(gC::Poland, "PL", "POL", "Poland", "Rzeczpospolita Polska");
    push(gC::Czechia, "CZ", "CZE", "Czechia", "Czech Republic");
    push(gC::Slovakia, "SK", "SVK", "Slovakia","Slovak Republic");
    push(gC::Hungary, "HU", "HUN", "Hungary", "Magyarország");

    push(gC::Estonia,  "EE", "EST", "Estonia", "Eesti");
    push(gC::Latvia, "LV", "LVA", "Latvia", "Latvija");
    push(gC::Lithuania, "LT", "LTU", "Lithuania", "Lietuva");
    push(gC::Belarus, "BY", "BLR", "Belarus", "Byelorussia");

    push(gC::Androra, "AD", "AND", "Andorra", "Principality of Andorra");
    push(gC::Portugal, "PT", "PRT", "Portugal", "República Portuguesa");
    push(gC::Spain, "ES", "ESP", "Spain", "España");
    push(gC::Gibraltar, "GI", "GIB", "Gibraltar");

    push(gC::Malta, "MT", "MLT", "Malta", "Republic of Malta");
    push(gC::Cyprus, "CY", "CYP", "Cyprus", "Republic of Cyprus");

    push(gC::Greece, "GR", "GRC", "Greece", "Hellenic Republic", "Elláda");
    push(gC::Albania, "AL", "ALB", "Albania", "Shqipëria");
    push(gC::NorthMacedonia, "MK", "MKD", "North Macedonia","Republic of North Macedonia");
    push(gC::Serbia, "RS", "SRB", "Serbia", "Republic of Serbia");
    push(gC::BosniaAndHerzegovina, "BA", "BIH", "Bosnia and Herzegovina", "BiH");
    push(gC::Montenegro, "ME", "MNE", "Montenegro", "Crna Gora");
    push(gC::Kosovo, "XK", "XKX", "Kosovo", "Republic of Kosovo");

    push(gC::Italy, "IT", "ITA", "Italy", "Italia");
    push(gC::Croatia, "HR", "HRV", "Croatia", "Hrvatska");
    push(gC::Slovenia, "SI", "SVN", "Slovenia", "Republika Slovenija");
    push(gC::SanMarino, "SM", "SMR", "San Marino", "Republic of San Marino");
    push(gC::Monaco, "MC", "MCO", "Monaco", "Principality of Monaco");
    push(gC::Vatican, "VA", "VAT", "Vatican City", "Holy See");

    push(gC::Sweden, "SE", "SWE", "Sweden", "Sverige");
    push(gC::Norway, "NO", "NOR", "Norway", "Norge");
    push(gC::Finland, "FI", "FIN", "Finland", "Suomi");
    push(gC::Iceland, "IS", "ISL", "Iceland", "Ísland");
    push(gC::Denmark, "DK", "DNK", "Denmark", "Danmark");
    push(gC::Greenland, "GL", "GRL", "Greenland", "Kalaallit Nunaat");

    push(gC::Belgium, "BE", "BEL", "Belgium", "België", "Belgique", "Belgien");
    push(gC::Netherlands, "NL", "NLD", "Netherlands", "Nederland");
    push(gC::Luxembourg, "LU", "LUX", "Luxembourg", "Lëtzebuerg");

    push(gC::UnitedKingdom, "GB", "GBR", "United Kingdom","UK","Great Britain","Britain");
    push(gC::IsleOfMan, "IM", "IMN", "Isle of Man");
    push(gC::Jersey, "JE", "JEY", "Jersey");
    push(gC::Guernsey, "GG", "GGY", "Guernsey");
    push(gC::Ireland, "IE", "IRL", "Ireland", "Éire");

    push(gC::France, "FR", "FRA", "France", "République française");
    push(gC::Germany, "DE", "DEU", "Germany", "Deutschland");
    push(gC::Switzerland, "CH", "CHE", "Switzerland", "Schweiz", "Suisse", "Svizzera");
    push(gC::Austria, "AT", "AUT", "Austria", "Österreich");

    // ----------------------- Asia ---------------------------------------------------------------
    push(gC::SaudiArabia,        "SA", "SAU", "Saudi Arabia", "Kingdom of Saudi Arabia");
    push(gC::UnitedArabEmirates, "AE", "ARE", "UAE", "United Arab Emirates");
    push(gC::Oman,               "OM", "OMN", "Oman", "Sultanate of Oman");
    push(gC::Yemen,              "YE", "YEM", "Yemen", "Republic of Yemen");
    push(gC::Kuwait,             "KW", "KWT", "Kuwait", "State of Kuwait");
    push(gC::Qatar,              "QA", "QAT", "Qatar", "State of Qatar");
    push(gC::Bahrein,            "BH", "BHR", "Bahrain", "Kingdom of Bahrain");

    push(gC::Armenia,    "AM", "ARM", "Armenia", "Republic of Armenia");
    push(gC::Georgia,    "GE", "GEO", "Georgia", "Sakartvelo");
    push(gC::Azerbaijan, "AZ", "AZE", "Azerbaijan", "Republic of Azerbaijan");

    push(gC::Israel,    "IL", "ISR", "Israel", "State of Israel");
    push(gC::Lebanon,   "LB", "LBN", "Lebanon", "Lebanese Republic");
    push(gC::Syria,     "SY", "SYR", "Syria", "Syrian Arab Republic");
    push(gC::Iraq,      "IQ", "IRQ", "Iraq", "Republic of Iraq");
    push(gC::Jordan,    "JO", "JOR", "Jordan", "Hashemite Kingdom of Jordan");
    push(gC::Turkiye,   "TR", "TUR", "Türkiye", "Turkey", "Republic of Türkiye");
    push(gC::Palestine, "PS", "PSE", "Palestine","State of Palestine");

    push(gC::Kazakhstan,   "KZ", "KAZ", "Kazakhstan", "Republic of Kazakhstan");
    push(gC::Turkmenistan, "TM", "TKM", "Turkmenistan");
    push(gC::Kyrgyzstan,   "KG", "KGZ", "Kyrgyzstan", "Kyrgyz Republic");
    push(gC::Tajikistan,   "TJ", "TJK", "Tajikistan", "Republic of Tajikistan");
    push(gC::Uzbekistan,   "UZ", "UZB", "Uzbekistan", "Republic of Uzbekistan");

    push(gC::India,     "IN", "IND", "India", "Republic of India");
    push(gC::Nepal,     "NP", "NPL", "Nepal", "Federal Democratic Republic of Nepal");
    push(gC::Maldives,  "MV", "MDV", "Maldives", "Republic of Maldives");
    push(gC::Bhutan,    "BT", "BTN", "Bhutan", "Kingdom of Bhutan");
    push(gC::Shrilanka, "LK", "LKA", "Sri Lanka",
                 "Democratic Socialist Republic of Sri Lanka");

    push(gC::Afghanistan, "AF", "AFG", "Afghanistan", "Islamic Republic of Afghanistan");
    push(gC::Iran,        "IR", "IRN", "Iran", "Islamic Republic of Iran", "Persia");
    push(gC::Pakistan,    "PK", "PAK", "Pakistan", "Islamic Republic of Pakistan");

    push(gC::Thailand, "TH", "THA", "Thailand", "Kingdom of Thailand");
    push(gC::Vietnam,  "VN", "VNM", "Vietnam", "Socialist Republic of Vietnam");
    push(gC::Cambodia, "KH", "KHM", "Cambodia", "Kingdom of Cambodia");
    push(gC::Laos,     "LA", "LAO", "Laos", "Lao PDR", "Lao People's Democratic Republic");
    push(gC::Myanmar,  "MM", "MMR", "Myanmar", "Burma","Republic of the Union of Myanmar");

    push(gC::Japan,      "JP", "JPN", "Japan", "Nippon", "State of Japan");
    push(gC::China,      "CN", "CHN", "China", "People's Republic of China", "PRC");
    push(gC::Taiwan,     "TW", "TWN", "Taiwan", "Republic of China", "ROC");
    push(gC::SouthKorea, "KR", "KOR", "South Korea", "Republic of Korea", "ROK");
    push(gC::HongKong,   "HK", "HKG", "Hong Kong", "Hong Kong SAR");
    push(gC::Mongolia,   "MN", "MNG", "Mongolia");
    push(gC::Macau,      "MO", "MAC", "Macau", "Macao SAR");
    push(gC::NorthKorea, "KP", "PRK",
                 "North Korea", "Democratic People's Republic of Korea", "DPRK");


    push(gC::Brunei,      "BN", "BRN", "Brunei", "Nation of Brunei, Abode of Peace");
    push(gC::Malaysia,    "MY", "MYS", "Malaysia");
    push(gC::Singapore,   "SG", "SGP", "Singapore", "Republic of Singapore");
    push(gC::Philippines, "PH", "PHL", "Philippines", "Republic of the Philippines");
    push(gC::Indonesia,   "ID", "IDN", "Indonesia", "Republic of Indonesia");
    push(gC::EastTimor,   "TL", "TLS",
                 "East Timor", "Timor-Leste", "Democratic Republic of Timor-Leste");

    // ----------------------- Africa -------------------------------------------------------------
    push(gC::Morocco, "MA", "MAR", "Morocco", "Kingdom of Morocco");
    push(gC::Algeria, "DZ", "DZA", "Algeria", "People's Democratic Republic of Algeria");
    push(gC::Tunisia, "TN", "TUN", "Tunisia", "Republic of Tunisia");
    push(gC::Libya,   "LY", "LBY", "Libya", "State of Libya");
    push(gC::WesternSahara, "EH", "ESH",
                 "Western Sahara", "Sahrawi Arab Democratic Republic");

    push(gC::Egypt,       "EG", "EGY", "Egypt", "Arab Republic of Egypt");
    push(gC::Sudan,       "SD", "SDN", "Sudan", "Republic of the Sudan");
    push(gC::SouthSudan,  "SS", "SSD", "South Sudan", "Republic of South Sudan");

    push(gC::Mauritania,  "MR", "MRT", "Mauritania", "Islamic Republic of Mauritania");
    push(gC::Mali,        "ML", "MLI", "Mali", "Republic of Mali");
    push(gC::Niger,       "NE", "NER", "Niger", "Republic of the Niger");
    push(gC::Chad,        "TD", "TCD", "Chad", "Republic of Chad");
    push(gC::BurkinaFaso, "BF", "BFA", "Burkina Faso");

    push(gC::Senegal,      "SN", "SEN", "Senegal", "Republic of Senegal");
    push(gC::Gambia,       "GM", "GMB", "Gambia", "The Gambia", "Republic of The Gambia");
    push(gC::Guinea,       "GN", "GIN", "Guinea", "Republic of Guinea");
    push(gC::GuineaBissau, "GW", "GNB", "Guinea-Bissau", "Republic of Guinea-Bissau");
    push(gC::SierraLeone,  "SL", "SLE", "Sierra Leone", "Republic of Sierra Leone");
    push(gC::Liberia,      "LR", "LBR", "Liberia", "Republic of Liberia");
    push(gC::Ghana,        "GH", "GHA", "Ghana", "Republic of Ghana");
    push(gC::Togo,         "TG", "TGO", "Togo", "Togolese Republic");
    push(gC::Benin,        "BJ", "BEN", "Benin", "Republic of Benin");
    push(gC::Nigeria,      "NG", "NGA""Nigeria", "Federal Republic of Nigeria");
    push(gC::IvoryCoast,   "CI","CIV", "Ivory Coast",
                 "Côte d'Ivoire", "Republic of Côte d'Ivoire");

    push(gC::Cameroon,               "CM", "CMR", "Cameroon", "Republic of Cameroon");
    push(gC::CentralAfricanRepublic, "CF", "CAF", "Central African Republic");
    push(gC::EquatorialGuinea,       "GQ", "GNQ",
                 "Equatorial Guinea", "Republic of Equatorial Guinea");
    push(gC::Gabon,                  "GA", "GAB", "Gabon", "Gabonese Republic");
    push(gC::RepublicOfCongo,        "CG", "COG",
                 "Republic of the Congo", "Congo-Brazzaville");
    push(gC::DR_Congo,               "CD", "COD",
                 "DR Congo", "Democratic Republic of the Congo", "Congo-Kinshasa");

    push(gC::Ethiopia, "ET", "ETH", "Ethiopia", "Federal Democratic Republic of Ethiopia");
    push(gC::Eritrea,  "ER", "ERI", "Eritrea", "State of Eritrea");
    push(gC::Djibouti, "DJ", "DJI", "Djibouti", "Republic of Djibouti");
    push(gC::Somalia,  "SO", "SOM", "Somalia", "Federal Republic of Somalia");

    push(gC::Uganda,   "UG", "UGA", "Uganda", "Republic of Uganda");
    push(gC::Rwanda,   "RW", "RWA", "Rwanda", "Republic of Rwanda");
    push(gC::Burundi,  "BI", "BDI", "Burundi", "Republic of Burundi");
    push(gC::Tanzania, "TZ", "TZA", "Tanzania", "United Republic of Tanzania");
    push(gC::Kenya,    "KE", "KEN", "Kenya", "Republic of Kenya");

    push(gC::Angola,      "AO", "AGO", "Angola", "Republic of Angola");
    push(gC::Zambia,      "ZM", "ZMB", "Zambia", "Republic of Zambia");
    push(gC::Zimbabwe,    "ZW", "ZWE", "Zimbabwe", "Republic of Zimbabwe");
    push(gC::Malawi,      "MW", "MWI", "Malawi", "Republic of Malawi");
    push(gC::Mozambique,  "MZ", "MOZ", "Mozambique", "Republic of Mozambique");
    push(gC::Namibia,     "NA", "NAM", "Namibia", "Republic of Namibia");
    push(gC::Botswana,    "BW", "BWA", "Botswana", "Republic of Botswana");
    push(gC::SouthAfrica, "ZA", "ZAF", "South Africa", "Republic of South Africa", "RSA");
    push(gC::Lesotho,     "LS", "LSO", "Lesotho", "Kingdom of Lesotho");
    push(gC::Eswatini,    "SZ", "SWZ", "Eswatini", "Swaziland", "Kingdom of Eswatini");

    push(gC::Madagascar, "MG", "MDG", "Madagascar", "Republic of Madagascar");
    push(gC::Comoros,    "KM", "COM", "Comoros", "Union of the Comoros");
    push(gC::Seychelles, "SC", "SYC", "Seychelles", "Republic of Seychelles");
    push(gC::Mauritius,  "MU", "MUS", "Mauritius", "Republic of Mauritius");

    // ----------------------- America ------------------------------------------------------------
    push(gC::UnitedStates, "US", "USA", "United States", "USA","United States of America");
    push(gC::Canada,       "CA", "CAN", "Canada");

    push(gC::Mexico,     "MX", "MEX", "Mexico", "Estados Unidos Mexicanos");
    push(gC::Panama,     "PA", "PAN", "Panama", "Republic of Panama");
    push(gC::Nicaragua,  "NI", "NIC", "Nicaragua", "Republic of Nicaragua");
    push(gC::Honduras,   "HN", "HND", "Honduras", "Republic of Honduras");
    push(gC::Guatemala,  "GT", "GTM", "Guatemala", "Republic of Guatemala");
    push(gC::ElSalvador, "SV", "SLV", "El Salvador", "Republic of El Salvador");
    push(gC::Belize,     "BZ", "BLZ", "Belize");

    push(gC::Cuba,              "CU", "CUB", "Cuba", "Republic of Cuba");
    push(gC::CostaRica,         "CR", "CRI", "Costa Rica", "Republic of Costa Rica");
    push(gC::Bermuda,           "BM", "BMU", "Bermuda"); // UK territory
    push(gC::Haiti,             "HT", "HTI", "Haiti", "Republic of Haiti");
    push(gC::Barbados,          "BB", "BRB", "Barbados");
    push(gC::Grenada,           "GD", "GRD", "Grenada");
    push(gC::PuertoRico,        "PR", "PRI", "Puerto Rico"); // US territory
    push(gC::Jamaica,           "JM", "JAM", "Jamaica");
    push(gC::Bahamas,           "BS", "BHS", "Bahamas", "The Bahamas");
    push(gC::CaymanIslands,     "KY", "CYM", "Cayman Islands"); // UK territory
    push(gC::DominicanRepublic, "DO", "DOM", "Dominican Republic");

    push(gC::Brazil,    "BR", "BRA", "Brazil", "Federative Republic of Brazil");
    push(gC::Argentina, "AR", "ARG", "Argentina", "Argentine Republic");
    push(gC::Chile,     "CL", "CHL", "Chile", "Republic of Chile");
    push(gC::Colombia,  "CO", "COL", "Colombia", "Republic of Colombia");
    push(gC::Peru,      "PE", "PER", "Peru", "Republic of Peru");
    push(gC::Uruguay,   "UY", "URY", "Uruguay", "Oriental Republic of Uruguay");
    push(gC::Paraguay,  "PY", "PRY", "Paraguay", "Republic of Paraguay");
    push(gC::Bolivia,   "BO", "BOL", "Bolivia", "Plurinational State of Bolivia");
    push(gC::Guyana,    "GY", "GUY", "Guyana", "Co-operative Republic of Guyana");
    push(gC::Suriname,  "SR", "SUR", "Suriname", "Republic of Suriname");
    push(gC::Venezuela, "VE", "VEN", "Venezuela", "Bolivarian Republic of Venezuela");
    push(gC::Falklands, "FK", "FLK", "Falkland Islands", "Islas Malvinas"); // UK territory

    // ----------------------- Oceania ------------------------------------------------------------
    push(gC::Australia,  "AU", "AUS", "Australia", "Commonwealth of Australia");
    push(gC::NewZealand, "NZ", "NZL", "New Zealand", "Aotearoa");

    push(gC::PapuaNewGuinea, "PG", "PNG", "Papua New Guinea", "PNG");
    push(gC::Fiji,           "FJ", "FJI", "Fiji", "Republic of Fiji");
    push(gC::SolomonIslands, "SB", "SLB", "Solomon Islands");
    push(gC::Vanuatu,        "VU", "VUT", "Vanuatu", "Republic of Vanuatu");
    push(gC::NewCaledonia,   "NC", "NCL", "New Caledonia"); // FR territory

    push(gC::Palau,    "PW", "PLW", "Palau", "Republic of Palau");
    push(gC::Nauru,    "NR", "NRU", "Nauru", "Republic of Nauru");
    push(gC::Kiribati, "KI", "KIR", "Kiribati", "Republic of Kiribati");
    push(gC::Guam,     "GU", "GUM", "Guam");
    push(gC::FederatedStatesOfMicronesia, "FM", "FSM", "Micronesia", "FSM");
    push(gC::NorthernMarianaIslands,      "MP", "MNP", "Northern Mariana Islands");
    push(gC::MarshallIslands,             "MH", "MHL",
                 "Marshall Islands", "Republic of the Marshall Islands");

    push(gC::Samoa,           "WS", "WSM", "Samoa", "Independent State of Samoa");
    push(gC::Tonga,           "TO", "TON", "Tonga", "Kingdom of Tonga");
    push(gC::Tuvalu,          "TV", "TUV", "Tuvalu");
    push(gC::CookIslands,     "CK", "COK", "Cook Islands"); // NZ assoc.
    push(gC::Niue,            "NU", "NIU", "Niue"); // NZ assoc.
    push(gC::FrenchPolynesia, "PF", "PYF", "French Polynesia"); // FR territory
    push(gC::WallisAndFutuna, "WF", "WLF", "Wallis and Futuna"); // FR territory
    push(gC::AmericanSamoa,   "AS", "ASM", "American Samoa"); // US territory
    push(gC::Tokelau,         "TK", "TKL", "Tokelau"); // NZ territory

    return meta;
};

using namespace geo;

QString geo::CountryName::primary(Country c) {
    auto it = Meta().find(c);
        return (it != Meta().end() && !it->second.aliases.isEmpty())
               ? it->second.aliases.first()
               : QString("Unknown");
}

QString geo::CountryName::longest(Country c) {
    auto it = Meta().find(c);
    if (it == Meta().end() || it->second.aliases.isEmpty())
        return QString();

    const QString* longest = &it->second.aliases.first();
    for (const auto& a : it->second.aliases) {
        if (a.size() > longest->size())
            longest = &a;
    }
    return *longest;
}

QString geo::CountryName::alpha2(Country c) {
    auto it = Meta().find(c);
    return it != Meta().end() ? QString(it->second.alpha2) : QString("ZZ");
}

QString geo::CountryName::alpha3(Country c) {
    auto it = Meta().find(c);
    return it != Meta().end() ? QString(it->second.alpha3) : QString("ZZZ");
}

Continent CountryName::continent(Country c) { return static_cast <Continent> (+c & 0xFF00); }
Subregion CountryName::region   (Country c) { return static_cast <Subregion> (+c & 0xFFF0); }

geo::Country geo::CountryName::from_string(const QString& any) {
    QString s = any.trimmed().toLower();
        for (auto& [key,val] : Meta()) {
            for (const auto& alias : val.aliases) {
                if (s == alias.toLower())
                    return key;
            }
        }
        return Country::Unknown;
}

geo::Country geo::CountryName::from_alpha2(const QString& a2) {
    QString s = a2.trimmed().toUpper();
    for (auto& [key,val] : Meta()) {
        if (s == val.alpha2)
            return key;
    }
    return Country::Unknown;
}

geo::Country geo::CountryName::from_alpha3(const QString& a3) {
    QString s = a3.trimmed().toUpper();
    for (auto& [key,val] : Meta()) {
        if (s == val.alpha3)
            return key;
    }
    return Country::Unknown;
}

geo::Country geo::CountryName::from_full(const QString& full) {
    return from_string(full);
}

QList <geo::Country> geo::CountryName::all() {
    QList <Country> list;
    for (auto& [key,val] : Meta()) list.append(key);
    return list;
}



// QDataStream operators
QDataStream& operator << (QDataStream& out, const gC& c) {
    out << static_cast<quint16>(+c);
    return out;
}

QDataStream& operator >> (QDataStream& in, gC& c) {
    quint16 val;
    in >> val;
    c = static_cast<gC>(val);
    return in;
}
