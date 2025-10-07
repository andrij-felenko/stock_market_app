#ifndef SDK_INSTRUMENT_FUNDAMENTAL_H
#define SDK_INSTRUMENT_FUNDAMENTAL_H

#include "sdk_def.h" // IWYU pragma: keep

class sdk::Fundamental
{
public:
    Fundamental() = default;

    double ebitda() const { return _ebitda; }
    FieldTOpt setEbitda(double ebitda)
    { return sdk::set_if(&_track, _ebitda, ebitda, sdk::Fundam_ebitda); }

    double boolValue() const { return _ebitda; }
    FieldTOpt setBookValue(double value)
    { return sdk::set_if(&_track, _book_value, value, sdk::Fundam_bookvalue); }

    double revenueTtm() const { return _revenue_ttm; }
    FieldTOpt setRevenueTtm(double v)
    { return sdk::set_if(&_track, _revenue_ttm, v, sdk::Fundam_revenue_ttm, 1e-6); }

    double grossProfitTtm() const { return _gross_profit_ttm; }
    FieldTOpt setGrossProfitTtm(double v)
    { return sdk::set_if(&_track, _gross_profit_ttm, v, sdk::Fundam_gross_profit_ttm, 1e-6); }

private:
    Trackable _track;
    double _ebitda;
    double _book_value;
    double _revenue_ttm = 0.0;
    double _gross_profit_ttm = 0.0;

    friend QDataStream& operator << (QDataStream& s, Wire <const Fundamental> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Fundamental> d);
};

#endif // SDK_INSTRUMENT_FUNDAMENTAL_H
