#ifndef SDK_INSTRUMENT_EARNINGS_H
#define SDK_INSTRUMENT_EARNINGS_H

#include "sdk_def.h" // IWYU pragma: keep
#include "core/symbol.h"

class sdk::Earnings : public Trackable
{
public:
    Earnings() = default;

    QDate reportDate() const { return _report_date; }
    FieldTOpt setReportDate(const QDate& date)
    { return sdk::set_if(this, _report_date, date, sdk::Earnings_q); }

    bool isBeforeMarket() const { return _before_market; }
    FieldTOpt setBeforeMarket(bool is_before)
    { return sdk::set_if(this, _before_market, is_before, sdk::Earnings_q); }

    float epsActual()   const { return _eps_actual; }
    FieldTOpt setEpsActual(float value)
    { return sdk::set_if(this, _eps_actual, value, sdk::Earnings_q); }

    float epsEstimate() const { return _eps_estimate; }
    FieldTOpt setEpsEstimate(float value)
    { return sdk::set_if(this, _eps_estimate, value, sdk::Earnings_q); }

    float epsDifference() const { return _eps_actual - _eps_estimate; }
    float surprisePercent() const {
        const float est = _eps_estimate;
        if (est == 0.0f) return 0.0f; // уникаємо ділення на нуль
        const float diff = _eps_actual - est;
        const float denom = (est < 0.0f) ? -est : est; // |estimate|
        return 100.0f * diff / denom;
    }

private:
    QDate _report_date;
    bool _before_market;

    float _eps_actual;
    float _eps_estimate;

    friend QDataStream& operator << (QDataStream& s, Wire <const Earnings> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Earnings> d);
};

#endif // SDK_INSTRUMENT_EARNINGS_H
