#ifndef DATA_MARKET_H
#define DATA_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "ticker.h"

namespace data { class Market; }

class data::Market : public QAbstractListModel
{
    Q_OBJECT
public:
    static Market* instance();

    static std::optional <Ticker*> find(QString tag);
    static std::vector   <Ticker*> all();

    static void add(QString tag);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    Market(QObject* parent = nullptr);
    Market& operator = (const Market&) = delete;

    void load_from_local_data();

    std::vector <Instrument*> _instruments;
    QTimer* _timer;
};

#endif
