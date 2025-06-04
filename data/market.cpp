#include "market.h"
#include <QtCore/QCoreApplication>

data::Market* data::Market::instance()
{
    static Market* _instance = new Market(qApp);
    return _instance;
}

std::optional <data::Ticker*> data::Market::find(QString tag)
{
    for (auto* t : instance()->_tickers)
        if (t->_ticker == tag)
            return t;
    return std::nullopt;
}

std::vector <data::Ticker*> data::Market::all()
{
    return instance()->_tickers;
}

void data::Market::add(QString tag)
{
    Market* market = instance();
    Ticker* ticker = new Ticker(market);
    ticker->_ticker = tag;
    market->_tickers.emplace_back(ticker);
}

data::Market::Market(QObject* parent) : QObject(parent)
{
    _tickers.reserve(2000);
    // TODO load all data from files
}
