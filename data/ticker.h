#ifndef DATA_TICKER_H
#define DATA_TICKER_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "ticker/quotes.h"
#include "ticker/dividend.h"
#include "ticker/identity.h"
#include "ticker/profitability.h"
#include "ticker/stability.h"
#include "ticker/valuation.h"

namespace data { class Ticker; }

class data::Ticker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ticker::Quotes*        quotes        READ quotes        CONSTANT)
    Q_PROPERTY(ticker::Dividend*      dividend      READ dividend      CONSTANT)
    Q_PROPERTY(ticker::Identity*      identity      READ identity      CONSTANT)
    Q_PROPERTY(ticker::Stability*     stability     READ stability     CONSTANT)
    Q_PROPERTY(ticker::Valuation*     valuation     READ valuation     CONSTANT)
    Q_PROPERTY(ticker::Profitability* profitability READ profitability CONSTANT)
public:
    Ticker(QObject* parent = nullptr);

    ticker::Quotes*   quotes() const;
    ticker::Dividend* dividend() const;
    ticker::Identity* identity() const;
    ticker::Stability* stability() const;
    ticker::Valuation* valuation() const;
    ticker::Profitability* profitability() const;

    void save() const;
    void load();

signals:
    void update_data();

private:
    ticker::Quotes*   _quotes = nullptr;
    ticker::Dividend* _dividend = nullptr;
    ticker::Identity* _identity = nullptr;
    ticker::Stability* _stability = nullptr;
    ticker::Valuation* _valuation = nullptr;
    ticker::Profitability* _profitability = nullptr;

    friend QDataStream& operator << (QDataStream& s, const Ticker& d);
    friend QDataStream& operator >> (QDataStream& s,       Ticker& d);
};

#endif // DATA_TICKER_H
