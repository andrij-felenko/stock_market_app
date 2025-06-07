#include "stability.h"

using namespace data::ticker;

data::ticker::Stability::Stability(QObject* parent) : QObject(parent)
{
    //
}

float Stability::    debt_equity() const { return _debt_equity; }
void  Stability::set_debt_equity(float new_debt_to_equity)
{
    if (qFuzzyCompare(_debt_equity, new_debt_to_equity))
        return;
    _debt_equity = new_debt_to_equity;
    emit debt_equityChanged(_debt_equity);
}

float Stability::    revenue() const { return _revenue; }
void  Stability::set_revenue(float new_revenue)
{
    if (qFuzzyCompare(_revenue, new_revenue))
        return;
    _revenue = new_revenue;
    emit revenueChanged(_revenue);
}

float Stability::    beta() const { return _beta; }
void  Stability::set_beta(float new_beta)
{
    if (qFuzzyCompare(_beta, new_beta))
        return;
    _beta = new_beta;
    emit betaChanged(_beta);
}

namespace data::ticker {
    QDataStream& operator<<(QDataStream& s, const Stability& d) {
        return s << d._beta << d._debt_equity << d._revenue;
    }

    QDataStream& operator>>(QDataStream& s, Stability& d) {
        return s >> d._beta >> d._debt_equity >> d._revenue;
    }
}
