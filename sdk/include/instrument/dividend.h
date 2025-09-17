#ifndef SDK_INSTRUMENT_DIVIDEND_H
#define SDK_INSTRUMENT_DIVIDEND_H

#include "sdk.h" // IWYU pragma: keep

class sdk::Dividend : Trackable
{
public:
    Dividend();

    double perShare() const { return _per_share; }
    FieldTOpt setPerShare(double v)
    { return sdk::set_if(this, _per_share, v, sdk::Dividend_per_share); }

    // TODO Дивідендна дохідність (Dividend Yield).
    // Показує відсоток доходу від дивідендів відносно ціни акції. DividendPerShare / Price
    // double yield() const

    double forwardAnnualRate() const { return _forward_annual_rate; }
    FieldTOpt setForwardAnnualRate(double v)
    { return sdk::set_if(this, _forward_annual_rate, v, sdk::Dividend_fw_anno_rate); }

    double forwardAnnualYield() const { return _forward_annual_yield; }
    FieldTOpt setForwardAnnualYield(double v)
    { return sdk::set_if(this, _forward_annual_yield, v, sdk::Dividend_fw_anno_yield); }

    double payout_ratio() const { return _payout_ratio; }
    FieldTOpt setPayoutRatio(double v)
    { return sdk::set_if(this, _payout_ratio, v, sdk::Dividend_payout_ratio); }

    const QDate& exDate() const { return _ex_date; }
    FieldTOpt setExDate(const QDate& date)
    { return sdk::set_if(this, _ex_date, date, sdk::Dividend_exdate); }

    const QDate& date() const { return _date; }
    FieldTOpt setDate(const QDate& date)
    { return sdk::set_if(this, _date, date, sdk::Dividend_date); }

    // TODO @brief Кількість дивідендних виплат на рік, історично по роках. */
    // std::optional<sdk::FieldType> setNumberOfDividendsByYear(const QMap<int,int>& v) {
    //     return sdk::set_if(*this, _numberOfDividendsByYear, v,
    //                        sdk::FieldType::TickerDividendNumberOfDividendsByYear);
    // }
    // const QMap<int,int>& numberOfDividendsByYear() const { return _numberOfDividendsByYear; }
    // private: QMap<int,int> _numberOfDividendsByYear;

private:
    double _per_share;
    double _yield;
    double _forward_annual_rate;
    double _forward_annual_yield;
    double _payout_ratio;
    QDate _date;
    QDate _ex_date;

    friend QDataStream& operator << (QDataStream& s, const Dividend& d);
    friend QDataStream& operator >> (QDataStream& s,       Dividend& d);
};

#endif // SDK_INSTRUMENT_DIVIDEND_H
