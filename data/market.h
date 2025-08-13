#ifndef DATA_MARKET_H
#define DATA_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "instrument/ticker.h"
#include "meta.h"

class Loader;
namespace data  { class Market; }
namespace model { class SearchTag; }

class data::Market : public QObject
{
    Q_OBJECT
public:
    std::vector <Instrument*> search_by(QString str) const;
    std::optional <Ticker*> find(ticker::Symbol tag);
    Instrument* const ensure(ticker::Symbol tag);
    Instrument* const ensure(meta::Ticker meta);
    void              add_meta(meta::Ticker meta);



    // sorted instruments list basic data
    void load_instruments();
    void save_instruments();
    void write_instrument(TickerMetaList list);
    bool empty() const;

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

    void add_instrument_list_from_meta(QByteArrayList list, bool force = false);
    void add_sorted_instrument(const ticker::Symbol main, const TickerMetaList list);

    std::vector <Instrument*>  _instruments;
    std::vector <meta::Ticker> _ticker_meta;

    friend class ::Loader;
};

#endif
