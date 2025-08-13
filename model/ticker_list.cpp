#include "instrument_list.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "api/finnhub.h"
#include <QTimer>
#include "data/instrument.h"

enum InstrumentRoles {
    TickerRole = Qt::UserRole + 1,
    TitleRole,
    CountryRole,
    IndustryRole,
    QuoteRole,
    LogoRole,
    PrimaryTickerRole,
    YearMinRole,
    YearMaxRole,
    CurrencyRole,
    CurrentPriceRole,
};

int model::InstrumentList::rowCount(const QModelIndex& parent) const
{
    return _instruments.size();
}

QVariant model::InstrumentList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_instruments.size()))
        return QVariant();

    data::Instrument* in = _instruments[index.row()];

    switch (role) {
        case TickerRole:   return in->primary_ticker()->symbol().full();
        case QuoteRole:    return in->primary_ticker()->quotes()->current();
        case TitleRole:    return in->identity()->title();
        case CountryRole:  return in->identity()->country();
        case IndustryRole: return in->identity()->industry();
        case LogoRole:     return in->identity()->logo();
        case PrimaryTickerRole: return QVariant::fromValue(in->primary_ticker());
        case YearMaxRole: return in->primary_ticker()->quotes()->year_max();
        case YearMinRole: return in->primary_ticker()->quotes()->year_min();
        case CurrencyRole: return in->primary_ticker()->currency_str();
        case CurrentPriceRole: return in->primary_ticker()->quotes()->current();
        default:           return QVariant();
    }
}

QHash<int, QByteArray> model::InstrumentList::roleNames() const
{
    QHash <int, QByteArray> roles;
    roles[TickerRole]  = "ticker";
    roles[TitleRole]   = "title";
    roles[CountryRole] = "country";
    roles[IndustryRole]= "industry";
    roles[QuoteRole]   = "price";
    roles[LogoRole]    = "logo";
    roles[PrimaryTickerRole] = "primary_ticker";
    roles[YearMaxRole] = "year_max";
    roles[YearMinRole] = "year_min";
    roles[CurrencyRole] = "currency";
    roles[CurrentPriceRole] = "price";
    return roles;
}

void model::InstrumentList::dataUpdated()
{
    beginResetModel();
    endResetModel();
}
