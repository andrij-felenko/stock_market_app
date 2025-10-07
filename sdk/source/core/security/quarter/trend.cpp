#include "core/security/quarter/trend.h"

double sdk::Trend::growth() const {
    if (_earnings_year_ago_eps != 0.0)
        return (_earnings_avg / _earnings_year_ago_eps) - 1.0;
    return 0.0;
}

/// @brief Приріст EPS: (earnings_avg / earnings_year_ago_eps) - 1
double sdk::Trend::earnings_growth() const {
    if (_earnings_year_ago_eps != 0.0)
        return (_earnings_avg / _earnings_year_ago_eps) - 1.0;
    return 0.0;
}

double sdk::Trend::revenue_growth() const {
    if (_revenue_year_ago_eps != 0.0)
        return (_revenue_avg / _revenue_year_ago_eps) - 1.0;
    return 0.0;
}

double sdk::Trend::to_eps(int32_t v) const { return double(v) / 1'000; }
double sdk::Trend::to_rev(int32_t v) const { return double(v) / sdk::currency::scale(_currency); }

int32_t sdk::Trend::from_eps(double v) const { return v * 1'000; }
int32_t sdk::Trend::from_rev(double v) const { return v * sdk::currency::scale(_currency); }

// --- earnings ---
double sdk::Trend::earningsAvg() const { return to_eps(_earnings_avg); }
FieldTOpt sdk::Trend::setEarningsAvg(double v)
{ return set_if(&_track, _earnings_avg, from_eps(v), sdk::Est_trend); }

double sdk::Trend::earningsLow() const { return to_eps(_earnings_low); }
FieldTOpt sdk::Trend::setEarningsLow(double v)
{ return set_if(&_track, _earnings_low, from_eps(v), sdk::Est_trend); }

double sdk::Trend::earningsHigh() const { return to_eps(_earnings_high); }
FieldTOpt sdk::Trend::setEarningsHigh(double v)
{ return set_if(&_track, _earnings_high, from_eps(v), sdk::Est_trend); }

double sdk::Trend::earningsYearAgoEps() const { return to_eps(_earnings_year_ago_eps); }
FieldTOpt sdk::Trend::setEarningsYearAgoEps(double v)
{ return set_if(&_track, _earnings_year_ago_eps, from_eps(v), sdk::Est_trend); }

uint16_t sdk::Trend::earningsAnalystsCount() const { return to_eps(_earnings_analysts_count); }
FieldTOpt sdk::Trend::setEarningsAnalystsCount(uint16_t v)
{ return set_if(&_track, _earnings_analysts_count, v, sdk::Est_trend); }

// --- revenue ---
double sdk::Trend::revenueAvg() const { return to_rev(_revenue_avg); }
FieldTOpt sdk::Trend::setRevenueAvg(double v)
{ return set_if(&_track, _revenue_avg, from_rev(v), sdk::Est_trend); }

double sdk::Trend::revenueLow() const { return to_rev(_revenue_low); }
FieldTOpt sdk::Trend::setRevenueLow(double v)
{ return set_if(&_track, _revenue_low, from_rev(v), sdk::Est_trend); }

double sdk::Trend::revenueHigh() const { return to_rev(_revenue_high); }
FieldTOpt sdk::Trend::setRevenueHigh(double v)
{ return set_if(&_track, _revenue_high, from_rev(v), sdk::Est_trend); }

double sdk::Trend::revenueYearAgoEps() const { return to_rev(_revenue_year_ago_eps); }
FieldTOpt sdk::Trend::setRevenueYearAgoEps(double v)
{ return set_if(&_track, _revenue_year_ago_eps, from_rev(v), sdk::Est_trend); }

uint16_t sdk::Trend::revenueAnalystsCount() const { return _revenue_analysts_count; }
FieldTOpt sdk::Trend::setRevenueAnalystsCount(uint16_t v)
{ return set_if(&_track, _revenue_analysts_count, v, sdk::Est_trend); }

// --- eps trend levels ---
double sdk::Trend::epsTrendCurrent() const { return to_eps(_eps_trend_current); }
FieldTOpt sdk::Trend::setEpsTrendCurrent(double v)
{ return set_if(&_track, _eps_trend_current, from_eps(v), sdk::Est_trend); }

double sdk::Trend::epsTrend7dAgo() const { return to_eps(_eps_trend_7d_ago); }
FieldTOpt sdk::Trend::setEpsTrend7dAgo(double v)
{ return set_if(&_track, _eps_trend_7d_ago, from_eps(v), sdk::Est_trend); }

double sdk::Trend::epsTrend30dAgo() const { return to_eps(_eps_trend_30d_ago); }
FieldTOpt sdk::Trend::setEpsTrend30dAgo(double v)
{ return set_if(&_track, _eps_trend_30d_ago, from_eps(v), sdk::Est_trend); }

double sdk::Trend::epsTrend60dAgo() const { return to_eps(_eps_trend_60d_ago); }
FieldTOpt sdk::Trend::setEpsTrend60dAgo(double v)
{ return set_if(&_track, _eps_trend_60d_ago, from_eps(v), sdk::Est_trend); }

double sdk::Trend::epsTrend90dAgo() const { return to_eps(_eps_trend_90d_ago); }
FieldTOpt sdk::Trend::setEpsTrend90dAgo(double v)
{ return set_if(&_track, _eps_trend_90d_ago, from_eps(v), sdk::Est_trend); }

// --- eps revisions ---
uint16_t sdk::Trend::epsRevisionUpLast7d() const { return _eps_revision_up_last_7d; }
FieldTOpt sdk::Trend::setEpsRevisionUpLast7d(uint16_t v)
{ return set_if(&_track, _eps_revision_up_last_7d, v, sdk::Est_trend); }

uint16_t sdk::Trend::epsRevisionUpLast30d() const { return _eps_revision_up_last_30d; }
FieldTOpt sdk::Trend::setEpsRevisionUpLast30d(uint16_t v)
{ return set_if(&_track, _eps_revision_up_last_30d, v, sdk::Est_trend); }

uint16_t sdk::Trend::epsRevisionDownLast7d() const { return _eps_revision_down_last_7d; }
FieldTOpt sdk::Trend::setEpsRevisionDownLast7d(uint16_t v)
{ return set_if(&_track, _eps_revision_down_last_7d, v, sdk::Est_trend); }

uint16_t sdk::Trend::epsRevisionDownLast30d() const { return _eps_revision_down_last_30d; }
FieldTOpt sdk::Trend::setEpsRevisionDownLast30d(uint16_t v)
{ return set_if(&_track, _eps_revision_down_last_30d, v, sdk::Est_trend); }


namespace sdk {
    QDataStream& operator << (QDataStream& s, Wire <const Trend> d){
        if (d.data()) s << d->_earnings_avg
                        << d->_earnings_low
                        << d->_earnings_high
                        << d->_earnings_year_ago_eps
                        << d->_earnings_analysts_count

                        << d->_revenue_avg
                        << d->_revenue_low
                        << d->_revenue_high
                        << d->_revenue_year_ago_eps
                        << d->_revenue_analysts_count

                        << d->_eps_trend_current
                        << d->_eps_trend_7d_ago
                        << d->_eps_trend_30d_ago
                        << d->_eps_trend_60d_ago
                        << d->_eps_trend_90d_ago

                        << d->_eps_revision_up_last_7d
                        << d->_eps_revision_up_last_30d
                        << d->_eps_revision_down_last_7d
                        << d->_eps_revision_down_last_30d;

        return s << d->_track;
    }

    QDataStream& operator >> (QDataStream& s, Wire <Trend> d){
        if (d.data()) s >> d->_earnings_avg
                        >> d->_earnings_low
                        >> d->_earnings_high
                        >> d->_earnings_year_ago_eps
                        >> d->_earnings_analysts_count

                        >> d->_revenue_avg
                        >> d->_revenue_low
                        >> d->_revenue_high
                        >> d->_revenue_year_ago_eps
                        >> d->_revenue_analysts_count

                        >> d->_eps_trend_current
                        >> d->_eps_trend_7d_ago
                        >> d->_eps_trend_30d_ago
                        >> d->_eps_trend_60d_ago
                        >> d->_eps_trend_90d_ago

                        >> d->_eps_revision_up_last_7d
                        >> d->_eps_revision_up_last_30d
                        >> d->_eps_revision_down_last_7d
                        >> d->_eps_revision_down_last_30d;

        return s >> d->_track;
    }
}
