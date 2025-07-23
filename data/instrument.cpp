#include "instrument.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

using namespace data::ticker;

enum InstRole {
    Ticker,
    TickerObject,
    Exchange,
    IsPrimary,
    Currency,
    Region
};

data::Instrument::Instrument(QObject* parent) : QAbstractListModel(parent)
{
    _dividend = new ticker::Dividend(this);
    _identity = new ticker::Identity(this);
    _stability = new ticker::Stability(this);
    _valuation = new ticker::Valuation(this);
    _profitability = new ticker::Profitability(this);
}

Dividend*      data::Instrument::dividend()      const { return _dividend; }
Identity*      data::Instrument::identity()      const { return _identity; }
Stability*     data::Instrument::stability()     const { return _stability; }
Valuation*     data::Instrument::valuation()     const { return _valuation; }
Profitability* data::Instrument::profitability() const { return _profitability; }

// tdsm - ticker data stock manager
void data::Instrument::save() const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(basePath);

    QDir dir(basePath);
    if (! dir.cd ("stocks")){
        dir.mkdir("stocks");
        dir.cd   ("stocks");
    }

    QString filename = basePath + "/stocks/" + primary_ticker()->symbol() + ".tdsm";
    QFile file(filename);
    qDebug() << "file" << file.fileName();
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);
    out << *this;
    file.close();
    qDebug() << "Save to: " << filename;
}

void data::Instrument::load()
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filename = basePath + "/" + primary_ticker()->symbol() + ".tdsm";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);
    in >> *this;
    file.close();
}

void data::Instrument::_update_primary_ticket()
{
    for (const auto& it : _tickers)
        if (it->_symbol == _primary_ticker)
            it->_primary = true;
}

void data::Instrument::_add_ticker(Ticker* ticker)
{
    _tickers.push_back(ticker);
    _update_primary_ticket();
    connect(ticker, &Ticker::update_data, this, &Instrument::save);
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Instrument& d) {
        s << *d._dividend  << *d._identity  << *d._profitability
          << *d._stability << *d._valuation << d._primary_ticker;
        s << int32_t(d._tickers.size());
        for (const auto& it : d._tickers)
            s << *it;
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Instrument& d) {
        s >> *d._dividend  >> *d._identity  >> *d._profitability
          >> *d._stability >> *d._valuation >> d._primary_ticker;
        int32_t size;
        s >> size;
        for (int i = 0; i < size; i++){
            Ticker* new_t = new Ticker(false, &d);
            s >> *new_t;
            d._add_ticker(new_t);
        }
        return s;
    }
}
