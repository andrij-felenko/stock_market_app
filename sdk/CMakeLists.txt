cmake_minimum_required(VERSION 3.24)
project(sdk)

# app -------------------------------------------------------------------------
set (sdk_h
    include/sdk.h
    include/api/api.h
    include/api/alphavantage.h
    include/api/eod.h
    include/api/figi.h
    include/api/finnhub.h
    include/api/openai.h
    include/api/reply.h
    include/api/twelvedata.h
    include/api/marketstack.h
    include/api/filefetch.h

    include/instrument/trend.h
    include/instrument/earnings.h
    include/instrument/quartel_data.h
    include/instrument/incomes.h
    include/instrument/cash_flow.h
    include/instrument/balance.h
    include/instrument/quotes.h
    include/instrument/short_interest.h
    include/instrument/ticker.h
    include/instrument/valuation.h
    include/instrument/capital.h
    include/instrument/corporate_action.h
    include/instrument/dividend.h
    include/instrument/estimate.h
    include/instrument/fundamental.h
    include/instrument/instrument.h
    include/instrument/legal.h
    include/instrument/meta.h
    include/instrument/finance.h
    include/instrument/symbol.h
    include/instrument/isin.h

    include/data/market.h

    include/data/instrument.h
    include/data/instrument/ticker.h
    include/data/instrument/quotes.h
    include/data/instrument/stability.h
    include/data/instrument/valuation.h
    include/data/instrument/dividend.h
    include/data/instrument/identity.h
    include/data/instrument/profitability.h
    include/data/instrument/shares.h
    include/data/instrument/earnings.h
    include/data/instrument/balance.h
    include/data/instrument/meta.h

    include/data/user.h
    include/data/user/info.h
    include/data/user/stock.h
    include/data/user/transaction.h

    include/geo/geo.h

    include/settings/network.h

    include/utilities/define.h
    include/utilities/res_dir.h
    include/utilities/features.h
    include/utilities/trackable.h

    include/loader.h
)

set (sdk_cpp
    source/api/api.cpp
    source/api/alphavantage.cpp
    source/api/eod.cpp
    source/api/figi.cpp
    source/api/finnhub.cpp
    source/api/openai.cpp
    source/api/reply.cpp
    source/api/twelvedata.cpp
    source/api/marketstack.cpp
    source/api/filefetch.cpp

    source/instrument/trend.cpp
    source/instrument/earnings.cpp
    source/instrument/quartel_data.cpp
    source/instrument/incomes.cpp
    source/instrument/cash_flow.cpp
    source/instrument/balance.cpp
    source/instrument/legal.cpp
    source/instrument/meta.cpp
    source/instrument/quotes.cpp
    source/instrument/short_interest.cpp
    source/instrument/ticker.cpp
    source/instrument/valuation.cpp
    source/instrument/capital.cpp
    source/instrument/corporate_action.cpp
    source/instrument/dividend.cpp
    source/instrument/estimate.cpp
    source/instrument/fundamental.cpp
    source/instrument/instrument.cpp
    source/instrument/finance.cpp
    source/instrument/symbol.cpp
    source/instrument/isin.cpp

    source/data/market.cpp

    source/data/instrument.cpp
    source/data/instrument/ticker.cpp
    source/data/instrument/quotes.cpp
    source/data/instrument/stability.cpp
    source/data/instrument/valuation.cpp
    source/data/instrument/dividend.cpp
    source/data/instrument/identity.cpp
    source/data/instrument/profitability.cpp
    source/data/instrument/shares.cpp
    source/data/instrument/earnings.cpp
    source/data/instrument/balance.cpp
    source/data/instrument/meta.cpp

    source/data/user.cpp
    source/data/user/info.cpp
    source/data/user/stock.cpp
    source/data/user/transaction.cpp

    source/geo/continent.cpp
    source/geo/country.cpp
    source/geo/currency.cpp
    source/geo/exchange.cpp
    source/geo/region.cpp
    source/geo/instype.cpp

    source/settings/network.cpp

    source/utilities/res_dir.cpp
    source/utilities/features.cpp
    source/utilities/trackable.cpp

    source/loader.cpp
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
