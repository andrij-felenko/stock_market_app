cmake_minimum_required(VERSION 3.24)
project(sdk)

# app -------------------------------------------------------------------------
set (sdk_h
    include/loader.h
    include/sdk.h
    include/sdk_def.h

    include/api/ai/openai.h
    # include/api/broker/ibkr.h
    # include/api/broker/tradernet.h
    include/api/connectors/endpoints.h
    include/api/eod/alphavantage.h
    include/api/eod/eod.h
    include/api/eod/figi.h
    include/api/eod/finnhub.h
    include/api/eod/marketstack.h
    include/api/eod/twelvedata.h
    include/api/eod/yahoo.h
    include/api/transport/filefetch.h
    include/api/transport/provider.h
    include/api/transport/call.h
    include/api/transport/syncer.h

    include/common/dtime.h
    include/common/json.h
    include/common/define.h
    include/common/features.h
    include/common/res_dir.h
    include/common/trackable.h

    include/core/enum.h
    include/core/isin.h
    include/core/symbol.h
    include/core/forex/pair.h

    include/core/security/data.h
    include/core/security/instrument.h
    include/core/security/legal.h
    include/core/security/meta.h

    include/core/security/finance/capital.h
    include/core/security/finance/estimate.h
    include/core/security/finance/finance.h
    include/core/security/finance/fundamental.h

    include/core/security/listing/corporate_action.h
    include/core/security/listing/dividend.h
    include/core/security/listing/quotes.h
    include/core/security/listing/short_interest.h
    include/core/security/listing/ticker.h
    include/core/security/listing/valuation.h

    include/core/security/quarter/balance.h
    include/core/security/quarter/cash_flow.h
    include/core/security/quarter/earnings.h
    include/core/security/quarter/incomes.h
    include/core/security/quarter/quarter_data.h
    include/core/security/quarter/trend.h

    include/service/finder.h
    include/service/forex.h
    include/service/market.h

    include/user/account/account.h
    include/user/account/profile.h
    include/user/account/session.h

    include/user/ledger/ledger.h
    include/user/ledger/pending.h
    include/user/ledger/portfolio.h
    include/user/ledger/screener.h
    include/user/ledger/targets.h
    include/user/ledger/watchlist.h

    include/user/stock/stock.h
    include/user/stock/transaction.h
)

set (sdk_cpp
    source/loader.cpp

    source/api/ai/openai.cpp
    # source/api/broker/ibkr.cpp
    # source/api/broker/tradernet.cpp
    source/api/connectors/endpoints.cpp
    source/api/eod/alphavantage.cpp
    source/api/eod/eod.cpp
    source/api/eod/figi.cpp
    source/api/eod/finnhub.cpp
    source/api/eod/marketstack.cpp
    source/api/eod/twelvedata.cpp
    source/api/eod/yahoo.cpp
    source/api/transport/filefetch.cpp
    source/api/transport/provider.cpp
    source/api/transport/call.cpp
    source/api/transport/syncer.cpp

    source/common/dtime.cpp
    source/common/json.cpp
    source/common/features.cpp
    source/common/res_dir.cpp
    source/common/trackable.cpp

    source/core/isin.cpp
    source/core/symbol.cpp
    source/core/forex/pair.cpp

    source/core/enum/continent.cpp
    source/core/enum/country.cpp
    source/core/enum/currency.cpp
    source/core/enum/exchange.cpp
    source/core/enum/home_category.cpp
    source/core/enum/instype.cpp
    source/core/enum/month.cpp
    source/core/enum/quarter.cpp
    source/core/enum/region.cpp

    source/core/security/data.cpp
    source/core/security/instrument.cpp
    source/core/security/legal.cpp
    source/core/security/meta.cpp

    source/core/security/finance/capital.cpp
    source/core/security/finance/estimate.cpp
    source/core/security/finance/finance.cpp
    source/core/security/finance/fundamental.cpp

    source/core/security/listing/corporate_action.cpp
    source/core/security/listing/dividend.cpp
    source/core/security/listing/quotes.cpp
    source/core/security/listing/short_interest.cpp
    source/core/security/listing/ticker.cpp
    source/core/security/listing/valuation.cpp

    source/core/security/quarter/balance.cpp
    source/core/security/quarter/cash_flow.cpp
    source/core/security/quarter/earnings.cpp
    source/core/security/quarter/incomes.cpp
    source/core/security/quarter/quarter_data.cpp
    source/core/security/quarter/trend.cpp

    source/service/finder.cpp
    source/service/forex.cpp
    source/service/market.cpp

    source/user/account/account.cpp
    source/user/account/profile.cpp
    source/user/account/session.cpp

    source/user/ledger/ledger.cpp
    source/user/ledger/pending.cpp
    source/user/ledger/portfolio.cpp
    source/user/ledger/screener.cpp
    source/user/ledger/targets.cpp
    source/user/ledger/watchlist.cpp

    source/user/stock/stock.cpp
    source/user/stock/transaction.cpp
)

set (sdk_files
    ${sdk_cpp} ${sdk_h}
)

add_library(sdk STATIC ${sdk_files})
target_include_directories(sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries (sdk PUBLIC ${Qt_lib_core})

qt_add_resources(sdk "rc_images"
    PREFIX "/"
    FILES
        rc/images/presentation.png
        rc/images/website.png
        rc/images/briefcase.png
        rc/images/folder.png
        rc/images/browser.png
        rc/images/bull-market.png
        rc/images/star.png
        rc/images/star_black.png
        rc/images/star_black_border.png
        rc/images/star_empty.png
        rc/images/stock.png
        rc/images/Stock_app_logo.png
        rc/images/Stock_app_logo2.png
        rc/images/person.png
)

file(GLOB tickers_meta_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} rc/tickers_meta/*.edsm)
qt_add_resources(sdk "rc_tickers_meta"
    PREFIX "/"
    FILES
        ${tickers_meta_files}
)

file(GLOB stock_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} rc/stocks/*.tdsm)
qt_add_resources(sdk "rc_stocks"
    PREFIX "/"
    FILES
        ${stock_files}
)
