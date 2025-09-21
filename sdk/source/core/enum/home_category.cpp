#include "core/enum.h"

namespace sdk::home_category {
    struct Meta {
        HomeCategory _enum;
        QString _full;
    };

    const std::vector <Meta>& metadata();
}

namespace sdk {
    QDataStream& operator << (QDataStream& out, const HomeCategory& tag)
    { return out << uint8_t(static_cast <uint8_t> (tag)); }

    QDataStream& operator >> (QDataStream& in, HomeCategory& tag) {
        uint8_t  val;
        in >> val;
        tag = static_cast <HomeCategory>(val);
        return in;
    }
}

QString sdk::operator ~ (sdk::HomeCategory c) { return sdk::home_category::to_string(c); }

QString sdk::home_category::to_string(HomeCategory c)
{
    for (const auto &it : metadata())
        if (it._enum == c)
            return it._full;
    return "NaN";
}

sdk::HomeCategory sdk::home_category::from_string(const QString& s)
{
    for (const auto &it : metadata())
        if (it._full.compare(s, Qt::CaseInsensitive) == 0)
            return it._enum;
    return HomeCategory::Unknown;
}

QStringList sdk::home_category::all_names()
{
    QStringList ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._full);

    return ret;
}

std::vector <sdk::HomeCategory> sdk::home_category::all()
{
    std::vector <sdk::HomeCategory> ret;
    ret.reserve(metadata().size());

    for (const auto &it : metadata())
        ret.push_back(it._enum);

    return ret;
}

const std::vector <sdk::home_category::Meta>& sdk::home_category::metadata()
{
    static std::vector <sdk::home_category::Meta> _;
    if (not _.empty())
        return _;

    _.emplace_back(HomeCategory::Domestic, "Domestic");
    _.emplace_back(HomeCategory::Foreign,  "Foreign");

    return _;
}
