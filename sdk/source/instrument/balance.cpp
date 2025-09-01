#ifndef SDK_INSTRUMENT_QUOTES_H
#define SDK_INSTRUMENT_QUOTES_H

#include "sdk.h" // IWYU pragma: keep

class sdk::Quotes : Trackable
{
public:
    Quotes();

    // TODO 52WeekHigh — максимум ціни за 52 тижні (розрахунок із історії котирувань).
    double fiftyTwoWeekHigh() const;

    // TODO 52WeekLow — мінімум ціни за 52 тижні (розрахунок із історії котирувань).
    double fiftyTwoWeekLow() const;

    // TODO 50DayMA — середня ціна за останні ~50 торгових днів.
    double movingAverage50Day() const;

    // TODO 200DayMA — середня ціна за останні ~200 торгових днів.
    double movingAverage200Day() const;

    double beta() const { return _beta; }
    FieldTOpt setBeta(double value)
    { return sdk::set_if(this, _beta, value, sdk::Quotes_beta); }

private:
    double _beta = 0.0;
};

#endif // SDK_INSTRUMENT_QUOTES_H
