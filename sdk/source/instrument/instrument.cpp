#include "instrument/instrument.h"

sdk::Instrument::Instrument(const Isin& isin) : _isin(isin)
{
    //
}

sdk::Data* const sdk::Instrument::data() const { return _data; }
bool sdk::Instrument::loaded() const { return _data != nullptr; }

sdk::Data* sdk::Instrument::create()
{
    if (_data == nullptr)
        _data = new Data();
    return _data;
}

sdk::Country sdk::Instrument::country() const { return _isin.country(); }


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


