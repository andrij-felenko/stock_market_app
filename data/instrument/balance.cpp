#include "balance.h"
#include "../instrument.h"

using namespace data;

data::Balance::Balance(Instrument* parent) : QObject(parent)
{
    //
}
Instrument* Balance::instrument() const { return static_cast <Instrument*> (parent()); }

double Balance::total_debt() const { return _total_debt; }
void   Balance::setTotalDebt(double newTotal_debt)
{
    if (qFuzzyCompare(_total_debt, newTotal_debt))
        return;
    _total_debt = newTotal_debt;
    emit totalDebtChanged(_total_debt);
}

double Balance::cash_sti() const { return _cash_sti; }
void   Balance::setCashSti(double newCash_sti)
{
    if (qFuzzyCompare(_cash_sti, newCash_sti))
        return;
    _cash_sti = newCash_sti;
    emit cashStiChanged(_cash_sti);
}

double Balance::   equity() const { return _equity; }
void   Balance::setEquity(double newEquity)
{
    if (qFuzzyCompare(_equity, newEquity))
        return;
    _equity = newEquity;
    emit equityChanged(_equity);
}

namespace data {
    QDataStream& operator<<(QDataStream& s, const Balance& d) {
        return s << d._total_debt << d._cash_sti << d._equity;
    }

    QDataStream& operator>>(QDataStream& s, Balance& d) {
        return s >> d._total_debt >> d._cash_sti >> d._equity;
    }
}
