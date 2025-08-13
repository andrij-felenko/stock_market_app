#include "user.h"
#include <QtCore/QCoreApplication>
#include "api/alphavantage.h"
#include "api/finnhub.h"
#include "market.h"
#include <QStandardPaths>
#include <qdir.h>
#include "api/eod.h"
#include "loader.h"
#include "utilities/features.h"

std::vector<data::Ticker*>& data::User::favorite_list() { return _favorite_list; }
std::vector<data::Stock*>&  data::User::   asset_list() { return    _asset_list; }

void data::User::save()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/user";
    QDir().mkpath(path);

    QFile file(path + "/" + username() + ".tdsm");
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);
    out << *this;
    file.close();
    qDebug() << "Save to: " << file.fileName();
}

void data::User::load()
{
    clear();

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/user";
    QDir().mkpath(path);

    QFile file(path + "/" + username() + ".tdsm");
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_10);
    in >> *this;
    file.close();
    qDebug() << "Load from: " << file.fileName();
}

void data::User::addToFavorite(const QString& symbol){ addToFavorite(ticker::Symbol(symbol)); }

bool data::User::isInAssetList(const QString& symbol)
{
    for (const auto& it : _asset_list)
        if (it->ticker()->symbol() == ticker::Symbol(symbol))
            return true;
    return false;
}

void data::User::addToFavorite(const ticker::Symbol& symbol)
{
    auto ticker = Loader::instance()->market()->find(symbol);
    if (not ticker.has_value())
        return;

    connect(ticker.value(), &Ticker::update_data, this, &User::favoriteListUpdated);
    _favorite_list.push_back(ticker.value());
    emit favoriteListUpdated();
    save();

    if (ticker.value()->quotes()->empty())
        api::Eod::historical_year(symbol, 1);

    if (not ticker.value()->instrument()->have_fundamental()){
        auto list = ticker.value()->instrument()->tickers();
        for (const auto& it : list)
            if (it.us())
                api::FinnHub::update_info_by_tag(it.code());
                // api::AlphaVantage::update_info_by_tag(it.code());
    }
}

void data::User::excludeFromFavorite(const QString& symbol)
{
    std::erase_if(_favorite_list, [&](const data::Ticker* t){
        return t && t->symbol() == ticker::Symbol(symbol);
    });
    emit favoriteListUpdated();
    save();
}

data::User::User(QObject* parent) : data::user::Info(parent)
{
    load();
}

void data::User::clear()
{
    _favorite_list.clear();
    _asset_list.clear();
    Info::clear();
}

namespace data {
    QDataStream& operator << (QDataStream& s, const User& d){
        ticker::SymbolList favlist;
        for (const auto& it : std::as_const(d._favorite_list))
            favlist.push_back(it->symbol());
        util::list_to_stream(s, favlist);

        qDebug() << "USER save list from QDataStream" << favlist;

        util::list_to_stream(s, d._asset_list);
        return s << static_cast <const user::Info&>(d);
    }

    QDataStream& operator >> (QDataStream& s, User& d){
        ticker::SymbolList favlist;
        util::list_from_stream(s, favlist);
        for (const auto& it : std::as_const(favlist))
            d.addToFavorite(it);

        qDebug() << "USER load list from QDataStream" << favlist;

        util::list_from_stream(s, d._asset_list);
        return s >> static_cast <user::Info&>(d);
    }
}
