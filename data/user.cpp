#include "user.h"
#include <QtCore/QCoreApplication>
#include "market.h"
#include <QStandardPaths>
#include <qdir.h>
#include "api/eod.h"
#include "loader.h"

std::vector<data::Instrument*>& data::User::favorite_list() { return _favorite_list; }
std::vector<data::Stock*>&      data::User::   asset_list() { return    _asset_list; }

void data::User::save()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/user";
    QDir().mkpath(path);

    QFile file(path + "/" + username() + ".tdsm");
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);
    out << *this;
    file.close();
    qDebug() << "Save to: " << file.fileName();
}

void data::User::load()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/user";
    QDir().mkpath(path);

    QFile file(path + "/" + username() + ".tdsm");
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);
    in >> *this;
    file.close();
    qDebug() << "Load from: " << file.fileName();
}

void data::User::addToFavorite(const ticker::Symbol& symbol)
{
    auto ticker = Loader::instance()->market()->find(symbol);
    if (not ticker.has_value())
        return;

    if (ticker.value()->quotes()->empty())
        api::Eod::historical_year(symbol, 1);
    _favorite_list.push_back(ticker.value()->instrument());
    emit favoriteListUpdated();
    save();
}

data::User::User(QObject* parent) : data::user::Info(parent)
{
    load();

    connect(Loader::instance()->market(), &Market::tickerMetaLoadFinish, this, &User::load);
}

namespace data {
    QDataStream& operator << (QDataStream& s, const User& d){
        // TODO add asset
        // list.clear();
        // for (const auto& it : d._asset_list)
        //     list.push_back(it->primary_ticker()->symbol());
        // s << list;

        return s << static_cast <const user::Info&>(d);
    }

    QDataStream& operator >> (QDataStream& s, User& d){
        QStringList list;
        s >> list;
        qDebug() << ">>" << list;
        for (const auto& it : std::as_const(list))
            d.addToFavorite(it);

        // util::import_list(s, d._asset_list);
        // util::import_list(s, d._favorite_list);
        return s >> static_cast <user::Info&>(d);
    }
}
