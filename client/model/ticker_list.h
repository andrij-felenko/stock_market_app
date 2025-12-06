#ifndef MODEL_TICKER_LIST_H
#define MODEL_TICKER_LIST_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include <core/security/instrument.h>

namespace model { class TickerList; }

class model::TickerList : public QAbstractListModel
{
    Q_OBJECT
public:
    template <typename Sender, typename Signal>
    TickerList(std::vector <sdk::Ticker*>& list, Sender* source, Signal signal)
        : QAbstractListModel(source), _tickers(list)
    {
        connect(source, signal, this, &TickerList::updateAllData);

        updateAllData();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector <sdk::Ticker*>& _tickers;

    void updateAllData();

    friend class Portfolio;
};

#endif
