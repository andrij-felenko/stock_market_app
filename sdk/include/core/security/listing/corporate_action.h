#ifndef SDK_INSTRUMENT_CORPORATE_ACTION_H
#define SDK_INSTRUMENT_CORPORATE_ACTION_H

#include "sdk_def.h" // IWYU pragma: keep

class sdk::CorporateAction : Trackable
{
public:
    CorporateAction() = default;

    const QDate& lastSplitDate() const;
    std::optional<sdk::FieldType> setLastSplitDate(const QDate& v);

    std::pair <int, int> lastSplitFactor() const;
    FieldTOpt setLastSplitFactor(int a, int b);

private:
    std::pair <int, int> _last_split_factor;
    QDate _last_split_date;

    friend QDataStream& operator << (QDataStream& s, Wire <const CorporateAction> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      CorporateAction> d);
};

#endif // SDK_INSTRUMENT_CORPORATE_ACTION_H
