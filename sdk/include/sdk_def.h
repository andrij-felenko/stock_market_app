#ifndef STOCK_MARKET_SDK_DEFENITION_H
#define STOCK_MARKET_SDK_DEFENITION_H

#include "common/define.h" // IWYU pragma: keep
#include "common/features.h" // IWYU pragma: keep
#include "core/enum.h" // IWYU pragma: keep
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
    class Trend;
    class User;
    class Valuation;
    class Symbol;

    // services
    class Market;
    class Roster;
    namespace market { class Finder; }

    // Utilities
    class ResDir;
    class Trackable;
    class DTime;

    namespace api {
        class Syncer;
        class Call;
        class Provider;
        class FileFetch;

        class EndPoints;
        class OpenAI;
        class Ibkr;
        class Tradernet;
        class AlphaVantage;
        class Eod;
        class FinnHub;
        class Figi;
        class MarketStack;
        class TwelveData;
        class Yahoo;
    }

    // user
    class Account;
    class Profile;
    class Identity;
    class Session;
    class Ledger;
    class Pending;
    class Portfolio;
    class Screener;
    class Targets;
    class WatchList;
    class Stock;
    class Transaction;
}

using FieldTOpt = std::optional <sdk::FieldType>;

#endif // STOCK_MARKET_SDK_DEFENITION_H
