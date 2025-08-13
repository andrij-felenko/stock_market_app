#include "asset_list.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
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
