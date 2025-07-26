#include "search_tag.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "data/instrument.h"
#include "data/market.h"

enum Tag {
    NameRole = Qt::UserRole + 1,
    TagRole,
    TypeRole,
    RegionRole,
    CurrencyRole,
    ExchangeRole,
    TickerSize,
};

model::SearchTag* model::SearchTag::instance()
{
    static SearchTag* _instance = nullptr;
    if (_instance == nullptr){
        _instance = new SearchTag(qApp);
    }
    return _instance;
}

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
        case TagRole:      return _list[index.row()].symbol;
        case NameRole:     return _list[index.row()].name;
        case TypeRole:     return _list[index.row()].type;
        case RegionRole:   return _list[index.row()].region;
        case CurrencyRole: return _list[index.row()].currency;
        case ExchangeRole: return _list[index.row()].exchange;
        case TickerSize: {
            auto ticker = data::Market::instance()->find(_list[index.row()].symbol);
            if (ticker.has_value())
                return ticker.value()->instrument()->tickers().join(" | ");
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
    return roles;
}

void model::SearchTag::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _list.clear();
    endRemoveRows();
}

void model::SearchTag::add(QString symbol, QString name,
                           QString type, QString region,
                           QString currency)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    data::TickerMeta m;
    m.symbol = symbol;
    m.name = name;
    m.type = type;
    m.region = region;
    m.currency = currency;
    _list.push_back(m);
    endInsertRows();
}

void model::SearchTag::find_by_part(QString str)
{
    qDebug() << "find by part" << str;
    clear();
    if (str.isEmpty())
        return;

    auto list = data::Market::instance()->search_by(str);

    // --------------------------------------------------------------
    beginInsertRows(QModelIndex(), 0, list.size() - 1);

    for (const auto& it : std::as_const(list))
        _list.emplace_back(*it);

    for (const auto& it : _list)
        _list.push_back(it);

    endInsertRows();
}
