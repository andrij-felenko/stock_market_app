// #include "data/user.h"
// #include <QtCore/QCoreApplication>
// #include "api/alphavantage.h"
// #include "api/finnhub.h"
// #include "data/market.h"
// #include <QStandardPaths>
// #include <QDir>
// #include "api/eod.h"
// #include "loader.h"
// #include "utilities/features.h"

// std::vector<data::Ticker*>& data::User::favorite_list() { return _favorite_list; }
// std::vector<data::Stock*>&  data::User::   asset_list() { return    _asset_list; }

// void data::User::save()
// {
//     QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     path += "/user";
//     QDir().mkpath(path);

//     QFile file(path + "/" + username() + ".tdsm");
//     if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
//         return;

//     QDataStream out(&file);
//     out.setVersion(QDataStream::Qt_6_10);
//     out << *this;
//     file.close();
//     qDebug() << "Save to: " << file.fileName();
// }

// void data::User::load()
// {
//     clear();

//     QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     path += "/user";
//     QDir().mkpath(path);

//     QFile file(path + "/" + username() + ".tdsm");
//     if (!file.open(QIODevice::ReadOnly))
//         return;

//     QDataStream in(&file);
//     in.setVersion(QDataStream::Qt_6_10);
//     in >> *this;
//     file.close();
//     qDebug() << "Load from: " << file.fileName();
// }

// void data::User::addToFavorite(const QString& symbol){
//     qDebug() << Q_FUNC_INFO << symbol;
//     addToFavorite(ticker::Symbol(symbol));
// }

// void data::User::addToFavorite(const QString& code, const QString& venue){
//     addToFavorite(ticker::Symbol(code, venue));
// }

// bool data::User::isInAssetList(const QString& symbol)
// {
//     for (const auto& it : _asset_list)
//         if (it->ticker()->symbol() == ticker::Symbol(symbol))
//             return true;
//     return false;
// }

// void data::User::addToFavorite(const ticker::Symbol& symbol)
// {
//     qDebug() << Q_FUNC_INFO << symbol;
//     auto t = Nexus.market()->find(symbol);
//     if (not t.has_value()){
//         qDebug() << "Not found symbol" << symbol;
//         return;
//     }

//     Ticker* ticker = t.value();

//     connect(ticker, &Ticker::signal_update, this, &User::favoriteListUpdated);
//     _favorite_list.push_back(ticker);
//     emit favoriteListUpdated();
//     save();

//     qDebug() << Q_FUNC_INFO << ticker->quotes()->raw_points().size() << symbol << ticker;
//     if (ticker->quotes()->empty())
//         api::Eod::historical_year(symbol, 30);

//     if (not ticker->instrument()->have_fundamental()){
//         auto list = ticker->instrument()->tickers();
//         for (const auto& it : list){
//             api::Eod::fundamental(symbol);
//         }
//     }
// }

// void data::User::excludeFromFavorite(const QString& code, const QString& venue)
// {
//     std::erase_if(_favorite_list, [&](const data::Ticker* t){
//         return t && t->symbol() == ticker::Symbol(code, venue);
//     });
//     emit favoriteListUpdated();
//     save();
// }

// data::User::User(QObject* parent) : data::user::Info(parent)
// {
//     load();
// }

// void data::User::clear()
// {
//     _favorite_list.clear();
//     _asset_list.clear();
//     Info::clear();
// }

// namespace data {
//     QDataStream& operator << (QDataStream& s, const User& d){
//         ticker::SymbolList favlist;
//         for (const auto& it : std::as_const(d._favorite_list))
//             favlist.push_back(it->symbol());
//         util::list_to_stream(s, favlist);

//         qDebug() << "USER save list from QDataStream" << favlist;

//         util::list_to_stream(s, d._asset_list);
//         return s << static_cast <const user::Info&>(d);
//     }

//     QDataStream& operator >> (QDataStream& s, User& d){
//         ticker::SymbolList favlist;
//         util::list_from_stream(s, favlist);
//         for (const auto& it : std::as_const(favlist))
//             d.addToFavorite(it);

//         qDebug() << "USER load list from QDataStream" << favlist;

//         util::list_from_stream(s, d._asset_list);
//         return s >> static_cast <user::Info&>(d);
//     }
// }
