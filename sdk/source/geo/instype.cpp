#include "geo/geo.h"

namespace geo::instype {
    struct Meta {
        Instype  _enum;
        QStringList aliases;

        Meta(Instype e, std::initializer_list<const char*> list) : _enum(e) {
            for (auto s : list)
                aliases << QString::fromUtf8(s);
        }
    };

    const std::vector <Meta>& metadata();
}

namespace geo {
    QDataStream& operator << (QDataStream& out, const Instype& e)
    { return out << static_cast <uint8_t > (e); }

    QDataStream& operator >> (QDataStream& in, Instype& e) {
        uint8_t  val = 0;
        in >> val;
        e = static_cast <Instype> (val);
        return in;
    }
}

QString geo::operator ~ (geo::Instype c) { return geo::instype::to_string(c); }

QString geo::instype::to_string(geo::Instype type)
{
    for (const auto &it : metadata())
        if (it._enum == type)
            if (not it.aliases.empty())
                return it.aliases.first();
    return "NaN";
}

QString geo::instype::file_name(Instype e)
{
    if (e != Instype::Unknown)
        for (const auto& it : metadata())
            if (it._enum == e && not it.aliases.empty()){
                QString ret = it.aliases.first();
                ret.replace(QChar::Space, QChar('_'));
                return ret;
            }
    return "other";
}

geo::Instype geo::instype::from_string(const QString& str)
{
    for (const auto &it : metadata())
        for (const auto& s : it.aliases)
            if (s.compare(str, Qt::CaseInsensitive) == 0)
                return it._enum;
    for (const auto &it : metadata())
        if (file_name(it._enum) == str)
            return it._enum;
    return Instype::Unknown;
}

QStringList geo::instype::all_names()
{
    QStringList ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret += it.aliases;
    return ret;
}

std::vector <geo::Instype> geo::instype::all()
{
    std::vector <geo::Instype> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._enum);
    return ret;
}

const std::vector <geo::instype::Meta>& geo::instype::metadata()
{
    static std::vector <geo::instype::Meta> _;
    if (not _.empty())
        return _;

    auto push = [&](geo::Instype e, auto... aliases){
        _.emplace_back(geo::instype::Meta(e, {aliases...}));
    };

    // Europe
    push(Instype::Unknown,        "");
    push(Instype::CommonStock,    "Common Stock");
    push(Instype::PreferredStock, "Preferred Stock");
    push(Instype::ETC,            "ETC");
    push(Instype::ETF,            "ETF");
    push(Instype::Fund,           "Fund");
    push(Instype::MutualFund,     "Mutual Fund");
    push(Instype::Unit,           "Unit");
    push(Instype::Bond,           "Bond");
    push(Instype::Index,          "Index");
    push(Instype::Note,           "Note", "Notes");
    push(Instype::CapitalNotes,   "Capital Notes");

    return _;
}
