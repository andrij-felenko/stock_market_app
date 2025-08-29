#include "ticker_list.h"
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
    CountryTagRole,
    IndustryRole,
    QuoteRole,
    LogoRole,
    LogoUrlRole,
    LogoSizeRole,
    PrimaryTickerRole,
    YearMinRole,
    YearMaxRole,
    CurrencyRole,
    CurrentPriceRole,

    Code,
    Venue,
};

int model::TickerList::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return _tickers.size();
}

QVariant model::TickerList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(_tickers.size()))
        return QVariant();

    data::Ticker* in = _tickers[index.row()];

    switch (role) {
        case TickerRole:   return in->symbol().full();
        case QuoteRole:    return in->quotes()->current();
        case TitleRole:    return in->instrument()->identity()->title();
        case CountryRole:  return ~in->instrument()->meta()->isin_country();
        case CountryTagRole: return geo::country::alpha2(in->instrument()->meta()->isin_country());
        case IndustryRole: return in->instrument()->identity()->industry();
        case LogoRole:     return in->instrument()->identity()->logo();
        case PrimaryTickerRole: return QVariant::fromValue(in->instrument()->primary_ticker());
        case YearMaxRole: return in->quotes()->yearMax();
        case YearMinRole: return in->quotes()->yearMin();
        case CurrencyRole: return in->currency_str();
        case CurrentPriceRole: return in->quotes()->current();
        case LogoUrlRole: return in->instrument()->identity()->logo_url();
        case LogoSizeRole: return in->instrument()->identity()->logo_size();
        case Code: return in->symbol().code();
        case Venue: return in->symbol().venue();
    }
    return "";
}

QHash<int, QByteArray> model::TickerList::roleNames() const
{
    QHash <int, QByteArray> roles;
    roles[TickerRole]  = "ticker";
    roles[TitleRole]   = "title";
    roles[CountryRole] = "country";
    roles[CountryTagRole] = "country_tag";
    roles[IndustryRole]= "industry";
    roles[QuoteRole]   = "prices";
    roles[LogoRole]    = "logo";
    roles[PrimaryTickerRole] = "primary_ticker";
    roles[YearMaxRole] = "year_max";
    roles[YearMinRole] = "year_min";
    roles[CurrencyRole] = "currency";
    roles[CurrentPriceRole] = "price";
    roles[LogoUrlRole] = "logo_url";
    roles[LogoSizeRole] = "logo_size";
    roles[Code] = "symbol_code";
    roles[Venue] = "symbol_venue";
    return roles;
}

void model::TickerList::updateAllData()
{
    beginResetModel();
    endResetModel();

    if (rowCount() > 0)
        emit dataChanged(index(0, 0), index(rowCount() - 1, 0), roleNames().keys());
}
