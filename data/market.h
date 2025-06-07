#ifndef DATA_MARKET_H
#define DATA_MARKET_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "ticker.h"

namespace data { class Market; }

class data::Market : public QObject
{
    Q_OBJECT
public:
    static Market* instance();

    static std::optional <Ticker*> find(QString tag);
    static std::vector   <Ticker*> all();

    static void add(QString tag);

private:
    Market(QObject* parent = nullptr);
    Market& operator = (const Market&) = delete;

    void load_from_local_data();

    std::vector <Ticker*> _tickers;
};

#endif
