#ifndef SDK_INSTRUMENT_VALUATION_H
#define SDK_INSTRUMENT_VALUATION_H

#include "sdk_def.h" // IWYU pragma: keep

class sdk::Valuation : Trackable
{
public:
    Valuation();

    // TODO Ринкова капіталізація компанії.
    // Це загальна вартість усіх акцій на біржі (ціна × кількість акцій)
    // double marketCapitalization() const;

    // TODO P/E — співвідношення ціни акції до прибутку на акцію.
    // Показує, скільки інвестори платять за 1 долар прибутку. Price / EPS(TTM)
    // double priceToEarningsRatio() const

    // TODO PEG — співвідношення P/E до темпу зростання прибутку.
    // Значення біля 1 означає збалансовану оцінку PE / (EPS growth rate)
    // double pegRatio() const

    // TODO TrailingPE — P/E на базі TTM.
    // Формула: TrailingPE = lastTradePrice() / earningsPerShareTtm()
    // double priceToEarningsTtm() const {
    //     const double eps = earningsPerShareTtm(); // Finance::Derived
    //     const double p   = lastTradePrice();      // Ticker::quotes
    //     if (eps == 0.0) return std::numeric_limits<double>::quiet_NaN();
    //     return p / eps;
    // }

    // TODO ForwardPE — прогнозний P/E.
    // Формула: ForwardPE = lastTradePrice() / epsEstimateNextYear()
    // double priceToEarningsForward() const {
    //     const double epsNext = epsEstimateNextYear(); // Finance::Estimates
    //     const double p       = lastTradePrice();      // Ticker::quotes
    //     if (epsNext == 0.0) return std::numeric_limits<double>::quiet_NaN();
    //     return p / epsNext;
    // }

    // TODO PriceSalesTTM — ціна до виручки.
    // Формула: P/S = marketCapitalization() / revenueTtm()
    // (еквівалентно: lastTradePrice() / revenuePerShareTtm())
    // double priceToSalesTtm() const {
    //     const double rev = revenueTtm();              // Finance::Fundamental
    //     if (rev == 0.0) return std::numeric_limits<double>::quiet_NaN();
    //     return marketCapitalization() / rev;          // Valuation (cap = price * shares)
    // }

    // TODO PriceBookMRQ — ціна до балансової вартості (на останню звітну дату).
    // Формула A: P/B = lastTradePrice() / bookValuePerShareMrq()
    // Формула B: P/B = marketCapitalization() / totalShareholderEquityMrq()
    // double priceToBookMrq() const {
    //     const double bvps = bookValuePerShareMrq(); // Finance::Derived (з Fundamental+Capital)
    //     const double p    = lastTradePrice();
    //     if (bvps == 0.0) return std::numeric_limits<double>::quiet_NaN();
    //     return p / bvps;
    // }

    // TODO EnterpriseValue — вартість компанії з боргом і грошима.
    // Формула (типова): EV = MarketCap + TotalDebt + PreferredEquity + MinorityInterest - CashAndEquivalents
    // double enterpriseValue() const {
    //     const double mc   = marketCapitalization();           // Valuation
    //     const double debt = totalDebt();                      // Fundamental (BS)
    //     const double cash = cashAndCashEquivalents();         // Fundamental (BS)
    //     const double pref = preferredEquity();                // Fundamental (за наявності)
    //     const double mi   = minorityInterest();               // Fundamental (за наявності)
    //     return mc + debt + pref + mi - cash;
    // }

    // TODO EV/Revenue — вартість компанії до виручки.
    // Формула: EnterpriseValue / revenueTtm()
    // double enterpriseValueToRevenueTtm() const {
    //     const double rev = revenueTtm(); // Fundamental
    //     if (rev == 0.0) return std::numeric_limits<double>::quiet_NaN();
    //     return enterpriseValue() / rev;
    // }

    // TODO EV/EBITDA — вартість компанії до EBITDA.
    // Формула: EnterpriseValue / ebitdaTtm()
    // double enterpriseValueToEbitdaTtm() const {
    //     const double e = ebitdaTtm(); // Fundamental (TTM)
    //     if (e == 0.0) return std::numeric_limits<double>::quiet_NaN();
    //     return enterpriseValue() / e;
    // }

private:
    friend QDataStream& operator << (QDataStream& s, const Valuation& d);
    friend QDataStream& operator >> (QDataStream& s,       Valuation& d);
};

#endif // SDK_INSTRUMENT_VALUATION_H
