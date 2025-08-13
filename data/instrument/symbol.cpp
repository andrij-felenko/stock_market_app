#include "symbol.h"
#include <QStringList>
#include <QtCore/QDebug>

data::ticker::Symbol::Symbol(QString code, Exchange exchange)
    : _code(code), _enum(exchange) { /* */ }

data::ticker::Symbol::Symbol(QString code, QString exch)
    : _code(code), _enum(Unknown)
{
    set_exchange(exch);
    set_short(exch);
}

data::ticker::Symbol::Symbol(QString full) : _code(""), _enum(Unknown)
{
    QStringList list = full.split(".");
    if (list.length() == 1){
        _code = full;
        _enum = US;
    }
    else if (list.length() == 2){
        _code = list[0];
        set_exchange(list[1]);
    }
}

using dtS = data::ticker::Symbol;
QString dtS::decription(Exchange e)
{
    if (exist(e))
        for (const auto& it : ex_map_list())
            if (it.enum_ == e)
                return it.fullname;
    return "Unknown";
}

QString dtS::to_short(Exchange e)
{
    if (exist(e))
        for (const auto& it : ex_map_list())
            if (it.enum_ == e)
                return it.sufix;
    return "Unknown";
}

dtS::Exchange dtS::from_short(QString str)
{
    for (const auto& it : ex_map_list())
        if (it.sufix.compare(str, Qt::CaseInsensitive) == 0)
            return it.enum_;
    return Unknown;
}

QString dtS::exchange_str(Exchange e)
{
    if (exist(e))
        for (const auto& it : ex_map_list())
            if (it.enum_ == e)
                return it.exchange;
    return "Unknown";
}

dtS::Exchange dtS::from_exchange(QString str)
{
    for (const auto& it : ex_map_list())
        if (it.exchange.compare(str, Qt::CaseInsensitive) == 0 ||
            it.sufix   .compare(str, Qt::CaseInsensitive) == 0)
            return it.enum_;
    return Unknown;
}


bool dtS::operator == (const Symbol& other) const
{ return _enum == other._enum && _code == other._code; }

bool dtS::Symbol::operator != (const Symbol& other) const
{ return !(*this == other); }

bool dtS::Symbol::operator < (const Symbol& other) const
{ return (_code == other._code) ? _enum < other._enum : _code < other._code; }

dtS::operator QString() const { return full(); }

bool dtS::Symbol::operator == (const dtS::Exchange enum_) const { return _enum == enum_; }
bool dtS::Symbol::operator == (const std::vector <dtS::Exchange> list) const
{
    for (const auto& it : list)
        if (*this == it)
            return true;
    return false;
}


QString dtS::decription()   const { return decription  (_enum); }
QString dtS::to_short()     const { return to_short    (_enum); }
QString dtS::exchange_str() const { return exchange_str(_enum); }

void dtS::set_short   (QString str) { set_exchange(from_short   (str)); }
void dtS::set_exchange(QString str) { set_exchange(from_exchange(str)); }
void dtS::set_exchange(Exchange  e) { if (e != Unknown) _enum = e; }

dtS::Exchange dtS::exchange() const { return _enum; }
QString dtS::full() const { return QString("%1.%2").arg(_code, to_short()); }
QString dtS::code() const { return _code; }
void dtS::set_code(QString code) { _code = code.toUpper(); }
void dtS::clear() { _code.clear(); _enum = Unknown; }
bool dtS::empty() const { return not exist() || _code.isEmpty(); }
bool dtS::lse_outer() const { return _enum == LSE && ! _code.isEmpty() &&  _code[0].isDigit(); }
bool dtS::lse_inner() const { return _enum == LSE && ! _code.isEmpty() && !_code[0].isDigit(); }

bool dtS::us()        const { return us       (_enum); }
bool dtS::nyse()      const { return nyse     (_enum); }
bool dtS::nasdaq()    const { return nasdaq   (_enum); }
bool dtS::otc()       const { return otc      (_enum); }
bool dtS::europe()    const { return europe   (_enum); }
bool dtS::trash()     const { return trash    (_enum); }
bool dtS::euromajor() const { return euromajor(_enum); }
bool dtS::eurominor() const { return eurominor(_enum); }
bool dtS::asia()      const { return asia     (_enum); }
bool dtS::world()     const { return world    (_enum); }
bool dtS::exist()     const { return exist    (_enum); }

