#ifndef SDK_INSTRUMENT_TREND_H
#define SDK_INSTRUMENT_TREND_H

#include "sdk_def.h" // IWYU pragma: keep

class sdk::Trend : Trackable
{
public:
    Trend() = default;

    /// @brief Загальний приріст (можна трактувати як EPS YoY)
    double growth() const;

    /// @brief Приріст EPS: (earnings_avg / earnings_year_ago_eps) - 1
    double earnings_growth() const;

    /// @brief Приріст виручки: (revenue_avg / revenue_year_ago_eps) - 1
    double revenue_growth() const;

    // --- earnings ---
    double       earningsAvg() const;
    FieldTOpt setEarningsAvg(double v);

    double       earningsLow() const;
    FieldTOpt setEarningsLow(double v);

    double       earningsHigh() const;
    FieldTOpt setEarningsHigh(double v);

    double       earningsYearAgoEps() const;
    FieldTOpt setEarningsYearAgoEps(double v);

    uint16_t     earningsAnalystsCount() const;
    FieldTOpt setEarningsAnalystsCount(uint16_t v);

    // --- revenue ---
    double       revenueAvg() const;
    FieldTOpt setRevenueAvg(double v);

    double       revenueLow() const;
    FieldTOpt setRevenueLow(double v);

    double       revenueHigh() const;
    FieldTOpt setRevenueHigh(double v);

    double       revenueYearAgoEps() const;
    FieldTOpt setRevenueYearAgoEps(double v);

    uint16_t     revenueAnalystsCount()const;
    FieldTOpt setRevenueAnalystsCount(uint16_t v);

    // --- eps trend levels ---
    double       epsTrendCurrent() const;
    FieldTOpt setEpsTrendCurrent(double v);

    double       epsTrend7dAgo() const;
    FieldTOpt setEpsTrend7dAgo(double v);

    double       epsTrend30dAgo() const;
    FieldTOpt setEpsTrend30dAgo(double v);

    double       epsTrend60dAgo() const;
    FieldTOpt setEpsTrend60dAgo(double v);

    double       epsTrend90dAgo() const;
    FieldTOpt setEpsTrend90dAgo(double v);

    // --- eps revisions ---
    uint16_t     epsRevisionUpLast7d() const;
    FieldTOpt setEpsRevisionUpLast7d(uint16_t v);

    uint16_t     epsRevisionUpLast30d() const;
    FieldTOpt setEpsRevisionUpLast30d(uint16_t v);

    uint16_t     epsRevisionDownLast7d() const;
    FieldTOpt setEpsRevisionDownLast7d(uint16_t v);

    uint16_t     epsRevisionDownLast30d() const;
    FieldTOpt setEpsRevisionDownLast30d(uint16_t v);

private:
    double to_eps(int32_t v) const;
    double to_rev(int32_t v) const;

    int32_t from_eps(double v) const;
    int32_t from_rev(double v) const;

    Currency _currency = Currency::None;

    int32_t  _earnings_avg  = 0;
    int32_t  _earnings_low  = 0;
    int32_t  _earnings_high = 0;
    int32_t  _earnings_year_ago_eps = 0;
    uint16_t _earnings_analysts_count = 0;

    int32_t  _revenue_avg  = 0;
    int32_t  _revenue_low  = 0;
    int32_t  _revenue_high = 0;
    int32_t  _revenue_year_ago_eps = 0;
    uint16_t _revenue_analysts_count = 0;

    int32_t _eps_trend_current = 0;
    int32_t _eps_trend_7d_ago  = 0;
    int32_t _eps_trend_30d_ago = 0;
    int32_t _eps_trend_60d_ago = 0;
    int32_t _eps_trend_90d_ago = 0;

    uint16_t _eps_revision_up_last_7d  = 0;
    uint16_t _eps_revision_up_last_30d = 0;
    uint16_t _eps_revision_down_last_7d  = 0;
    uint16_t _eps_revision_down_last_30d = 0;

    friend class QuartelData;
    friend QDataStream& operator << (QDataStream& s, const Trend& d);
    friend QDataStream& operator >> (QDataStream& s,       Trend& d);
};

#endif // SDK_INSTRUMENT_TREND_H
