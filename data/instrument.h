#ifndef DATA_INSTRUMENT_H
#define DATA_INSTRUMENT_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

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
    Instrument(QObject* parent = nullptr);

    Dividend* dividend() const;
    Identity* identity() const;
    Stability* stability() const;
    Valuation* valuation() const;
    Profitability* profitability() const;

    Ticker* primary_ticker(bool absolute = false) const;

    QStringList tickers() const;
    bool contains(const QString& symbol) const;

    void save() const;
    void load();

    Ticker* const operator[](const QString& symbol) const;
    Ticker* const get(const QString& symbol, bool createif = false, bool prime = false);

    operator data::TickerMeta() const;

private:
    Dividend* _dividend = nullptr;
    Identity* _identity = nullptr;
    Stability* _stability = nullptr;
    Valuation* _valuation = nullptr;
    Profitability* _profitability = nullptr;

    std::vector <Ticker*> _tickers;
    QString _primary_ticker;

    void _update_primary_ticket();
    void _add_ticker(Ticker* ticker);

    friend class data::Market;

    friend QDataStream& operator << (QDataStream& s, const Instrument& d);
    friend QDataStream& operator >> (QDataStream& s,       Instrument& d);
};

#endif
