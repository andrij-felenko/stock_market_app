#ifndef STOCK_MARKET_SDK_DEFENITION_H
#define STOCK_MARKET_SDK_DEFENITION_H

#include "utilities/define.h" // IWYU pragma: keep
#include "utilities/features.h" // IWYU pragma: keep
#include "enum/enum.h" // IWYU pragma: keep
#include <optional>

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

    // services
    class Market;
    class Finder;

    // Utilities
    class ResDir;
    class Trackable;
}

using FieldTOpt = std::optional <sdk::FieldType>;

#endif // STOCK_MARKET_SDK_DEFENITION_H
