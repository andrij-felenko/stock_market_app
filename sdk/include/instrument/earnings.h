#ifndef SDK_INSTRUMENT_EARNINGS_H
#define SDK_INSTRUMENT_EARNINGS_H

#include "sdk.h" // IWYU pragma: keep
#include "symbol.h"

class sdk::Earnings
{
public:
    Earnings() = default;

    float epsActual()   const { return _eps_actual; }
    float epsEstimate() const { return _eps_estimate; }

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
};

#endif // SDK_INSTRUMENT_EARNINGS_H
