#ifndef SDK_INSTRUMENT_QUARTEL_DATA_H
#define SDK_INSTRUMENT_QUARTEL_DATA_H

#include "sdk.h" // IWYU pragma: keep
#include "earnings.h"
#include "balance.h"
#include "cash_flow.h"
#include "incomes.h"
#include "trend.h"

class sdk::QuartelData
{
public:
    QuartelData(uint16_t year = QDate().year(), Quartel q = Quartel::Annual);

    Quartel quartel() const;
    uint16_t year() const;

    Balance  balance;
    CashFlow cashflow;
    Earnings earning;
    Incomes  incomes;
    Trend    trend;

    sdk::Currency currency() const;
    void setCurrency(sdk::Currency new_c);

private:
    sdk::Currency _currency;
    uint16_t _year = QDate().year();
    Quartel _quartel = Quartel::Annual;

    friend QDataStream& operator << (QDataStream& s, const QuartelData& d);
    friend QDataStream& operator >> (QDataStream& s,       QuartelData& d);
};

#endif // SDK_INSTRUMENT_QUARTEL_DATE_H
