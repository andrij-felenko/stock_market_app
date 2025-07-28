#ifndef DATA_MARKET_H
#define DATA_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "instrument/ticker.h"

namespace data {
    class Market;
    struct TickerMeta {
        QString symbol;
        QString name;
        QString type;
        QString region;
        QString currency;
        QString exchange;
    };

    using TickerMetaList = std::vector <TickerMeta>;
}
namespace model { class SearchTag; }


class data::Market : public QObject
{
    Q_OBJECT
public:
    static Market* instance();

    std::vector <Instrument*> search_by(QString str) const;
    static std::optional <Ticker*> find(QString tag);
    static Instrument* const add(QString tag);
    static void              add(TickerMeta meta);

    // sorted instruments list basic data
    void load_instruments();
    void save_instruments();
    void write_instrument(TickerMetaList list);

    void load_ticker_meta();
    void save_ticker_meta();
    void write_ticker_meta(TickerMetaList list);

    // parse ticker meta for create instruments
    void clusterise_ticker_meta();
    void clusterise_ticker_meta(TickerMetaList metalist);

signals:
    void tickerMetaLoadFinish();

private:
    Market(QObject* parent = nullptr);
    Market& operator = (const Market&) = delete;

    void _load_data();

    std::vector <Instrument*> _instruments;
    std::vector <TickerMeta> _ticker_meta;
};

#endif
