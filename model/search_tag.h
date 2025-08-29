#ifndef MODEL_SEARCH_TAG_H
#define MODEL_SEARCH_TAG_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>
#include "data/market.h"

namespace model { class SearchTag; }

class model::SearchTag : public QAbstractListModel
{
    Q_OBJECT
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    // void add(QString symbol, QString name, QString type);

public slots:
    void find_by_part(QString str);

private:
    SearchTag(QObject* parent = nullptr);
    SearchTag& operator = (const SearchTag&) = delete;

    friend class ::Loader;

    std::vector <data::Instrument*> _list;
};

#endif // MODEL_SEARCH_TAG_H
