#ifndef DATA_PORTFOLIO_H
#define DATA_PORTFOLIO_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "stock.h"

namespace data { class Portfolio; }

class data::Portfolio : public QAbstractListModel
{
    Q_OBJECT
public:
    static Portfolio* instance();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void save() const;
    void load();

signals:
    void update_data();

private:
    Portfolio(QObject* parent = nullptr);
    Portfolio& operator = (const Portfolio&) = delete;

    std::vector <Stock*> _stocks;
};

#endif // DATA_PORTFOLIO_H
