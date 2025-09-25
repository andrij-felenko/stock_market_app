#include "core/security/finance/estimate.h"

using namespace sdk;

// --------------------------- Main Estimate ------------------------------------------------------
double    sdk::Estimate::   wallStreetTargetPrice() const { return _wall_street_target_price; }
FieldTOpt sdk::Estimate::setWallStreetTargetPrice(double v)
{ return  sdk::set_if(this, _wall_street_target_price, v, sdk::Est_wallstr_target_price, 1e-6); }

double    sdk::Estimate::   epsEstimateCurrentYear() const { return _eps_estimate_current_year; }
FieldTOpt sdk::Estimate::setEpsEstimateCurrentYear(double v)
{ return  sdk::set_if(this, _eps_estimate_current_year, v, sdk::Est_eps_curr_year, 1e-6); }

double    sdk::Estimate::   epsEstimateNextYear() const { return _eps_estimate_next_year; }
FieldTOpt sdk::Estimate::setEpsEstimateNextYear(double v)
{ return  sdk::set_if(this, _eps_estimate_next_year, v, sdk::Est_eps_next_year, 1e-6); }

double    sdk::Estimate::   epsEstimateCurrentQuart() const { return _eps_estimate_current_quart; }
FieldTOpt sdk::Estimate::setEpsEstimateCurrentQuart(double v)
{ return  sdk::set_if(this, _eps_estimate_current_quart, v, sdk::Est_eps_curr_quart, 1e-6); }

double    sdk::Estimate::   epsEstimateNextQuart() const { return _eps_estimate_next_quart; }
FieldTOpt sdk::Estimate::setEpsEstimateNextQuart(double v)
{ return  sdk::set_if(this, _eps_estimate_next_quart, v, sdk::Est_eps_next_quart, 1e-6); }

const QDate& sdk::Estimate::   recentQuart() const { return _recent_quart; }
FieldTOpt    sdk::Estimate::setRecentQuart(const QDate& date)
{ return     sdk::set_if(this, _recent_quart, date, sdk::Fundam_recent_quart); }
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

namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Estimate> d){
        s << d->_wall_street_target_price
          << d->_eps_estimate_current_year << d->_eps_estimate_next_year
          << d->_eps_estimate_current_quart << d->_eps_estimate_next_quart
          << d->_recent_quart

          << d->_analyst.consensus_rate
          << d->_analyst.target_price
          << d->_analyst.strongbuy << d->_analyst.buy << d->_analyst.hold
          << d->_analyst.sell << d->_analyst.strongsell;
        if (d.recursive) s << static_cast <const Trackable&> (d.ref);
        return s;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Estimate> d){
        s >> d->_wall_street_target_price
          >> d->_eps_estimate_current_year >> d->_eps_estimate_next_year
          >> d->_eps_estimate_current_quart >> d->_eps_estimate_next_quart
          >> d->_recent_quart

          >> d->_analyst.consensus_rate
          >> d->_analyst.target_price
          >> d->_analyst.strongbuy >> d->_analyst.buy >> d->_analyst.hold
          >> d->_analyst.sell >> d->_analyst.strongsell;
        if (d.recursive) s >> static_cast <Trackable&> (d.ref);
        return s;
    }
}
