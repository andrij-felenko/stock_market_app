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
    template <typename Sender, typename Signal>
    InstrumentList(std::vector <data::Instrument*>& list, Sender* source, Signal signal)
        : QAbstractListModel(source), _instruments(list)
    {
        connect(source, signal, this, &InstrumentList::dataUpdated);

        _instruments.clear();
        _instruments.reserve(2000);
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector <data::Instrument*>& _instruments;

    void dataUpdated();

    friend class Portfolio;
};

#endif
