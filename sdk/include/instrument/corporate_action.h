#ifndef SDK_INSTRUMENT_CORPORATE_ACTION_H
#define SDK_INSTRUMENT_CORPORATE_ACTION_H

#include "sdk.h" // IWYU pragma: keep

class sdk::CorporateAction : Trackable
{
public:
    CorporateAction();

    const QDate& lastSplitDate() const { return _last_split_date; }
    std::optional<sdk::FieldType> setLastSplitDate(const QDate& v)
    { return sdk::set_if(this, _last_split_date, v, sdk::Corp_split_date); }

    std::pair <int, int> lastSplitFactor() const { return _last_split_factor; }
    FieldTOpt setLastSplitFactor(int a, int b)
    { return sdk::set_if(this, _last_split_factor, std::make_pair(a, b),
                         sdk::Corp_last_split_factor); }

private:
    std::pair <int, int> _last_split_factor;
    QDate _last_split_date;
    //
};

#endif // SDK_INSTRUMENT_CORPORATE_ACTION_H
