#ifndef MODEL_ASSET_LIST_H
#define MODEL_ASSET_LIST_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "data/user/stock.h"

namespace model { class AssetList; }

class model::AssetList : public QAbstractListModel
{
    Q_OBJECT
public:
    AssetList(std::vector <data::Stock*>& list, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector <data::Stock*>& _assets;
};

#endif // DATA_PORTFOLIO_H
