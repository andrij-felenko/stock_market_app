#ifndef METRIC_H
#define METRIC_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

#include "currency/name.h"

namespace data { class Metric; }

class data::Metric : public QObject
{
public:
    Metric(QObject* parent = nullptr);

    // оцінка компанії
    double _market_capitalization; // "marketCapitalization": 141555.53,
    double _enterprise_value; // "enterpriseValue": 890337.53,
    double _p_e_ttm; // "peTTM": 10.5836,
    double _p_b_quarterly; // "pbQuarterly": 0.629,
    double _price_to_sales_ttm; // "psAnnual": 1.1491,
    double _price_to_free_cash_flow; // "pfcfShareTTM": 6.1258,

    // ринкова динаміка
    double _52_week_high_v; // "52WeekHigh": 84.74,
    QDate  _52_week_high_date; // "52WeekHighDate": "2025-02-18",
    double _52_week_low_v; // "52WeekLow": 53.51,
    QDate  _52_week_low_date; // "52WeekLowDate": "2024-08-05",
    double _52_week_price_return; // "52WeekPriceReturnDaily": 22.3963,
    double _5_day_price_return; // "5DayPriceReturnDaily": 0.4373,
    double _current_year_price_return; // "yearToDatePriceReturnDaily": 7.6858
    double _montly_price_return; // "monthToDatePriceReturnDaily": 0.6373,
    double _beta; // "beta": 1.3568959,
    double _relative_to_sp500; // "priceRelativeToS&P50052Week": 8.9829,


    // Дивіденди
    // "dividendGrowthRate5Y": 2.51,
    // "dividendPerShareAnnual": 2.7507,
    // "dividendPerShareTTM": 2.7713,
    // "dividendYieldIndicatedAnnual": 2.9739776,
    // "payoutRatioTTM": 39.11,



    // Рентабельність
    // "roeTTM": 6.38,
    // "roaTTM": 0.55,
    // "operatingMarginTTM": 23.01502,
    // "netProfitMarginTTM": 17.3666,
    // "pretaxMarginTTM": 23.01502,


    // Зростання
    // "epsGrowthTTMYoy": 70.01,
    // "revenueGrowthTTMYoy": 99.23775,
    // "revenueGrowth3Y": -0.9243,
    // "epsGrowth3Y": -15.18,


    // Фінансова стійкість
    // "totalDebt/totalEquityAnnual": 2.7664,
    // "longTermDebt/equityQuarterly": 1.3921,
    // "cashPerSharePerShareAnnual": 170.3001,
    // "bookValuePerShareQuarterly": 113.727,








// "10DayAverageTradingVolume": 11.62567,
// "13WeekPriceReturnDaily": -5.1907,
// "26WeekPriceReturnDaily": 9.9347,
// "3MonthADReturnStd": 46.49328,
// "3MonthAverageTradingVolume": 16.0154,
// "bookValuePerShareAnnual": 111.1295,
// "bookValueShareGrowth5Y": 3.99,
// "capexCagr5Y": 4.03,
// "cashFlowPerShareAnnual": -13.9414,
// "cashFlowPerShareQuarterly": -39.5974,
// "cashFlowPerShareTTM": 8.7165,
// "cashPerSharePerShareQuarterly": 123.4899,
// "currentDividendYieldTTM": 3.6954,
// "ebitdPerShareAnnual": 11.0082,
// "ebitdPerShareTTM": 11.4783,
// "ebitdaCagr5Y": -5.14,
// "ebitdaInterimCagr5Y": 9.89,
// "epsAnnual": 6.5368,
// "epsBasicExclExtraItemsAnnual": 6.5368,
// "epsBasicExclExtraItemsTTM": 6.9182,
// "epsExclExtraItemsAnnual": 6.5368,
// "epsExclExtraItemsTTM": 6.9182,
// "epsGrowth5Y": -5.26,
// "epsGrowthQuarterlyYoy": 22.04,
// "epsInclExtraItemsAnnual": 6.5368,
// "epsInclExtraItemsTTM": 6.9182,
// "epsNormalizedAnnual": 6.5368,
// "epsTTM": 6.9182,
// "focfCagr5Y": null,
// "longTermDebt/equityAnnual": 1.3365,
// "netIncomeEmployeeAnnual": 0.0535,
// "netIncomeEmployeeTTM": 0.0564,
// "netMarginGrowth5Y": -1.80381,
// "netProfitMargin5Y": 23.18555,
// "netProfitMarginAnnual": 20.13062,
// "operatingMargin5Y": 28.99823,
// "operatingMarginAnnual": 24.96515,
// "payoutRatioAnnual": 41,
// "pb": 0.6664,
// "pbAnnual": 0.6382,
// "pcfShareAnnual": null,
// "pcfShareTTM": null,
// "peAnnual": 11.1619,
// "peBasicExclExtraTTM": 10.5836,
// "peExclExtraAnnual": 6.35881,
// "peExclExtraTTM": 10.5836,
// "peInclExtraTTM": 10.5836,
// "peNormalizedAnnual": 11.1619,
// "pfcfShareAnnual": 7.2828,
// "pretaxMargin5Y": 28.99823,
// "pretaxMarginAnnual": 24.96515,
// "priceRelativeToS&P50013Week": -4.9433,
// "priceRelativeToS&P50026Week": 10.0964,
// "priceRelativeToS&P5004Week": 2.7339,
// "priceRelativeToS&P500Ytd": 6.5546,
// "psTTM": 1.09533,
// "ptbvAnnual": 0.6522,
// "ptbvQuarterly": 0.6424,
// "revenueEmployeeAnnual": 325131.6,
// "revenueGrowth5Y": 3.85739,
// "revenueGrowthQuarterlyYoy": 74.89332,
// "revenuePerShareAnnual": 38.35107,
// "revenuePerShareTTM": 40.3204,
// "revenueShareGrowth5Y": 7.39387,
// "roa5Y": 0.6,
// "roaRfy": 0.54,
// "roe5Y": 6.87,
// "roeRfy": 6.08,
// "roi5Y": 1.95,
// "roiAnnual": 1.61,
// "roiTTM": 1.59,
// "tangibleBookValuePerShareAnnual": 108.7354,
// "tangibleBookValuePerShareQuarterly": 111.3551,
// "tbvCagr5Y": 1.61,
// "totalDebt/totalEquityQuarterly": 3.5252,
};

#endif
