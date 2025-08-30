#include "data/instrument/profitability.h"
#include "data/instrument.h"

using namespace data;

data::Profitability::Profitability(Instrument* parent) : QObject(parent)
{
    //
}
Instrument* Profitability::instrument() const { return static_cast <Instrument*> (parent()); }

float Profitability::   roe() const { return _roe; }
void  Profitability::setRoe(float newRoe)
{
    if (qFuzzyCompare(_roe, newRoe))
        return;
    _roe = newRoe;
    emit roeChanged(_roe);
}

float Profitability::   roa() const { return _roa; }
void  Profitability::setRoa(float newRoa)
{
    if (qFuzzyCompare(_roa, newRoa))
        return;
    _roa = newRoa;
    emit roaChanged(_roa);
}

float Profitability::    margin_gros() const { return _margin_gros; }
void  Profitability::setMarginGros(float new_gross_margin)
{
    if (qFuzzyCompare(_margin_gros, new_gross_margin))
        return;
    _margin_gros = new_gross_margin;
    emit marginGrosChanged(_margin_gros);
}

float Profitability::margin_oper() const { return _margin_oper; }
void  Profitability::setMarginOper(float new_operating_margin)
{
    if (qFuzzyCompare(_margin_oper, new_operating_margin))
        return;
    _margin_oper = new_operating_margin;
    emit marginOperChanged(_margin_oper);
}

float Profitability::net_income() const { return _netincome; }
void  Profitability::setNetIncome(float newNet_income)
{
    if (qFuzzyCompare(_netincome, newNet_income))
        return;
    _netincome = newNet_income;
    emit netincomeChanged(_netincome);
}

double Profitability::revenue_ttm() const { return _revenue_ttm; }
void   Profitability::setRevenueTtm(double newRevenue_ttm)
{
    if (qFuzzyCompare(_revenue_ttm, newRevenue_ttm))
        return;
    _revenue_ttm = newRevenue_ttm;
    emit revenueTtmChanged(_revenue_ttm);
}

double Profitability::ebitda() const { return _ebitda; }
void   Profitability::setEbitda(double newEbitda)
{
    if (qFuzzyCompare(_ebitda, newEbitda))
        return;
    _ebitda = newEbitda;
    emit ebitdaChanged(_ebitda);
}

double Profitability::gross_profit_ttm() const { return _gross_profit_ttm; }
void   Profitability::setGrossProfitTtm(double newGross_profit_ttm)
{
    if (qFuzzyCompare(_gross_profit_ttm, newGross_profit_ttm))
        return;
    _gross_profit_ttm = newGross_profit_ttm;
    emit grosProfitTtmChanged(_gross_profit_ttm);
}

float Profitability::revenue_per_share_ttm() const { return _revenue_per_share_ttm; }
void  Profitability::setRevenuePerShareTtm(float newRevenue_per_share_ttm)
{
    if (qFuzzyCompare(_revenue_per_share_ttm, newRevenue_per_share_ttm))
        return;
    _revenue_per_share_ttm = newRevenue_per_share_ttm;
    emit revenuePSTtmChanged(_revenue_per_share_ttm);
}

float Profitability::profit_margin() const { return _profit_margin; }
void  Profitability::setProfitMargin(float newProfit_margin)
{
    if (qFuzzyCompare(_profit_margin, newProfit_margin))
        return;
    _profit_margin = newProfit_margin;
    emit profitMarginChanged(_profit_margin);
}

float Profitability::operating_margin_ttm() const { return _operating_margin_ttm; }
void  Profitability::setOperatingMarginTtm(float newOperating_margin_ttm)
{
    if (qFuzzyCompare(_operating_margin_ttm, newOperating_margin_ttm))
        return;
    _operating_margin_ttm = newOperating_margin_ttm;
    emit operMargTtmChanged(_operating_margin_ttm);
}

float Profitability::eps_ttm() const { return _eps_ttm; }
void  Profitability::setEpsTtm(float newEps_ttm)
{
    if (qFuzzyCompare(_eps_ttm, newEps_ttm))
        return;
    _eps_ttm = newEps_ttm;
    emit epsTtmChanged(_eps_ttm);
}

