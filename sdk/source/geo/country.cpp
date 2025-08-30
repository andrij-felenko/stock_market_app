#include "geo/geo.h"
#include <qdebug.h>

namespace geo::country {
    struct Meta {
        Country _enum;
        QString alpha2;
        QString alpha3;
        QStringList aliases;

        Meta(Country e, QString a2, QString a3, std::initializer_list<const char*> list)
            : _enum(e), alpha2(a2), alpha3(a3)
        {
            for (auto s : list)
                aliases << QString::fromUtf8(s);
        }
    };

    const std::vector <Meta>& metadata();
}

namespace geo {
    QDataStream& operator << (QDataStream& out, const Country& tag)
    { return out << static_cast <uint16_t > (tag); }

    QDataStream& operator >> (QDataStream& in, Country& tag) {
        uint16_t  val;
        in >> val;
        tag = static_cast <Country>(val);
        return in;
    }
}

QString geo::operator - (geo::Country c) { return geo::country::alpha3(c); }
QString geo::operator & (geo::Country c) { return geo::country::alpha2(c); }
QString geo::operator ~ (geo::Country c) { return geo::country::primary(c); }

QString geo::country::primary(Country c)
{
    for (const auto& it : metadata())
        if (it._enum == c)
            if (not it.aliases.isEmpty())
                return it.aliases[0];
    return "NaN";
}

QString geo::country::longest(Country c)
{
    QString ret;
    for (const auto& it : metadata())
        if (it._enum == c){
            for (const auto& s : it.aliases)
                if (ret.length() < s.length())
                    ret = s;
            break;
        }
    return ret.isEmpty() ? "NaN" : ret;
}

QString geo::country::alpha2(Country c)
{
    for (const auto& it : metadata())
        if (it._enum == c)
            return it.alpha2;
    return "--";
}

QString geo::country::alpha3(Country c)
{
    for (const auto& it : metadata())
        if (it._enum == c)
            return it.alpha3;
    return "---";
}

// Розпізнавання з рядка: аліаси/повна назва/ISO
geo::Country geo::country::from_string(const QString& any)
{
    for (const auto& it : metadata())
        if (it.aliases.contains(any, Qt::CaseInsensitive) ||
            it.alpha2.compare(any, Qt::CaseInsensitive) == 0 ||
            it.alpha3.compare(any, Qt::CaseInsensitive) == 0)
            return it._enum;
    qDebug() << Q_FUNC_INFO << any;
    return Country::Unknown;
}

geo::Continent geo::country::continent(Country c) { return static_cast <Continent> (+c & 0xFFF0); }
geo::Region    geo::country::region   (Country c) { return static_cast <Region>    (+c & 0xFF00); }

std::vector <geo::Country> geo::country::all(Region r)
{
    std::vector <geo::Country> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        if (r == Region::Unknown || r == region(it._enum))
            ret.push_back(it._enum);
    return ret;
}

std::vector <geo::Country> geo::country::all(Continent c)
{
    std::vector <geo::Country> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        if (c == Continent::None || c == continent(it._enum))
            ret.push_back(it._enum);
    return ret;
}

QStringList geo::country::all_names(Region r)
{
    QStringList ret;
    ret.reserve(metadata().size() * 4);

    for (const auto &it : metadata())
        if (r == Region::Unknown || r == region(it._enum))
            ret += it.aliases;
    return ret;
}

QStringList geo::country::all_names(Continent c)
{
    QStringList ret;
    ret.reserve(metadata().size() * 4);

    for (const auto &it : metadata())
        if (c == Continent::None || c == continent(it._enum))
            ret += it.aliases;
    return ret;
}

QStringList geo::country::all_alpha2(Region r)
{
    QStringList ret;
    ret.reserve(metadata().size() * 4);

    for (const auto &it : metadata())
        if (r == Region::Unknown || r == region(it._enum))
            ret.push_back(it.alpha2);
    return ret;
}

QStringList geo::country::all_alpha2(Continent c)
{
    QStringList ret;
    ret.reserve(metadata().size() * 4);

    for (const auto &it : metadata())
        if (c == Continent::None || c == continent(it._enum))
            ret.push_back(it.alpha2);
    return ret;
}

