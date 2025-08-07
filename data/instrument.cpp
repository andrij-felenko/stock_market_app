#include "instrument.h"
#include "data/market.h"
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "utilities/features.h"

enum InstRole {
    Ticker,
    TickerObject,
    Exchange,
    IsPrimary,
    Currency,
    Region
};

data::Instrument::Instrument(const ticker::Symbol& tag, QObject* parent) : QObject(parent)
{
    _dividend = new Dividend(this);
    _identity = new Identity(this);
    _stability = new Stability(this);
    _valuation = new Valuation(this);
    _profitability = new Profitability(this);
    _tickers.reserve(100);

    ensure(tag);
}

data::Instrument::Instrument(const meta::Ticker& meta, QObject* parent)
    : Instrument(meta.symbol, parent)
{
    _identity->set_title(meta.name);
    _identity->set_country(meta.region);

    primary_ticker(true)->setCurrency(currency::Name::from_short(meta.currency));
}

data::ticker::Symbol data::Instrument::primary_symbol(bool absolute) const
{
    if (not absolute){
        // TODO add check user primary ticker settings
    }

    return _tickers[0]->symbol();
}

data::Ticker* data::Instrument::primary_ticker(bool absolute) const
{
    if (not absolute){
        // TODO add check user primary ticket settings
    }

    return _tickers[0];
}

data::ticker::SymbolList data::Instrument::tickers() const
{
    ticker::SymbolList ret;
    ret.reserve(50);
    for (const auto& it : _tickers)
        if (not ret.contains(it->symbol()))
            ret.emplace_back(it->symbol());
    return ret;
}

bool data::Instrument::contains(const ticker::Symbol& symbol) const
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
    qDebug() << Q_FUNC_INFO;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/stocks";
    QDir().mkpath(path);

    QFile file(path + "/" + primary_symbol(true).full() + ".tdsm");
    qDebug() << "file" << file.fileName();
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        qDebug() << "data::Instrument::save() can`t open file"
                 << file.fileName() << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);
    out << *this;
    file.close();
    qDebug() << "data::Instrument::save() to: " << file.fileName();
}

void data::Instrument::load()
{
    qDebug() << Q_FUNC_INFO;
    std::function load_data = [this](QString path){
        QFile file(path + "/stocks/" + primary_symbol(true).full() + ".tdsm");
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);
        in >> *this;
        file.close();
    };

    load_data(":/rc");
    load_data(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    // if (not primary_ticker()->quotes()->empty())
        qDebug() << "load instrument" << primary_symbol().full()
                 << primary_ticker()->quotes()->points().size()
                 << primary_ticker()->quotes()->current();
}

data::Ticker* const data::Instrument::operator[](ticker::Symbol symbol) const
{
    // qDebug() << Q_FUNC_INFO << symbol << _tickers.size();
    for (const auto& it : _tickers)
        if (it->symbol() == symbol){
            // qDebug() << "[" << symbol << "]" << it;
            return it;
        }
    return nullptr;
}

data::Ticker* const data::Instrument::ensure(ticker::Symbol symbol)
{
    qDebug() << "ensure" << symbol << _tickers.size();
    Ticker* t = operator[](symbol);
    if (t == nullptr){
        t = new Ticker(_tickers.empty(), this);
        t->set_symbol(symbol);
        _tickers.push_back(t);
        connect(t, &Ticker::update_data, this, &Instrument::save);
    }
    qDebug() << "ensure 2" << symbol << _tickers.size();
    return t;
}

data::Instrument::operator meta::Ticker() const
{
    meta::Ticker meta;
    meta.name    = _identity->title();
    meta.region  = _identity->country();
    meta.symbol  = _tickers[0]->symbol();
    meta.currency = primary_ticker(true)->currency_str();
    return meta;
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Instrument& d) {
        qDebug() << Q_FUNC_INFO;
        s << *d._dividend  << *d._identity  << *d._profitability
          << *d._stability << *d._valuation;
        util::list_to_stream(s, d._tickers);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Instrument& d) {
        qDebug() << Q_FUNC_INFO;
        s >> *d._dividend  >> *d._identity  >> *d._profitability
          >> *d._stability >> *d._valuation;
        util::list_from_stream(s, d._tickers, false, &d);
        return s;
    }
}
