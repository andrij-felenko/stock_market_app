#include "instrument.h"
#include "data/market.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

enum InstRole {
    Ticker,
    TickerObject,
    Exchange,
    IsPrimary,
    Currency,
    Region
};

data::Instrument::Instrument(QObject* parent) : QObject(parent)
{
    _dividend = new Dividend(this);
    _identity = new Identity(this);
    _stability = new Stability(this);
    _valuation = new Valuation(this);
    _profitability = new Profitability(this);
    _tickers.reserve(10);
}

data::Ticker* data::Instrument::primary_ticker(bool absolute) const
{
    if (not absolute){
        // TODO add check user primary ticket settings
    }

    return operator[](_primary_ticker);
}

QStringList data::Instrument::tickers() const
{
    QStringList ret;
    ret.reserve(50);
    for (const auto& it : _tickers)
        if (not ret.contains(it->symbol()))
            ret.emplace_back(it->symbol());
    return ret;
}

bool data::Instrument::contains(const QString& symbol) const
{
    for (const auto& it : _tickers)
        if (it->_symbol == symbol)
            return true;
    return false;
}

data::Dividend*      data::Instrument::dividend()      const { return _dividend; }
data::Identity*      data::Instrument::identity()      const { return _identity; }
data::Stability*     data::Instrument::stability()     const { return _stability; }
data::Valuation*     data::Instrument::valuation()     const { return _valuation; }
data::Profitability* data::Instrument::profitability() const { return _profitability; }

// tdsm - ticker data stock manager
void data::Instrument::save() const
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "stocks";
    QDir().mkpath(path);

    QFile file(path + "/" + _primary_ticker + ".tdsm");
    qDebug() << "file" << file.fileName();
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);
    out << *this;
    file.close();
    qDebug() << "Save to: " << file.fileName();
}

void data::Instrument::load()
{
    std::function load_data = [this](QString path){
        QFile file(path + "stocks/" + _primary_ticker + ".tdsm");
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);
        in >> *this;
        file.close();
    };

    load_data(":/rc/");
    load_data(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}

data::Ticker* const data::Instrument::operator[](const QString& symbol) const
{
    for (const auto& it : _tickers)
        if (it->symbol().compare(symbol, Qt::CaseInsensitive) == 0)
            return it;
    return nullptr;
}

data::Instrument::operator data::TickerMeta() const
{
    TickerMeta meta;
    meta.name = _identity->title();
    meta.currency = primary_ticker(true)->currency_str();
    meta.region = _identity->country();
    meta.symbol = _primary_ticker;
    meta.exchange = primary_ticker(true)->exchange();
    return meta;
}

data::Ticker* const data::Instrument::get(const QString& symbol, bool createif, bool prime)
{
    for (const auto& it : _tickers)
        if (it->symbol() == symbol)
            return it;

    if (not createif)
        return nullptr;

    Ticker* ticker = new Ticker(prime, this);
    ticker->_symbol = symbol;
    _tickers.push_back(ticker);
    return ticker;
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