QStringList geo::country::all_alpha3(Region r)
{
    QStringList ret;
    ret.reserve(metadata().size() * 4);

    for (const auto &it : metadata())
        if (r == Region::Unknown || r == region(it._enum))
            ret.push_back(it.alpha3);
    return ret;
}

QStringList geo::country::all_alpha3(Continent c)
{
    QStringList ret;
    ret.reserve(metadata().size() * 4);

    for (const auto &it : metadata())
        if (c == Continent::None || c == continent(it._enum))
            ret.push_back(it.alpha3);
    return ret;
}

const std::vector <geo::country::Meta>& geo::country::metadata()
{
    static std::vector <geo::country::Meta> _;
    if (not _.empty())
        return _;

    auto push = [&](geo::Country e, const char* a2, const char* a3, auto... aliases){
        _.emplace_back(geo::country::Meta(e, a2, a3, {aliases...}));
    };

    // ----------------------- Europe -------------------------------------------------------------
    push(Country::Ukraine, "UA", "UKR", "Ukraine", "Ukrayina");
    push(Country::Moldova, "MD", "MDA", "Moldova", "Republic of Moldova");
    push(Country::Romania, "RO", "ROU", "Romania");
    push(Country::Bulgaria, "BG", "BGR", "Bulgaria");

    push(Country::Poland, "PL", "POL", "Poland", "Rzeczpospolita Polska");
    push(Country::Czechia, "CZ", "CZE", "Czechia", "Czech Republic");
    push(Country::Slovakia, "SK", "SVK", "Slovakia","Slovak Republic");
    push(Country::Hungary, "HU", "HUN", "Hungary", "Magyarország");

    push(Country::Estonia,  "EE", "EST", "Estonia", "Eesti");
    push(Country::Latvia, "LV", "LVA", "Latvia", "Latvija");
    push(Country::Lithuania, "LT", "LTU", "Lithuania", "Lietuva");
    push(Country::Belarus, "BY", "BLR", "Belarus", "Byelorussia");

    push(Country::Androra, "AD", "AND", "Andorra", "Principality of Andorra");
    push(Country::Portugal, "PT", "PRT", "Portugal", "República Portuguesa");
    push(Country::Spain, "ES", "ESP", "Spain", "España");
    push(Country::Gibraltar, "GI", "GIB", "Gibraltar");

    push(Country::Malta, "MT", "MLT", "Malta", "Republic of Malta");
    push(Country::Cyprus, "CY", "CYP", "Cyprus", "Republic of Cyprus");

    push(Country::Greece, "GR", "GRC", "Greece", "Hellenic Republic", "Elláda");
    push(Country::Albania, "AL", "ALB", "Albania", "Shqipëria");
    push(Country::NorthMacedonia, "MK", "MKD", "North Macedonia","Republic of North Macedonia");
    push(Country::Serbia, "RS", "SRB", "Serbia", "Republic of Serbia");
    push(Country::BosniaAndHerzegovina, "BA", "BIH", "Bosnia and Herzegovina", "BiH");
    push(Country::Montenegro, "ME", "MNE", "Montenegro", "Crna Gora");
    push(Country::Kosovo, "XK", "XKX", "Kosovo", "Republic of Kosovo");

    push(Country::Italy, "IT", "ITA", "Italy", "Italia");
    push(Country::Croatia, "HR", "HRV", "Croatia", "Hrvatska");
    push(Country::Slovenia, "SI", "SVN", "Slovenia", "Republika Slovenija");
    push(Country::SanMarino, "SM", "SMR", "San Marino", "Republic of San Marino");
    push(Country::Monaco, "MC", "MCO", "Monaco", "Principality of Monaco");
    push(Country::Vatican, "VA", "VAT", "Vatican City", "Holy See");

    push(Country::Sweden, "SE", "SWE", "Sweden", "Sverige");
    push(Country::Norway, "NO", "NOR", "Norway", "Norge");
    push(Country::Finland, "FI", "FIN", "Finland", "Suomi");
    push(Country::Iceland, "IS", "ISL", "Iceland", "Ísland");
    push(Country::Denmark, "DK", "DNK", "Denmark", "Danmark");
    push(Country::Greenland, "GL", "GRL", "Greenland", "Kalaallit Nunaat");

    push(Country::Belgium, "BE", "BEL", "Belgium", "België", "Belgique", "Belgien");
    push(Country::Netherlands, "NL", "NLD", "Netherlands", "Nederland");
    push(Country::Luxembourg, "LU", "LUX", "Luxembourg", "Lëtzebuerg");

    push(Country::UnitedKingdom, "GB", "GBR", "United Kingdom","UK","Great Britain","Britain");
    push(Country::IsleOfMan, "IM", "IMN", "Isle of Man");
    push(Country::Jersey, "JE", "JEY", "Jersey");
    push(Country::Guernsey, "GG", "GGY", "Guernsey");
    push(Country::Ireland, "IE", "IRL", "Ireland", "Éire");

    push(Country::France, "FR", "FRA", "France", "République française");
    push(Country::Germany, "DE", "DEU", "Germany", "Deutschland");
    push(Country::Switzerland, "CH", "CHE", "Switzerland", "Schweiz", "Suisse", "Svizzera");
    push(Country::Austria, "AT", "AUT", "Austria", "Österreich");

    // ----------------------- Asia ---------------------------------------------------------------
    push(Country::SaudiArabia,        "SA", "SAU", "Saudi Arabia", "Kingdom of Saudi Arabia");
    push(Country::UnitedArabEmirates, "AE", "ARE", "UAE", "United Arab Emirates");
    push(Country::Oman,               "OM", "OMN", "Oman", "Sultanate of Oman");
    push(Country::Yemen,              "YE", "YEM", "Yemen", "Republic of Yemen");
    push(Country::Kuwait,             "KW", "KWT", "Kuwait", "State of Kuwait");
    push(Country::Qatar,              "QA", "QAT", "Qatar", "State of Qatar");
    push(Country::Bahrein,            "BH", "BHR", "Bahrain", "Kingdom of Bahrain");

    push(Country::Armenia,    "AM", "ARM", "Armenia", "Republic of Armenia");
    push(Country::Georgia,    "GE", "GEO", "Georgia", "Sakartvelo");
    push(Country::Azerbaijan, "AZ", "AZE", "Azerbaijan", "Republic of Azerbaijan");

    push(Country::Israel,    "IL", "ISR", "Israel", "State of Israel");
    push(Country::Lebanon,   "LB", "LBN", "Lebanon", "Lebanese Republic");
    push(Country::Syria,     "SY", "SYR", "Syria", "Syrian Arab Republic");
    push(Country::Iraq,      "IQ", "IRQ", "Iraq", "Republic of Iraq");
    push(Country::Jordan,    "JO", "JOR", "Jordan", "Hashemite Kingdom of Jordan");
    push(Country::Turkiye,   "TR", "TUR", "Türkiye", "Turkey", "Republic of Türkiye");
    push(Country::Palestine, "PS", "PSE", "Palestine","State of Palestine");

    push(Country::Kazakhstan,   "KZ", "KAZ", "Kazakhstan", "Republic of Kazakhstan");
    push(Country::Turkmenistan, "TM", "TKM", "Turkmenistan");
    push(Country::Kyrgyzstan,   "KG", "KGZ", "Kyrgyzstan", "Kyrgyz Republic");
    push(Country::Tajikistan,   "TJ", "TJK", "Tajikistan", "Republic of Tajikistan");
    push(Country::Uzbekistan,   "UZ", "UZB", "Uzbekistan", "Republic of Uzbekistan");

    push(Country::India,     "IN", "IND", "India", "Republic of India");
    push(Country::Nepal,     "NP", "NPL", "Nepal", "Federal Democratic Republic of Nepal");
    push(Country::Maldives,  "MV", "MDV", "Maldives", "Republic of Maldives");
    push(Country::Bhutan,    "BT", "BTN", "Bhutan", "Kingdom of Bhutan");
    push(Country::Shrilanka, "LK", "LKA", "Sri Lanka",
                 "Democratic Socialist Republic of Sri Lanka");

    push(Country::Afghanistan, "AF", "AFG", "Afghanistan", "Islamic Republic of Afghanistan");
    push(Country::Iran,        "IR", "IRN", "Iran", "Islamic Republic of Iran", "Persia");
    push(Country::Pakistan,    "PK", "PAK", "Pakistan", "Islamic Republic of Pakistan");

    push(Country::Thailand, "TH", "THA", "Thailand", "Kingdom of Thailand");
    push(Country::Vietnam,  "VN", "VNM", "Vietnam", "Socialist Republic of Vietnam");
    push(Country::Cambodia, "KH", "KHM", "Cambodia", "Kingdom of Cambodia");
    push(Country::Laos,     "LA", "LAO", "Laos", "Lao PDR", "Lao People's Democratic Republic");
    push(Country::Myanmar,  "MM", "MMR", "Myanmar", "Burma","Republic of the Union of Myanmar");

    push(Country::Japan,      "JP", "JPN", "Japan", "Nippon", "State of Japan");
    push(Country::China,      "CN", "CHN", "China", "People's Republic of China", "PRC");
    push(Country::Taiwan,     "TW", "TWN", "Taiwan", "Republic of China", "ROC");
    push(Country::SouthKorea, "KR", "KOR", "South Korea", "Republic of Korea", "ROK");
    push(Country::HongKong,   "HK", "HKG", "Hong Kong", "Hong Kong SAR");
    push(Country::Mongolia,   "MN", "MNG", "Mongolia");
    push(Country::Macau,      "MO", "MAC", "Macau", "Macao SAR");
    push(Country::NorthKorea, "KP", "PRK",
                 "North Korea", "Democratic People's Republic of Korea", "DPRK");


    push(Country::Brunei,      "BN", "BRN", "Brunei", "Nation of Brunei, Abode of Peace");
    push(Country::Malaysia,    "MY", "MYS", "Malaysia");
    push(Country::Singapore,   "SG", "SGP", "Singapore", "Republic of Singapore");
    push(Country::Philippines, "PH", "PHL", "Philippines", "Republic of the Philippines");
    push(Country::Indonesia,   "ID", "IDN", "Indonesia", "Republic of Indonesia");
    push(Country::EastTimor,   "TL", "TLS",
                 "East Timor", "Timor-Leste", "Democratic Republic of Timor-Leste");

    // ----------------------- Africa -------------------------------------------------------------
    push(Country::Morocco, "MA", "MAR", "Morocco", "Kingdom of Morocco");
    push(Country::Algeria, "DZ", "DZA", "Algeria", "People's Democratic Republic of Algeria");
    push(Country::Tunisia, "TN", "TUN", "Tunisia", "Republic of Tunisia");
    push(Country::Libya,   "LY", "LBY", "Libya", "State of Libya");
    push(Country::WesternSahara, "EH", "ESH",
                 "Western Sahara", "Sahrawi Arab Democratic Republic");

    push(Country::Egypt,       "EG", "EGY", "Egypt", "Arab Republic of Egypt");
    push(Country::Sudan,       "SD", "SDN", "Sudan", "Republic of the Sudan");
    push(Country::SouthSudan,  "SS", "SSD", "South Sudan", "Republic of South Sudan");

    push(Country::Mauritania,  "MR", "MRT", "Mauritania", "Islamic Republic of Mauritania");
    push(Country::Mali,        "ML", "MLI", "Mali", "Republic of Mali");
    push(Country::Niger,       "NE", "NER", "Niger", "Republic of the Niger");
    push(Country::Chad,        "TD", "TCD", "Chad", "Republic of Chad");
    push(Country::BurkinaFaso, "BF", "BFA", "Burkina Faso");

    push(Country::Senegal,      "SN", "SEN", "Senegal", "Republic of Senegal");
    push(Country::Gambia,       "GM", "GMB", "Gambia", "The Gambia", "Republic of The Gambia");
    push(Country::Guinea,       "GN", "GIN", "Guinea", "Republic of Guinea");
    push(Country::GuineaBissau, "GW", "GNB", "Guinea-Bissau", "Republic of Guinea-Bissau");
    push(Country::SierraLeone,  "SL", "SLE", "Sierra Leone", "Republic of Sierra Leone");
    push(Country::Liberia,      "LR", "LBR", "Liberia", "Republic of Liberia");
    push(Country::Ghana,        "GH", "GHA", "Ghana", "Republic of Ghana");
    push(Country::Togo,         "TG", "TGO", "Togo", "Togolese Republic");
    push(Country::Benin,        "BJ", "BEN", "Benin", "Republic of Benin");
    push(Country::Nigeria,      "NG", "NGA""Nigeria", "Federal Republic of Nigeria");
    push(Country::IvoryCoast,   "CI","CIV", "Ivory Coast",
                 "Côte d'Ivoire", "Republic of Côte d'Ivoire");

    push(Country::Cameroon,               "CM", "CMR", "Cameroon", "Republic of Cameroon");
    push(Country::CentralAfricanRepublic, "CF", "CAF", "Central African Republic");
    push(Country::EquatorialGuinea,       "GQ", "GNQ",
                 "Equatorial Guinea", "Republic of Equatorial Guinea");
    push(Country::Gabon,                  "GA", "GAB", "Gabon", "Gabonese Republic");
    push(Country::RepublicOfCongo,        "CG", "COG",
                 "Republic of the Congo", "Congo-Brazzaville");
    push(Country::DR_Congo,               "CD", "COD",
                 "DR Congo", "Democratic Republic of the Congo", "Congo-Kinshasa");

    push(Country::Ethiopia, "ET", "ETH", "Ethiopia", "Federal Democratic Republic of Ethiopia");
    push(Country::Eritrea,  "ER", "ERI", "Eritrea", "State of Eritrea");
    push(Country::Djibouti, "DJ", "DJI", "Djibouti", "Republic of Djibouti");
    push(Country::Somalia,  "SO", "SOM", "Somalia", "Federal Republic of Somalia");

    push(Country::Uganda,   "UG", "UGA", "Uganda", "Republic of Uganda");
    push(Country::Rwanda,   "RW", "RWA", "Rwanda", "Republic of Rwanda");
    push(Country::Burundi,  "BI", "BDI", "Burundi", "Republic of Burundi");
    push(Country::Tanzania, "TZ", "TZA", "Tanzania", "United Republic of Tanzania");
    push(Country::Kenya,    "KE", "KEN", "Kenya", "Republic of Kenya");

    push(Country::Angola,      "AO", "AGO", "Angola", "Republic of Angola");
    push(Country::Zambia,      "ZM", "ZMB", "Zambia", "Republic of Zambia");
    push(Country::Zimbabwe,    "ZW", "ZWE", "Zimbabwe", "Republic of Zimbabwe");
    push(Country::Malawi,      "MW", "MWI", "Malawi", "Republic of Malawi");
    push(Country::Mozambique,  "MZ", "MOZ", "Mozambique", "Republic of Mozambique");
    push(Country::Namibia,     "NA", "NAM", "Namibia", "Republic of Namibia");
    push(Country::Botswana,    "BW", "BWA", "Botswana", "Republic of Botswana");
    push(Country::SouthAfrica, "ZA", "ZAF", "South Africa", "Republic of South Africa", "RSA");
    push(Country::Lesotho,     "LS", "LSO", "Lesotho", "Kingdom of Lesotho");
    push(Country::Eswatini,    "SZ", "SWZ", "Eswatini", "Swaziland", "Kingdom of Eswatini");

    push(Country::Madagascar, "MG", "MDG", "Madagascar", "Republic of Madagascar");
    push(Country::Comoros,    "KM", "COM", "Comoros", "Union of the Comoros");
    push(Country::Seychelles, "SC", "SYC", "Seychelles", "Republic of Seychelles");
    push(Country::Mauritius,  "MU", "MUS", "Mauritius", "Republic of Mauritius");

    // ----------------------- America ------------------------------------------------------------
    push(Country::UnitedStates, "US", "USA", "United States", "USA","United States of America");
    push(Country::Canada,       "CA", "CAN", "Canada");

    push(Country::Mexico,     "MX", "MEX", "Mexico", "Estados Unidos Mexicanos");
    push(Country::Panama,     "PA", "PAN", "Panama", "Republic of Panama");
    push(Country::Nicaragua,  "NI", "NIC", "Nicaragua", "Republic of Nicaragua");
    push(Country::Honduras,   "HN", "HND", "Honduras", "Republic of Honduras");
    push(Country::Guatemala,  "GT", "GTM", "Guatemala", "Republic of Guatemala");
    push(Country::ElSalvador, "SV", "SLV", "El Salvador", "Republic of El Salvador");
    push(Country::Belize,     "BZ", "BLZ", "Belize");

    push(Country::Cuba,              "CU", "CUB", "Cuba", "Republic of Cuba");
    push(Country::CostaRica,         "CR", "CRI", "Costa Rica", "Republic of Costa Rica");
    push(Country::Bermuda,           "BM", "BMU", "Bermuda"); // UK territory
    push(Country::Haiti,             "HT", "HTI", "Haiti", "Republic of Haiti");
    push(Country::Barbados,          "BB", "BRB", "Barbados");
    push(Country::Grenada,           "GD", "GRD", "Grenada");
    push(Country::PuertoRico,        "PR", "PRI", "Puerto Rico"); // US territory
    push(Country::Jamaica,           "JM", "JAM", "Jamaica");
    push(Country::Bahamas,           "BS", "BHS", "Bahamas", "The Bahamas");
    push(Country::CaymanIslands,     "KY", "CYM", "Cayman Islands"); // UK territory
    push(Country::DominicanRepublic, "DO", "DOM", "Dominican Republic");
    push(Country::VirginIslands,     "VG", "VGB", "Virgin Islands, British"); // UK territory

    push(Country::Brazil,    "BR", "BRA", "Brazil", "Federative Republic of Brazil");
    push(Country::Argentina, "AR", "ARG", "Argentina", "Argentine Republic");
    push(Country::Chile,     "CL", "CHL", "Chile", "Republic of Chile");
    push(Country::Colombia,  "CO", "COL", "Colombia", "Republic of Colombia");
    push(Country::Peru,      "PE", "PER", "Peru", "Republic of Peru");
    push(Country::Uruguay,   "UY", "URY", "Uruguay", "Oriental Republic of Uruguay");
    push(Country::Paraguay,  "PY", "PRY", "Paraguay", "Republic of Paraguay");
    push(Country::Bolivia,   "BO", "BOL", "Bolivia", "Plurinational State of Bolivia");
    push(Country::Guyana,    "GY", "GUY", "Guyana", "Co-operative Republic of Guyana");
    push(Country::Suriname,  "SR", "SUR", "Suriname", "Republic of Suriname");
    push(Country::Venezuela, "VE", "VEN", "Venezuela", "Bolivarian Republic of Venezuela");
    push(Country::Falklands, "FK", "FLK", "Falkland Islands", "Islas Malvinas"); // UK territory

    // ----------------------- Oceania ------------------------------------------------------------
    push(Country::Australia,  "AU", "AUS", "Australia", "Commonwealth of Australia");
    push(Country::NewZealand, "NZ", "NZL", "New Zealand", "Aotearoa");

    push(Country::PapuaNewGuinea, "PG", "PNG", "Papua New Guinea", "PNG");
    push(Country::Fiji,           "FJ", "FJI", "Fiji", "Republic of Fiji");
    push(Country::SolomonIslands, "SB", "SLB", "Solomon Islands");
    push(Country::Vanuatu,        "VU", "VUT", "Vanuatu", "Republic of Vanuatu");
    push(Country::NewCaledonia,   "NC", "NCL", "New Caledonia"); // FR territory

    push(Country::Palau,    "PW", "PLW", "Palau", "Republic of Palau");
    push(Country::Nauru,    "NR", "NRU", "Nauru", "Republic of Nauru");
    push(Country::Kiribati, "KI", "KIR", "Kiribati", "Republic of Kiribati");
    push(Country::Guam,     "GU", "GUM", "Guam");
    push(Country::FederatedStatesOfMicronesia, "FM", "FSM", "Micronesia", "FSM");
    push(Country::NorthernMarianaIslands,      "MP", "MNP", "Northern Mariana Islands");
    push(Country::MarshallIslands,             "MH", "MHL",
                 "Marshall Islands", "Republic of the Marshall Islands");

    push(Country::Samoa,           "WS", "WSM", "Samoa", "Independent State of Samoa");
    push(Country::Tonga,           "TO", "TON", "Tonga", "Kingdom of Tonga");
    push(Country::Tuvalu,          "TV", "TUV", "Tuvalu");
    push(Country::CookIslands,     "CK", "COK", "Cook Islands"); // NZ assoc.
    push(Country::Niue,            "NU", "NIU", "Niue"); // NZ assoc.
    push(Country::FrenchPolynesia, "PF", "PYF", "French Polynesia"); // FR territory
    push(Country::WallisAndFutuna, "WF", "WLF", "Wallis and Futuna"); // FR territory
    push(Country::AmericanSamoa,   "AS", "ASM", "American Samoa"); // US territory
    push(Country::Tokelau,         "TK", "TKL", "Tokelau"); // NZ territory

    push(Country::SpecailArea,     "XS", "XSA", "Special area");

    return _;
}
