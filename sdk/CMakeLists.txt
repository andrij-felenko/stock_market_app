cmake_minimum_required(VERSION 3.24)
project(sdk)

# app -------------------------------------------------------------------------
set (sdk_h
    include/sdk.h
    include/sdk_def.h

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

    include/instrument/data.h
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

    include/services/finder.h
    include/services/market.h

    include/account/account.h
    include/account/info.h
    include/stock/stock.h
    include/stock/transaction.h

    include/enum/enum.h

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

    source/instrument/data.cpp
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

    source/services/finder.cpp
    source/services/market.cpp

    source/account/account.cpp
    source/account/info.cpp
    source/stock/stock.cpp
    source/stock/transaction.cpp

    source/enum/continent.cpp
    source/enum/country.cpp
    source/enum/currency.cpp
    source/enum/exchange.cpp
    source/enum/region.cpp
    source/enum/instype.cpp
    source/enum/month.cpp
    source/enum/quartel.cpp
    source/enum/home_category.cpp

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
