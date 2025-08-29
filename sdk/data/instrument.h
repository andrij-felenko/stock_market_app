#ifndef DATA_INSTRUMENT_H
#define DATA_INSTRUMENT_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "instrument/meta.h"
#include "instrument/symbol.h"
#include "instrument/ticker.h"
#include "instrument/dividend.h"
#include "instrument/identity.h"
#include "instrument/profitability.h"
#include "instrument/stability.h"
#include "instrument/valuation.h"
#include "instrument/balance.h"
#include "instrument/earnings.h"
#include "instrument/shares.h"

#include "utilities/features.h"

namespace data {
    class Market;
    class Instrument;
    struct TickerMeta;

    struct InstrumentMeta {
        //
    };
}

namespace api {
    class Eod;
}

class data::Instrument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Balance*       balance       READ balance       CONSTANT)
    Q_PROPERTY(Dividend*      dividend      READ dividend      CONSTANT)
    Q_PROPERTY(Earnings*      earnings      READ earnings      CONSTANT)
    Q_PROPERTY(Identity*      identity      READ identity      CONSTANT)
    Q_PROPERTY(Meta*          meta          READ meta          CONSTANT)
    Q_PROPERTY(Profitability* profitability READ profitability CONSTANT)
    Q_PROPERTY(Shares*        shares        READ shares        CONSTANT)
    Q_PROPERTY(Stability*     stability     READ stability     CONSTANT)
    Q_PROPERTY(Valuation*     valuation     READ valuation     CONSTANT)
public:
    Dividend* dividend() const;
    Identity* identity() const;
    Stability* stability() const;
    Valuation* valuation() const;
    Profitability* profitability() const;
    Earnings* earnings() const;
    Balance* balance() const;
    Shares* shares() const;
    Meta* meta() const;

    Ticker* primary_ticker(bool absolute = false) const;

    ticker::SymbolList tickers() const;
    std::vector <Ticker*> tickers_ptr();
    ticker::Symbol primary_symbol(bool absolute = false) const;
    bool contains(const ticker::Symbol& symbol) const;
    bool was_loaded() const;

    bool have_fundamental() const;

    void save() const;
    void load();

    Ticker* const operator [] (ticker::Symbol symbol) const;

private:
    Instrument(const ticker::Symbol& tag,  QObject* parent = nullptr);

    Ticker* const ensure(ticker::Symbol symbol);
    void _sort_tickers();

    Balance* _balance = nullptr;
    Dividend* _dividend = nullptr;
    Earnings* _earnings = nullptr;
    Identity* _identity = nullptr;
    Profitability* _profitability = nullptr;
    Shares* _shares = nullptr;
    Stability* _stability = nullptr;
    Valuation* _valuation = nullptr;
    Meta* _meta = nullptr;

    std::vector <Ticker*> _tickers;
    bool _save_locker;
    bool _was_loaded;
    void fix_tickers_load();
    void updatePrimarySymbol(const QString& primary);

    friend class data::Market;
    friend class api::Eod;

    friend QDataStream& operator << (QDataStream& s, const Instrument& d);
    friend QDataStream& operator >> (QDataStream& s,       Instrument& d);
};

#endif
