#ifndef DATA_USER_H
#define DATA_USER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "instrument/instrument.h"
// #include "user/stock.h"
#include "sdk_def.h"

class sdk::User
{
    // Q_OBJECT
// public:
//     std::vector <data::Ticker*>& favorite_list();
//     std::vector <data::Stock*>&     asset_list();

//     void save();
//     void load();

//     void addToFavorite(const ticker::Symbol& symbol);

// public slots:
//     void addToFavorite(const QString& symbol);
//     void addToFavorite(const QString& code, const QString& venue);
//     void excludeFromFavorite(const QString& code, const QString& venue);
//     bool isInAssetList(const QString& symbol);

// signals:
//     void    assetListUpdated();
//     void favoriteListUpdated();

// private:
//     User(QObject* parent = nullptr);
//     User& operator = (const User&) = delete;
//     void clear();

//     std::vector <data::Ticker*> _favorite_list;
//     std::vector <data::Stock*>     _asset_list;

//     friend class sdk::Loader;
//     friend class model::AssetList;
//     friend class model::TickerList;

//     friend QDataStream& operator << (QDataStream& s, const User& d);
//     friend QDataStream& operator >> (QDataStream& s,       User& d);
};

// namespace data { User* account(); }

#endif
