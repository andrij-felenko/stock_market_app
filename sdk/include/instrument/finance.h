#ifndef SDK_INSTRUMENT_FINANCE_H
#define SDK_INSTRUMENT_FINANCE_H

#include "sdk.h" // IWYU pragma: keep
#include "capital.h"
#include "quartel_data.h"

class sdk::Finance : Trackable
{
public:
    Finance();

    Capital capital;


    QuartelData& quartel(uint16_t year, Quartel q);

    // TODO Прибуток на акцію (EPS). NetIncomeTTM / SharesOutstanding
    // Розраховується як чистий прибуток компанії за останні 12 місяців, поділений на к-сть акцій.
    // double earningsShare() const;

    // TODO ProfitMargin — рентабельність (NetIncome / Revenue).
    // Показує, яку частку від доходів компанія залишає у вигляді прибутку.
    // double profitMargin() const

    // TODO OperatingMarginTTM — операційна маржа.
    // Формула: OperatingIncomeTTM / RevenueTTM
    // double operatingMarginTtm() const

    // TODO ReturnOnAssetsTTM — рентабельність активів (ROA).
    // Формула: NetIncomeTTM / TotalAssets
    // double returnOnAssetsTtm() const

    // TODO ReturnOnEquityTTM — рентабельність власного капіталу (ROE).
    // Формула: NetIncomeTTM / ShareholderEquity
    // double returnOnEquityTtm() const

    // TODO RevenuePerShareTTM — виручка на акцію.
    // Формула: RevenueTTM / SharesOutstanding
    // double revenuePerShareTtm() const

    // TODO QuarterlyRevenueGrowthYOY — ріст виручки за квартал р/р.
    // Формула: (RevenueCurrentQuarter - RevenueSameQuarterLY) / RevenueSameQuarterLY
    // double quarterlyRevenueGrowthYoy() const

    // TODO DilutedEpsTTM — прибуток на акцію з розведенням.
    // Формула: NetIncomeTTM / DilutedSharesOutstanding
    // double dilutedEpsTtm() const

    // TODO QuarterlyEarningsGrowthYOY — ріст прибутку за квартал р/р.
    // Формула: (NetIncomeCurrentQuarter - NetIncomeSameQuarterLY) / NetIncomeSameQuarterLY
    // double quarterlyEarningsGrowthYoy() const

private:
    std::vector <QuartelData> _quartel_data;
};

#endif // SDK_INSTRUMENT_FINANCE_H