bool dtS::us       (Exchange e) { return exist(e) && (e & 0b1100'0000) == 0; }
bool dtS::nyse     (Exchange e) { return exist(e) && (e & 0b1111'1000) == NYSE; }
bool dtS::nasdaq   (Exchange e) { return e == NASDAQ; }
bool dtS::otc      (Exchange e) { return exist(e) && (e & 0b1111'0000) == OTC; }
bool dtS::europe   (Exchange e) { return exist(e) && (e & 0b1000'0000) == EU; }
bool dtS::trash    (Exchange e) { return exist(e) && (e & 0b1111'0000) == PINK; }
bool dtS::euromajor(Exchange e) { return exist(e) && (e & 0b1100'0000) == EU1; }
bool dtS::eurominor(Exchange e) { return exist(e) && (e & 0b1100'0000) == EU2; }
bool dtS::asia     (Exchange e) { return exist(e) && (e & 0b1110'0000) == Asia; }
bool dtS::world    (Exchange e) { return exist(e) && (e & 0b1100'0000) == World; }

bool dtS::check_exchange(QString ex) const
{
    for (const auto& it : ex_map_list()){
        if (it.exchange == ex || it.sufix == ex)
            return true;
    }
    return false;
}

bool dtS::check_exchange(Exchange ex) const { return _enum == ex; }
bool dtS::contains(QList <Exchange> ex) const
{
    for (const auto& it : std::as_const(ex))
        if (check_exchange(it))
            return true;
    return false;
}

bool data::ticker::Symbol::exist(Exchange e)
{
    if (e == Unknown)
        return false;

    for (const auto& it : ex_map_list())
        if (it.enum_ == e)
            return true;
    return false;
}

namespace data::ticker {
    QDataStream& operator << (QDataStream& s, const Symbol& d) {
        return s << d._code << static_cast <uint8_t> (d._enum);
    }

    QDataStream& operator >> (QDataStream& s, Symbol& d) {
        uint8_t e;
        s >> d._code >> e;
        d.set_exchange(static_cast <Symbol::Exchange> (e));
        return s;
    }

    QDebug operator << (QDebug dbg, const Symbol& symbol) {
        QDebugStateSaver saver(dbg);
        dbg.nospace() << "Symbol(" << symbol.code() << "." << symbol.to_short() << ")";
        return dbg;
    }
}

ExchangeEnumList data::ticker::Symbol::major_europe_sufix()
{
    return { PA, AM, XETRA, BE, LU };
}

ExchangeEnumList data::ticker::Symbol::minor_europe_sufix()
{
    return {
        SW, MC, BR, OL, HE, CO, ST, XSTO, VI, LS,
        DUSEL, HANOV, MUNIC, STUTG, FRANK, HUMBR, PRAGA, WARSH, ATHENS
    };
}

ExchangeEnumList data::ticker::Symbol::other_worlds_sufix()
{
    return { Taiwan, Korea, Australia, Toronto, NEO, TSX, TEL_AVIV };
}

ExchangeEnumList data::ticker::Symbol::us_sufix()
{
    return {
        US, NASDAQ, NYSE, NYSE_ARCA, NYSE_MKT, AMEX, BATS,
        PINK, OTC_GREY, OTC_CE, OTC_BB,
        OTC, OTC_QB, OTC_QX, NMFQA, OTC_MKTS, OTC_MTKS
    };
}

QList <data::ticker::Symbol::Exchange> data::ticker::Symbol::all_exchange()
{
    QList <data::ticker::Symbol::Exchange> ret;
    for (const auto& it : ex_map_list())
        ret.push_back(it.enum_);
    return ret;
}

QStringList data::ticker::Symbol::all_exchange_short()
{
    QStringList ret;
    for (const auto& it : ex_map_list())
        ret.push_back(it.exchange);
    ret.removeDuplicates();
    return ret;
}

const std::vector <data::ticker::Symbol::ExchangeEnumStruct>& data::ticker::Symbol::ex_map_list()
{
    std::function add = [](std::vector <data::ticker::Symbol::ExchangeEnumStruct>& list,
                                Exchange enum_, QString sufix, QString exchange, QString name){
        ExchangeEnumStruct add;
        add.exchange = exchange;
        add.enum_ = enum_;
        add.fullname = name;
        add.sufix = sufix;
        list.push_back(add);
    };

    static std::vector <data::ticker::Symbol::ExchangeEnumStruct> list;
    if (list.empty()){
        add(list, Exchange::Australia, "AU", "AU", "Australian Securities Exchange");
        add(list, Exchange::Toronto, "TO", "TW", "Toronto Stock Exchange");
        add(list, Exchange::Taiwan, "TW", "TW", "Taipei / Taiwan Stock Exchange");
        add(list, Exchange::Korea, "KO", "KO", "Korea Stock Exchange");
        add(list, Exchange::NEO, "NEO", "NEO", "NEO Exchange");
        add(list, Exchange::TSX, "V", "V", "TSX Venture Exchange");
        add(list, Exchange::TEL_AVIV, "TA", "TA", "Tel Aviv Stock Exchange");

        add(list, Exchange::AM, "AS", "AS", "Euronext Amsterdam");
        add(list, Exchange::BE, "BE", "BE", "Börse Berlin");
        add(list, Exchange::XETRA, "XETRA", "XETRA", "Deutsche Börse");
        add(list, Exchange::BR, "BR", "BR", "Euronext Brussels");
        add(list, Exchange::LSE, "L", "LSE", "London Stock Exchange");
        add(list, Exchange::CO, "CO", "CO", "Copenhagen Stock Exchange");
        add(list, Exchange::MC, "MC", "MC", "Bolsa de Madrid");
        add(list, Exchange::HE, "FI", "HE", "Helsinki");
        add(list, Exchange::OL, "OL", "OL", "Oslo Stock Exchange");
        add(list, Exchange::PA, "PA", "PA", "Euronext Paris");
        add(list, Exchange::ST, "ST", "ST", "Stockholm Stock Exchange");
        add(list, Exchange::XSTO, "ST", "XSTO", "");
        add(list, Exchange::SW, "SW", "SW", "Switzerland");
        add(list, Exchange::LU, "LU", "LU", "Luxembourg Stock Exchange");
        add(list, Exchange::HUMBR, "HM", "HM", "Hamburg Exchange");
        add(list, Exchange::DUSEL, "DU", "DU", "Dusseldorf Exchange");
        add(list, Exchange::HANOV, "HA", "HA", "Hanover Exchange");
        add(list, Exchange::MUNIC, "MU", "MU", "Munich Exchange");
        add(list, Exchange::STUTG, "STU", "STU", "Stuttgart Exchange");
        add(list, Exchange::FRANK, "F", "F", "Frankfurt Exchange");
        add(list, Exchange::VI, "VI", "VI", "Vienna Exchange");
        add(list, Exchange::LS, "LS", "LS", "Euronext Lisbon");
        add(list, Exchange::PRAGA, "PR", "PR", "Prague Stock Exchange");
        add(list, Exchange::WARSH, "WAR", "WAR", "Warsaw Stock Exchange");
        add(list, Exchange::ATHENS, "AT", "AT", "Athens Exchange");

        add(list, Exchange::US, "US", "US", "");
        add(list, Exchange::NYSE, "US", "NYSE",
            "New York Stock Exchange, найстаріша та найбільша біржа у світі за капіталізацією");
        add(list, Exchange::NYSE_ARCA, "US", "NYSE ARCA",
            "електронна біржа, спадкоємець ArcaEx, спеціалізується на ETF та ETP продуктах");
        add(list, Exchange::NASDAQ, "US", "NASDAQ",
            "повністю електронна біржа, орієнтована на технологічні компанії,"
            " друга за обсягами в США");

        add(list, Exchange::BATS, "US", "BATS",
            "мультибіржа, що приймає торги NYSE, AMEX, Arca, Nasdaq;"
            " часто використовується як ECN");
        add(list, Exchange::AMEX, "US", "AMEX", "");
        add(list, Exchange::NYSE_MKT, "US", "NYSE MKT",
            "NYSE American, раніше AMEX — фокус на малих і середніх компаніях,"
            " торгівля ETF, опціонами, bonds");

        add(list, Exchange::OTC, "US", "OTC", "Позабіржовий ринок — umbrella для всіх нижче");
        add(list, Exchange::OTC_QB, "US", "OTCQB",
            "Другий рівень, призначений для молодих чи невеликих компаній."
            " Повинні мати SEC-реєстрацію або бути звітними. Мінімум $0.01/акція");
        add(list, Exchange::OTC_QX, "US", "OTCQX",
            "Найвищий рівень OTC. Прозорі компанії з міжнародною звітністю, без SEC-філінгів,"
            " але з фінансовим аудитом. Приклади: Heineken, Roche");
        add(list, Exchange::NMFQA, "US", "NMFQS",
            "OTCQX (найвищий OTC-tier від OTC Markets Group),"
            " для якісних компаній з вимогами до фінзвітності");
        add(list, Exchange::OTC_MKTS, "US", "OTCMKTS", "");
        add(list, Exchange::OTC_MTKS, "US", "OTCMTKS",
            "Іноді використовується як синонім до OTC Group /"
            " OTC Link ATS (Alternative Trading System)");

        add(list, Exchange::PINK, "US", "PINK",
            "ринок Pink Sheets (OTC), менші компанії без обов’язкового звітування SEC,"
            " високий ризик");
        add(list, Exchange::OTC_GREY, "US", "OTCGREY",
            "Взагалі не мають публічної інформації чи маркет-мейкерів. Найменш прозорий ринок."
            " Немає котирувань, тільки ручні транзакції");
        add(list, Exchange::OTC_BB, "US", "OTCBB",
            "Раніше регульований майданчик від FINRA (OTCBB). Більше не функціонує (закритий 2021)."
            " Колись був більш прозорий, ніж Pink Sheets");
        add(list, Exchange::OTC_CE, "US", "OTCCE",
            "\"Обережно, покупцю\" — маркування від OTC для компаній із підозрілою діяльністю,"
            " маніпуляціями або браком інформації");
    }
    return list;
}
