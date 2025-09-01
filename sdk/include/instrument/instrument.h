#ifndef SDK_INSTRUMENT_H
#define SDK_INSTRUMENT_H

// Instrument {
//   Listing = vector<Ticker> {
//     dividend,
//     quotes,
//     short_interest,
//     corporate_action,
//     valuation   // завжди per-ticker, залежить від біржі/ціни
//   }

//   Legal        // юр. інформація: країна реєстрації, форма, регуляторні дані
//   Meta         // назва, опис, сектор, галузь, регіон

//   Finance {
//     QuartelData {
//       Balance
//       CashFlow
//       Earnings
//       Incomes
//       Trend
//     }
//     -Derived     // EPS TTM, BVPS MRQ, SalesPS TTM, NetDebt — обчислюється з Fundamental+Capital
//     Fundamental // звіти IS/BS/CF по періодах (raw data)
//     Capital     // shares_outstanding, float, insider%, institutions%
//     Estimates   // аналітичні прогнози: EPS next, Revenue next, Target price, Rating...
//   }
// }

#include "sdk.h" // IWYU pragma: keep

class sdk::Instrument
{
    //
};
#endif // SDK_INSTRUMENT_H
