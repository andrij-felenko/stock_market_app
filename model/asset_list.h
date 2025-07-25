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
    static AssetList* instance();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    AssetList(QObject* parent = nullptr);
    AssetList& operator = (const AssetList&) = delete;

    std::vector <data::Stock*> _assets;
};

#endif // DATA_PORTFOLIO_H
