#ifndef STICK_MARKET_SDK_H
#define STICK_MARKET_SDK_H

#include "utilities/define.h" // IWYU pragma: keep
#include "utilities/features.h" // IWYU pragma: keep
#include "geo/geo.h" // IWYU pragma: keep
#include <optional>

namespace sdk {
enum FieldType {
    Ticker_code,
    Quotes_beta,

    Meta_type,
    Meta_name,
    Meta_sector,
    Meta_industry,
    Meta_gic_sector,
    Meta_gic_group,
    Meta_gic_industry,
    Meta_gic_subindustry,
    Meta_home_category,
    Meta_description,
    Meta_officers,
    Meta_phone_number,
    Meta_website,
    Meta_logo,
    Meta_logo_url,
    Meta_logo_full,
    Meta_fulltime_employees,

    Legal_openFigi,
    Legal_isin,
    Legal_lei,
    Legal_cik,
    Legal_fiscalYearEnd,
    Legal_ein,
    Legal_cusip,
    Legal_ipoDate,
    Legal_internationalDomestic,
    Legal_delisted,
    Legal_address,
    Legal_address_json,

    Fundam_ebitda,
    Fundam_bookvalue,
    Fundam_recent_quart,
    Fundam_revenue_ttm,
    Fundam_gross_profit_ttm,
    Fundam_annual_earnings,
    Fundam_balance, // TODO separate it
    Fundam_cashflow, // TODO separate it
    Fundam_incomes, // TODO separate it
    Fundam_earnings, // TODO separate it

    Est_wallstr_target_price,
    Est_eps_curr_year,
    Est_eps_next_year,
    Est_eps_curr_quart,
    Est_eps_next_quart,
    Est_trend,

    Analyst_rate,
    Analyst_targetprice,
    Analyst_strongbuy,
    Analyst_buy,
    Analyst_hold,
    Analyst_sell,
    Analyst_strongsell,

    Dividend_per_share,
    Dividend_yield,
    Dividend_fw_anno_rate,
    Dividend_fw_anno_yield,
    Dividend_payout_ratio,
    Dividend_date,
    Dividend_exdate,

    Fin_earnings_share,

    Cap_shares_outstand,
    Cap_shares_float,
    Cap_percent_of_insiders,
    Cap_percent_institution,
    Cap_outstand_shares,

    Short_shares,
    Short_shares_priormonth,

    Corp_last_split_factor,
    Corp_split_date,

    Earnings_q,
    Balance_q,
    CashFlow_q,
    Incomes_q,
    Trend_q,
};
}

using FieldTOpt = std::optional <sdk::FieldType>;

namespace sdk {
    // instrument
    class Balance;
    class Capital;
    class CashFlow;
    class CorporateAction;
    class Data;
    class Dividend;
    class Earnings;
    class Estimate;
    class Finance;
    class Fundamental;
    class Incomes;
    class Instrument;
    class Isin;
    class Legal;
    class Meta;
    class Market;
    class QuartelData;

    class Quotes;
    struct QuotesDate;
    struct QuotesTime;

    class ShortInterest;
    class Ticker;
    class Transaction;
    class Trend;
    class User;
    class Valuation;
    class Stock;
    class Symbol;

    // Utilities
    class ResDir;
    class Trackable;
}

#endif // STICK_MARKET_SDK_H
