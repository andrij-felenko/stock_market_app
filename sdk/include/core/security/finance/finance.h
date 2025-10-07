#ifndef SDK_INSTRUMENT_FINANCE_H
#define SDK_INSTRUMENT_FINANCE_H

#include "sdk_def.h" // IWYU pragma: keep
#include "capital.h"
#include "estimate.h"
#include "fundamental.h"
#include "core/security/quarter/quarter_data.h"

class sdk::Finance
{
public:
    Finance();

    Capital capital;
    Estimate estimates;
    Fundamental fundamental;

    QuartelData& quartel(uint16_t year, Quartel q);
    QuartelData& quartel(const QDate& date);

    bool hasQuartel(uint16_t year, Quartel q) const;
    bool hasQuartel(const QDate& date) const;

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
    Trackable _track;
    sdk::List <QuartelData, Finance> _quartel_data;

    friend QDataStream& operator << (QDataStream& s, Wire <const Finance> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Finance> d);
};

#endif // SDK_INSTRUMENT_FINANCE_H
