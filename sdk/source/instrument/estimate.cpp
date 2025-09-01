#include "instrument/estimate.h"

using namespace sdk;

// --------------------------- Main Estimate ------------------------------------------------------
double    sdk::Estimate::   wallStreetTargetPrice() const { return _wallStreetTargetPrice; }
FieldTOpt sdk::Estimate::setWallStreetTargetPrice(double v)
{ return  sdk::set_if(this, _wallStreetTargetPrice, v, sdk::Est_wallstr_target_price, 1e-6); }

double    sdk::Estimate::   epsEstimateCurrentYear() const { return _epsEstimateCurrentYear; }
FieldTOpt sdk::Estimate::setEpsEstimateCurrentYear(double v)
{ return  sdk::set_if(this, _epsEstimateCurrentYear, v, sdk::Est_eps_curr_year, 1e-6); }

double    sdk::Estimate::   epsEstimateNextYear() const { return _epsEstimateNextYear; }
FieldTOpt sdk::Estimate::setEpsEstimateNextYear(double v)
{ return  sdk::set_if(this, _epsEstimateNextYear, v, sdk::Est_eps_next_year, 1e-6); }

double    sdk::Estimate::   epsEstimateCurrentQuart() const { return _epsEstimateCurrentQuart; }
FieldTOpt sdk::Estimate::setEpsEstimateCurrentQuart(double v)
{ return  sdk::set_if(this, _epsEstimateCurrentQuart, v, sdk::Est_eps_curr_quart, 1e-6); }

double    sdk::Estimate::   epsEstimateNextQuart() const { return _epsEstimateNextQuart; }
FieldTOpt sdk::Estimate::setEpsEstimateNextQuart(double v)
{ return  sdk::set_if(this, _epsEstimateNextQuart, v, sdk::Est_eps_next_quart, 1e-6); }

const QDate& sdk::Estimate::   recentQuart() const { return _recentQuart; }
FieldTOpt    sdk::Estimate::setRecentQuart(const QDate& date)
{ return     sdk::set_if(this, _recentQuart, date, sdk::Fundam_recent_quart); }
// ================================================================================================


// --------------------------- Analyst ------------------------------------------------------------
FieldTOpt sdk::Estimate::setAnalystConsensusRate(double rate)
{ return  sdk::set_if(this, _analyst.consensus_rate, rate, sdk::Analyst_rate); }

FieldTOpt sdk::Estimate::setAnalystTargetPrice(double price)
{ return  sdk::set_if(this, _analyst.target_price, price, sdk::Analyst_targetprice); }

FieldTOpt sdk::Estimate::setAnalystStrongBuy(uint16_t count)
{ return  sdk::set_if(this, _analyst.strongbuy, count, sdk::Analyst_strongbuy); }

FieldTOpt sdk::Estimate::setAnalystBuy(uint16_t count)
{ return  sdk::set_if(this, _analyst.buy, count, sdk::Analyst_buy); }

FieldTOpt sdk::Estimate::setAnalystHold(uint16_t count)
{ return  sdk::set_if(this, _analyst.hold, count, sdk::Analyst_hold); }

FieldTOpt sdk::Estimate::setAnalystSell(uint16_t count)
{ return  sdk::set_if(this, _analyst.sell, count, sdk::Analyst_sell); }

FieldTOpt sdk::Estimate::setAnalystStrongSell(uint16_t count)
{ return  sdk::set_if(this, _analyst.strongsell, count, sdk::Analyst_strongsell); }

const sdk::Estimate::Analyst& sdk::Estimate::analyst() const { return _analyst; }
// ================================================================================================
