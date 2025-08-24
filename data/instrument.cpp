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

data::Instrument::Instrument(const ticker::Symbol& tag, QObject* parent)
    : QObject(parent), _save_locker(false), _was_loaded(false)
{
    _balance = new Balance(this);
    _dividend = new Dividend(this);
    _earnings = new Earnings(this);
    _identity = new Identity(this);
    _profitability = new Profitability(this);
    _shares = new Shares(this);
    _stability = new Stability(this);
    _valuation = new Valuation(this);

    _tickers.reserve(100);

    ensure(tag);
}

data::Instrument::Instrument(const meta::Ticker& meta, QObject* parent)
    : Instrument(meta.symbol, parent)
{
    _identity->set_title(meta.name);
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
        if (not std::ranges::any_of(ret, [&](const auto& s){ return s == it->symbol(); }))
            ret.emplace_back(it->symbol());
    return ret;
}

std::vector <data::Ticker*> data::Instrument::tickers_ptr()
{
    return _tickers;
}

bool data::Instrument::contains(const ticker::Symbol& symbol) const
{
    for (const auto& it : _tickers)
        if (it->_symbol == symbol)
            return true;
    return false;
}

bool data::Instrument::was_loaded() const { return _was_loaded; }
bool data::Instrument::have_fundamental() const
{
    qDebug() << Q_FUNC_INFO << _identity->filled_capacity() << primary_symbol(true);
    return _identity->filled_capacity() > 30;
}

data::Balance*       data::Instrument::balance()       const { return _balance; }
data::Dividend*      data::Instrument::dividend()      const { return _dividend; }
data::Earnings*      data::Instrument::earnings()      const { return _earnings; }
data::Identity*      data::Instrument::identity()      const { return _identity; }
data::Profitability* data::Instrument::profitability() const { return _profitability; }
data::Shares*        data::Instrument::shares()        const { return _shares; }
data::Stability*     data::Instrument::stability()     const { return _stability; }
data::Valuation*     data::Instrument::valuation()     const { return _valuation; }

// tdsm - ticker data stock manager
void data::Instrument::save() const
{
    if (_save_locker)
        return;

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
    out.setVersion(QDataStream::Qt_6_10);
    out << *this;
    file.close();
    qDebug() << "data::Instrument::save() to: " << file.fileName()
             << primary_ticker()->quotes()->raw_points().size();
}

void data::Instrument::load()
{
    _save_locker = true;
    std::function load_data = [this](QString path){
        QFile file(path + "/stocks/" + primary_symbol(true).full() + ".tdsm");
        if (!file.open(QIODevice::ReadOnly))
            return;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_10);
        in >> *this;
        file.close();
        _was_loaded = true;

        for (const auto& it : _tickers)
            emit it->signal_update();
    };

    load_data(":/rc");
    load_data(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    _save_locker = false;
}

data::Ticker* const data::Instrument::operator[](ticker::Symbol symbol) const
{
    for (const auto& it : _tickers)
        if (it->symbol() == symbol){
            return it;
        }
    return nullptr;
}

data::Ticker* const data::Instrument::ensure(ticker::Symbol symbol)
{
    Ticker* t = operator[](symbol);
    if (t == nullptr){
        t = new Ticker(this);
        t->set_symbol(symbol);
        _tickers.push_back(t);
        connect(t, &Ticker::signal_save, this, &Instrument::save);
        if (symbol == ticker::Symbol("DTREF.US"))
            qDebug() << "CREATE DTREF.US" << this << t << &t;
    }
    return t;
}

data::Instrument::operator meta::Ticker() const
{
    meta::Ticker meta;
    meta.name    = _identity->title();
    meta.symbol  = _tickers[0]->symbol();
    return meta;
}

void data::Instrument::fix_tickers_load()
{
    std::unordered_map<std::string, data::Ticker*> firstBySymbol;

    std::erase_if(_tickers, [&](data::Ticker* t){
        const std::string key = t->symbol_str().toStdString();

        auto [it, inserted] = firstBySymbol.emplace(key, t);
        if (inserted)
            return false;               // перший раз бачимо цей символ — залишаємо

        // дубль: зливаємо стан у перший і видаляємо дубль
        *it->second = *t;               // потребує Ticker::operator= і Quotes::operator=
        delete t;                       // чистий C++
        return true;                    // прибрати з вектора
    });
}

void data::Instrument::updatePrimarySymbol(const QString& primary)
{
    for (std::size_t i = 0; i < _tickers.size(); ++i) {
        if (_tickers[i]->_symbol == ticker::Symbol(primary)) {
            if (i == 0)
                return;
            std::swap(_tickers[0], _tickers[i]);
            return;
        }
    }
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Instrument& d) {
        s << *d._dividend
          << *d._identity
          << *d._profitability
          << *d._stability
          << *d._valuation
          << *d._balance
          << *d._earnings
          << *d._shares;
        util::list_to_stream(s, d._tickers);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Instrument& d) {
        s >> *d._dividend
          >> *d._identity
          >> *d._profitability
          >> *d._stability
          >> *d._valuation
          >> *d._balance
          >> *d._earnings
          >> *d._shares;
        util::list_from_stream(s, d._tickers, &d);
        d.fix_tickers_load();
        return s;
    }
}
