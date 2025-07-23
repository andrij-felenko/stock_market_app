#ifndef DATA_FAVORITE_H
#define DATA_FAVORITE_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "stock.h"

namespace data { class Favorite; }

class data::Favorite : public QAbstractListModel
{
    Q_OBJECT
public:
    static Favorite* instance();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

protected:
    enum MarketBaseRoles {
        TickerRole = Qt::UserRole + 1,
        TitleRole,
        CountryRole,
        IndustryRole,
        QuoteRole,
        LogoRole,
    };

    std::vector <Ticker*> _tickers;

private:
    Favorite(QObject* parent = nullptr);
    Favorite& operator = (const Favorite&) = delete;

    friend class Portfolio;
};

#endif
