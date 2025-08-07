#include "asset_list.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "api/finnhub.h"
#include <QTimer>
#include "data/instrument.h"

enum AssetRoles {
    TickerRole = Qt::UserRole + 50,
    TitleRole,
    CountryRole,
    IndustryRole,
    QuoteRole,
    LogoRole,
};

model::AssetList::AssetList(std::vector <data::Stock*>& list, QObject* parent)
    : QAbstractListModel(parent), _assets(list)
{
    _assets.clear();
    _assets.reserve(2000);
}

// // pdsm - portfolio data stock manager
// void data::Portfolio::save() const
// {
//     QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     QDir().mkpath(basePath);
//     QString filename = basePath + "/guest" + ".pdsm";

//     QFile file(filename);
//     qDebug() << "file" << file.fileName();
//     if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
//         return;

//     QDataStream out(&file);
//     out.setVersion(QDataStream::Qt_6_0);
//     out << int32_t(_stocks.size());
//     for (const auto& it : _stocks)
//         out << *it;

//     file.close();
//     qDebug() << "Save to: " << filename;
// }

// void data::Portfolio::load()
// {
//     QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     QString filename = basePath + "/guest" + ".pdsm";

//     QFile file(filename);
//     if (!file.open(QIODevice::ReadOnly))
//         return;

//     QDataStream in(&file);
//     in.setVersion(QDataStream::Qt_6_0);
//     int32_t size = 0;
//     in >> size;
//     if (size > 0){
//         beginInsertRows(QModelIndex(), rowCount(), rowCount() + size - 1);
//         for (int i = 0; i < size; i++){
//             Stock* s = new Stock(this);
//             in >> *s;
//         }
//         endInsertRows();
//     }
//     file.close();
// }

int model::AssetList::rowCount(const QModelIndex& parent) const
{
    return _assets.size();
}

QVariant model::AssetList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_assets.size()))
        return QVariant();

    data::Stock* stock = _assets[index.row()];

    switch (role) {
        case TickerRole:  return stock->ticker()->symbol().full();
        case QuoteRole:   return stock->ticker()->quotes()->current();
        case TitleRole:   return stock->instrument()->identity()->title();
        case CountryRole: return stock->instrument()->identity()->country();
        case IndustryRole:return stock->instrument()->identity()->industry();
        case LogoRole:    return stock->instrument()->identity()->logo();
        default:          return QVariant();
    }
}

QHash<int, QByteArray> model::AssetList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TickerRole]   = "ticker";
    roles[TitleRole]    = "title";
    roles[CountryRole]  = "country";
    roles[IndustryRole] = "industry";
    roles[QuoteRole]    = "price";
    roles[LogoRole]     = "logo";
    return roles;
}
