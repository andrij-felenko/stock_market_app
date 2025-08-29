#ifndef DATA_MARKET_H
#define DATA_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "instrument/ticker.h"
#include "instrument/meta.h"

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
    Instrument* const ensure(const data::Meta& meta, ticker::Symbol symbol);

    void detect_main_ticker();

    void load_meta();
    void save_meta() const;
    bool empty() const;

signals:
    void metaLoaded();

private:
    Market(QObject* parent = nullptr);
    Market& operator = (const Market&) = delete;

    std::vector <Instrument*>  _instruments;

    friend class ::Loader;
};

#endif
