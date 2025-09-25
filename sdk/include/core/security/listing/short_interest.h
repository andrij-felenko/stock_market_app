#ifndef SDK_INSTRUMENT_SHORT_INTEREST_H
#define SDK_INSTRUMENT_SHORT_INTEREST_H

#include "sdk_def.h" // IWYU pragma: keep

class sdk::ShortInterest : Trackable
{
public:
    ShortInterest();

    int64_t shares() const { return _shares; }
    FieldTOpt setShortInterestShares(int64_t value)
    { return sdk::set_if(this, _shares, value, sdk::Short_shares); }


    int64_t sharesPriorMonth() const { return _shares_prior_month; }
    FieldTOpt setSharesPriorMonth(int64_t value)
    { return sdk::set_if(this, _shares_prior_month, value, sdk::Short_shares_priormonth); }

    // TODO ShortInterestRatio (Days To Cover) — скільки днів потрібно, щоб покрити шорт.
    // Формула: DaysToCover = SharesShort / AverageDailyVolume
    // std::optional<double> shortInterestRatioDaysToCover() const {
    //     if (!_shortInterestShares) return std::nullopt;
    //     const double adv = _ticker.quotes().averageDailyVolume(); // якщо є
    //     if (adv <= 0.0) return std::nullopt;
    //     return static_cast<double>(*_shortInterestShares) / adv;
    // }

    // TODO Частка шорту від усіх акцій.
    // Формула: ShortPercentOutstanding = SharesShort / SharesOutstanding
    // std::optional<double> shortInterestAsPercentageOfOutstandingShares() const {
    //     if (!_shortInterestShares) return std::nullopt;
    //     const double so = _ticker.company().finance().capital().sharesOutstanding();
    //     if (so == 0.0) return std::nullopt;
    //     return static_cast<double>(*_shortInterestShares) / so;
    // }

    // TODO Частка шорту від float.
    // Формула: ShortPercentFloat = SharesShort / SharesFloat
    // std::optional<double> shortInterestAsPercentageOfFloat() const {
    //     if (!_shortInterestShares) return std::nullopt;
    //     const double fl = _ticker.company().finance().capital().sharesFloat();
    //     if (fl == 0.0) return std::nullopt;
    //     return static_cast<double>(*_shortInterestShares) / fl;
    // }


private:
    int64_t _shares;
    int64_t _shares_prior_month;

    friend QDataStream& operator << (QDataStream& s, Wire <const ShortInterest> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      ShortInterest> d);
};

#endif // SDK_INSTRUMENT_SHORT_INTEREST_H
