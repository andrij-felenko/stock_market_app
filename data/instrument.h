#ifndef DATA_INSTRUMENT_H
#define DATA_INSTRUMENT_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

#include "data/meta.h"
#include "instrument/symbol.h"
#include "instrument/ticker.h"
#include "instrument/dividend.h"
#include "instrument/identity.h"
#include "instrument/profitability.h"
#include "instrument/stability.h"
#include "instrument/valuation.h"

#include "utilities/features.h"

namespace data {
    class Market;
    class Instrument;
    struct TickerMeta;

    struct InstrumentMeta {
        //
    };
}

class data::Instrument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Dividend*      dividend      READ dividend      CONSTANT)
    Q_PROPERTY(Identity*      identity      READ identity      CONSTANT)
    Q_PROPERTY(Stability*     stability     READ stability     CONSTANT)
    Q_PROPERTY(Valuation*     valuation     READ valuation     CONSTANT)
    Q_PROPERTY(Profitability* profitability READ profitability CONSTANT)
public:
    Instrument(const ticker::Symbol& tag,  QObject* parent = nullptr);
    Instrument(const meta  ::Ticker& meta, QObject* parent = nullptr);

    Dividend* dividend() const;
    Identity* identity() const;
    Stability* stability() const;
    Valuation* valuation() const;
    Profitability* profitability() const;

    Ticker* primary_ticker(bool absolute = false) const;

    ticker::SymbolList tickers() const;
    ticker::Symbol primary_symbol(bool absolute = false) const;
    bool contains(const ticker::Symbol& symbol) const;

    void save() const;
    void load();

    Ticker* const operator [] (ticker::Symbol symbol) const;

    operator meta::Ticker() const;
    // void set_meta_ticker(const meta::Ticker& meta);

private:
    Ticker* const ensure(ticker::Symbol symbol);

    Dividend* _dividend = nullptr;
    Identity* _identity = nullptr;
    Stability* _stability = nullptr;
    Valuation* _valuation = nullptr;
    Profitability* _profitability = nullptr;

    std::vector <Ticker*> _tickers;

    friend class data::Market;

    friend QDataStream& operator << (QDataStream& s, const Instrument& d);
    friend QDataStream& operator >> (QDataStream& s,       Instrument& d);
};

#endif