float Profitability::eps_estimate_year() const { return _eps_estimate_year; }
void  Profitability::setEpsEstimateYear(float newEps_estimate_year)
{
    if (qFuzzyCompare(_eps_estimate_year, newEps_estimate_year))
        return;
    _eps_estimate_year = newEps_estimate_year;
    emit epsEstYearChanged(_eps_estimate_year);
}

float Profitability::eps_estimate_nextyear() const { return _eps_estimate_next_year; }
void  Profitability::setEpsEstimateNextYear(float newEps_estimate_next_year)
{
    if (qFuzzyCompare(_eps_estimate_next_year, newEps_estimate_next_year))
        return;
    _eps_estimate_next_year = newEps_estimate_next_year;
    emit epsEstNYChanged(_eps_estimate_next_year);
}

float Profitability::peg_ratio() const { return _peg_ratio; }
void  Profitability::setPegRatio(float newPeg_ratio)
{
    if (qFuzzyCompare(_peg_ratio, newPeg_ratio))
        return;
    _peg_ratio = newPeg_ratio;
    emit pegRatioChanged(_peg_ratio);
}

float Profitability::target_price() const { return _target_price; }
void  Profitability::setTargetPrice(float newTarget_price)
{
    if (qFuzzyCompare(_target_price, newTarget_price))
        return;
    _target_price = newTarget_price;
    emit targetPriceChanged(_target_price);
}

QDate Profitability::most_recent_quarter() const { return _most_recent_quarter; }
void  Profitability::setMostRecentQuarter(const QDate& newMost_recent_quarter)
{
    if (_most_recent_quarter == newMost_recent_quarter)
        return;
    _most_recent_quarter = newMost_recent_quarter;
    emit mostRecentQChanged(_most_recent_quarter);
}

float Profitability::q_rev_growth_yoy() const { return _q_rev_growth_yoy; }
void  Profitability::setQrevGrowthYoy(float newQ_rev_growth_yoy)
{
    if (qFuzzyCompare(_q_rev_growth_yoy, newQ_rev_growth_yoy))
        return;
    _q_rev_growth_yoy = newQ_rev_growth_yoy;
    emit qRevGrowYoyChanged(_q_rev_growth_yoy);
}

float Profitability::q_eps_growth_yoy() const { return _q_eps_growth_yoy; }
void  Profitability::setQepsGrowthYoy(float newQ_eps_growth_yoy)
{
    if (qFuzzyCompare(_q_eps_growth_yoy, newQ_eps_growth_yoy))
        return;
    _q_eps_growth_yoy = newQ_eps_growth_yoy;
    emit qEpsGrowYoyChanged(_q_eps_growth_yoy);
}

namespace data {
    QDataStream& operator << (QDataStream& s, const Profitability& p) {
        return s << p._roe
                 << p._roa
                 << p._margin_gros
                 << p._margin_oper
                 << p._netincome
                 << p._revenue_ttm
                 << p._gross_profit_ttm
                 << p._revenue_per_share_ttm
                 << p._profit_margin
                 << p._operating_margin_ttm
                 << p._eps_ttm
                 << p._eps_estimate_year
                 << p._eps_estimate_next_year
                 << p._peg_ratio
                 << p._target_price
                 << p._most_recent_quarter
                 << p._q_rev_growth_yoy
                 << p._q_eps_growth_yoy
                 << p._ebitda;
    }

    QDataStream& operator >> (QDataStream& s, Profitability& p) {
        return s >> p._roe
                 >> p._roa
                 >> p._margin_gros
                 >> p._margin_oper
                 >> p._netincome
                 >> p._revenue_ttm
                 >> p._gross_profit_ttm
                 >> p._revenue_per_share_ttm
                 >> p._profit_margin
                 >> p._operating_margin_ttm
                 >> p._eps_ttm
                 >> p._eps_estimate_year
                 >> p._eps_estimate_next_year
                 >> p._peg_ratio
                 >> p._target_price
                 >> p._most_recent_quarter
                 >> p._q_rev_growth_yoy
                 >> p._q_eps_growth_yoy
                 >> p._ebitda;
    }
}
