#include "favorite.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "api/finnhub.h"
#include <QTimer>
#include "data/instrument.h"

data::Favorite* data::Favorite::instance()
{
    static Favorite* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new Favorite(qApp);
    }
    return _instance;
}

data::Favorite::Favorite(QObject* parent) : QAbstractListModel(parent)
{
    _stocks.clear();
    _stocks.reserve(2000);
}

int data::Favorite::rowCount(const QModelIndex& parent) const
{
    return _stocks.size();
}

QVariant data::Favorite::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_stocks.size()))
        return QVariant();

    Stock* stock = _stocks[index.row()];

    switch (role) {
        case TickerRole:  return stock->ticker()->symbol();
        case QuoteRole:   return stock->ticker()->quotes()->current();
        case TitleRole:   return stock->instrument()->identity()->title();
        case CountryRole: return stock->instrument()->identity()->country();
        case IndustryRole:return stock->instrument()->identity()->industry();
        case LogoRole:    return stock->instrument()->identity()->logo();
        default:          return QVariant();
    }
}

QHash<int, QByteArray> data::Favorite::roleNames() const
{
    QHash <int, QByteArray> roles;
    roles[TickerRole]  = "ticker";
    roles[TitleRole]   = "title";
    roles[CountryRole] = "country";
    roles[IndustryRole]= "industry";
    roles[QuoteRole]   = "price";
    roles[LogoRole]    = "logo";
    return roles;
}
