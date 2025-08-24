#include "stability.h"
#include "../instrument.h"

using namespace data;

data::Stability::Stability(Instrument* parent) : QObject(parent)
{
    //
}
Instrument* Stability::instrument() const { return static_cast <Instrument*> (parent()); }

float Stability::debt_equity() const { return _debt_equity; }
void  Stability::setDebtEquity(float new_debt_to_equity)
{
    if (qFuzzyCompare(_debt_equity, new_debt_to_equity))
        return;
    _debt_equity = new_debt_to_equity;
    emit debt_equityChanged(_debt_equity);
}

float Stability::   beta() const { return _beta; }
void  Stability::setBeta(float new_beta)
{
    if (qFuzzyCompare(_beta, new_beta))
        return;
    _beta = new_beta;
    emit betaChanged(_beta);
}

double Stability::net_debt() const { return _net_debt; }
void   Stability::setNetDebt(double newNet_debt)
{
    if (qFuzzyCompare(_net_debt, newNet_debt))
        return;
    _net_debt = newNet_debt;
    emit netDebtChanged(_net_debt);
}

double Stability::short_debt() const { return _short_debt; }
void   Stability::setShortDebt(double newShort_debt)
{
    if (qFuzzyCompare(_short_debt, newShort_debt))
        return;
    _short_debt = newShort_debt;
    emit shortDebtChanged(_short_debt);
}

double Stability::long_debt() const { return _long_debt; }
void   Stability::setLongDebt(double newLong_debt)
{
    if (qFuzzyCompare(_long_debt, newLong_debt))
        return;
    _long_debt = newLong_debt;
    emit longDebtChanged(_long_debt);
}

double Stability::net_working_cap() const { return _net_working_cap; }
void   Stability::setNetWorkingCap(double newNet_working_cap)
{
    if (qFuzzyCompare(_net_working_cap, newNet_working_cap))
        return;
    _net_working_cap = newNet_working_cap;
    emit netWorkingCapChanged(_net_working_cap);
}


namespace data {
    QDataStream& operator<<(QDataStream& s, const Stability& d) {
        return s << d._debt_equity
                 << d._beta
                 << d._net_debt
                 << d._short_debt
                 << d._long_debt
                 << d._net_working_cap;
    }

    QDataStream& operator>>(QDataStream& s, Stability& d) {
        return s >> d._debt_equity
                 >> d._beta
                 >> d._net_debt
                 >> d._short_debt
                 >> d._long_debt
                 >> d._net_working_cap;
    }

}
