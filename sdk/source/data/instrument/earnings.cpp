#include "data/instrument/earnings.h"
#include "data/instrument.h"

using namespace data;

data::Earnings::Earnings(Instrument* parent) : QObject(parent)
{
    //
}
Instrument* Earnings::instrument() const { return static_cast <Instrument*> (parent()); }

double Earnings::revenue_ttm() const { return _revenue_ttm; }
void   Earnings::setRevenueTtm(double newRevenue_ttm)
{
    if (qFuzzyCompare(_revenue_ttm, newRevenue_ttm))
        return;
    _revenue_ttm = newRevenue_ttm;
    emit revenueTtmChanged(_revenue_ttm);
}

double Earnings::revenue_per_share_ttm() const { return _revenue_per_share_ttm; }
void   Earnings::setRevenuePerShareTtm(double newRevenue_per_share_ttm)
{
    if (qFuzzyCompare(_revenue_per_share_ttm, newRevenue_per_share_ttm))
        return;
    _revenue_per_share_ttm = newRevenue_per_share_ttm;
    emit revenuePSTtmChanged(_revenue_per_share_ttm);
}

double Earnings::ebitda() const { return _ebitda; }
void   Earnings::setEbitda(double newEbitda)
{
    if (qFuzzyCompare(_ebitda, newEbitda))
        return;
    _ebitda = newEbitda;
    emit ebitdaChanged(_ebitda);
}

float Earnings::diluted_eps_ttm() const { return _diluted_eps_ttm; }
void  Earnings::setDilutedEpsTtm(float newDiluted_eps_ttm)
{
    if (qFuzzyCompare(_diluted_eps_ttm, newDiluted_eps_ttm))
        return;
    _diluted_eps_ttm = newDiluted_eps_ttm;
    emit dilutedEpsTtmChanged(_diluted_eps_ttm);
}

float Earnings::rev_growth_yoy() const { return _rev_growth_yoy; }
void  Earnings::setRevGrowthYoy(float newRev_growth_yoy)
{
    if (qFuzzyCompare(_rev_growth_yoy, newRev_growth_yoy))
        return;
    _rev_growth_yoy = newRev_growth_yoy;
    emit revGrowthYoyChanged(_rev_growth_yoy);
}

float Earnings::earnings_growth_yoy() const { return _earnings_growth_yoy; }
void  Earnings::setEarningsGrowthYoy(float newEarnings_growth_yoy)
{
    if (qFuzzyCompare(_earnings_growth_yoy, newEarnings_growth_yoy))
        return;
    _earnings_growth_yoy = newEarnings_growth_yoy;
    emit earnGrowthYoyChanged(_earnings_growth_yoy);
}

QDate Earnings::most_recent_quarter() const { return _most_recent_quarter; }
void  Earnings::setMostRecentQuarter(const QDate& newMost_recent_quarter)
{
    if (_most_recent_quarter == newMost_recent_quarter)
        return;
    _most_recent_quarter = newMost_recent_quarter;
    emit mostRecentQChanged(_most_recent_quarter);
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Earnings& d) {
        return s << d._revenue_ttm
                 << d._revenue_per_share_ttm
                 << d._ebitda
                 << d._diluted_eps_ttm
                 << d._rev_growth_yoy
                 << d._earnings_growth_yoy
                 << d._most_recent_quarter;
    }

    QDataStream& operator >> (QDataStream& s, Earnings& d) {
        return s >> d._revenue_ttm
                 >> d._revenue_per_share_ttm
                 >> d._ebitda
                 >> d._diluted_eps_ttm
                 >> d._rev_growth_yoy
                 >> d._earnings_growth_yoy
                 >> d._most_recent_quarter;
    }
}
