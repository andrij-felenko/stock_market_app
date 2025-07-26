#ifndef MODEL_INSTRUMENT_LIST_H
#define MODEL_INSTRUMENT_LIST_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "data/instrument.h"

namespace model { class InstrumentList; }

class model::InstrumentList : public QAbstractListModel
{
    Q_OBJECT
public:
    InstrumentList(std::vector <data::Instrument*>& list, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector <data::Instrument*>& _instruments;

    friend class Portfolio;
};

#endif
