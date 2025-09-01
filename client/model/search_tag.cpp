#include "search_tag.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "data/instrument.h"
#include "data/market.h"
#include "loader.h"
#include "data/geo/geo.h"

enum Tag {
    NameRole = Qt::UserRole + 1,
    TagRole,
    TypeRole,
    RegionRole,
    CurrencyRole,
    ExchangeRole,
    TickerSize,
    CodeRole,
    VenueRole,
};

model::SearchTag::SearchTag(QObject* parent) : QAbstractListModel(parent)
{
    //
}

int model::SearchTag::rowCount(const QModelIndex& parent) const
{
    return _list.size();
}

QVariant model::SearchTag::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_list.size()))
        return QVariant();

    switch (role) {
        case TagRole:      return _list[index.row()]->primary_symbol(true).full();
        case NameRole:     return _list[index.row()]->meta()->title();
        case TypeRole:     return _list[index.row()]->meta()->type_str();
        case ExchangeRole: return _list[index.row()]->primary_symbol(true).venue();
        case CodeRole:     return _list[index.row()]->primary_symbol().code();
        case VenueRole:    return _list[index.row()]->primary_symbol().venue();
        case TickerSize: {
            // TODO make list of values of symbols
            QStringList list;
            for (const auto& it : _list[index.row()]->tickers())
                list.push_back(it.full());
            if (not list.isEmpty())
                return list.join(" | ");
            return 0;
        }
        default:           return QVariant();
    }
}

QHash<int, QByteArray> model::SearchTag::roleNames() const
{
    QHash <int, QByteArray> roles;
    roles[TagRole]      = "symbol";
    roles[NameRole]     = "title";
    roles[TypeRole]     = "type";
    roles[RegionRole]   = "region";
    roles[CurrencyRole] = "currency";
    roles[ExchangeRole] = "exchange";
    roles[TickerSize]   = "ticker_size";
    roles[CodeRole]   = "symbol_code";
    roles[VenueRole]   = "symbol_venue";
    return roles;
}

void model::SearchTag::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _list.clear();
    endRemoveRows();
}

// void model::SearchTag::add(QString symbol, QString name, QString type)
// {
//     beginInsertRows(QModelIndex(), rowCount(), rowCount());
//     meta::Ticker m;
//     m.symbol = symbol;
//     m.name = name;
//     m.type = sdk::instype::from_string(type);
//     _list.push_back(m);
//     endInsertRows();
// }

void model::SearchTag::find_by_part(QString str)
{
    qDebug() << "find by part" << str;
    clear();
    if (str.isEmpty())
        return;

    auto list = Loader::instance()->market()->search_by(str);

    // --------------------------------------------------------------
    beginInsertRows(QModelIndex(), 0, list.size() - 1);

    for (const auto& it : std::as_const(list))
        _list.push_back(it);

    // for (const auto& it : _list)
        // _list.push_back(it);

    endInsertRows();
}
