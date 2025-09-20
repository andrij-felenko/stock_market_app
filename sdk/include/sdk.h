#ifndef STOCK_MARKET_SDK_H
#define STOCK_MARKET_SDK_H

#include <optional> // IWYU pragma: keep

#include "api/api.h"          // IWYU pragma: keep
#include "api/alphavantage.h" // IWYU pragma: keep
#include "api/eod.h"          // IWYU pragma: keep
#include "api/figi.h"         // IWYU pragma: keep
#include "api/finnhub.h"      // IWYU pragma: keep
#include "api/openai.h"       // IWYU pragma: keep
#include "api/reply.h"        // IWYU pragma: keep
#include "api/twelvedata.h"   // IWYU pragma: keep
#include "api/marketstack.h"  // IWYU pragma: keep
#include "api/filefetch.h"    // IWYU pragma: keep

#include "instrument/data.h"            // IWYU pragma: keep
#include "instrument/trend.h"           // IWYU pragma: keep
#include "instrument/earnings.h"        // IWYU pragma: keep
#include "instrument/quartel_data.h"    // IWYU pragma: keep
#include "instrument/incomes.h"         // IWYU pragma: keep
#include "instrument/cash_flow.h"       // IWYU pragma: keep
#include "instrument/balance.h"         // IWYU pragma: keep
#include "instrument/quotes.h"          // IWYU pragma: keep
#include "instrument/short_interest.h"  // IWYU pragma: keep
#include "instrument/ticker.h"          // IWYU pragma: keep
#include "instrument/valuation.h"       // IWYU pragma: keep
#include "instrument/capital.h"         // IWYU pragma: keep
#include "instrument/corporate_action.h"// IWYU pragma: keep
#include "instrument/dividend.h"        // IWYU pragma: keep
#include "instrument/estimate.h"        // IWYU pragma: keep
#include "instrument/fundamental.h"     // IWYU pragma: keep
#include "instrument/instrument.h"      // IWYU pragma: keep
#include "instrument/legal.h"           // IWYU pragma: keep
#include "instrument/meta.h"            // IWYU pragma: keep
#include "instrument/finance.h"         // IWYU pragma: keep
#include "instrument/symbol.h"          // IWYU pragma: keep
#include "instrument/isin.h"            // IWYU pragma: keep

#include "services/market.h" // IWYU pragma: keep

#include "account/account.h"   // IWYU pragma: keep
#include "account/info.h"      // IWYU pragma: keep
#include "stock/stock.h"       // IWYU pragma: keep
#include "stock/transaction.h" // IWYU pragma: keep

#include "enum/enum.h" // IWYU pragma: keep

#include "settings/network.h" // IWYU pragma: keep

#include "utilities/define.h"    // IWYU pragma: keep
#include "utilities/res_dir.h"   // IWYU pragma: keep
#include "utilities/features.h"  // IWYU pragma: keep
#include "utilities/trackable.h" // IWYU pragma: keep

#include "loader.h" // IWYU pragma: keep

#endif // STOCK_MARKET_SDK_H
