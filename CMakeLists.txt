cmake_minimum_required(VERSION 3.24)
project(stock_manager)

set(project_name "Stock manager")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)

set (QT_QML_GENERATE_QMLLS_INI ON)

# destinations folders
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/bin/app)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/bin/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/bin/archive)

find_package (Qt6 REQUIRED COMPONENTS
    Core Network WebSockets
    Gui Widgets Svg
    Qml QuickControls2 Quick
)

set (Qt_lib_core  Qt6::Core   Qt6::Network         Qt6::WebSockets)
set (Qt_lib_gui   Qt6::Gui    Qt6::Widgets         Qt6::Svg)
set (Qt_lib_quick Qt6::Qml    Qt6::QuickControls2  Qt::Quick)
set (Qt_libs ${Qt_lib_core} ${Qt_lib_gui} ${Qt_lib_quick})

# app -------------------------------------------------------------------------
set (src
    main.cpp

    api/api.cpp
    api/api.h
    api/alphavantage.cpp
    api/alphavantage.h
    api/eod.cpp
    api/eod.h
    api/figi.cpp
    api/figi.h
    api/finnhub.cpp
    api/finnhub.h
    api/openai.cpp
    api/openai.h
    api/reply.cpp
    api/reply.h
    api/twelvedata.cpp
    api/twelvedata.h
    api/marketstack.cpp
    api/marketstack.h

    chart/quote.cpp
    chart/quote.h

    data/market.cpp
    data/market.h

    data/instrument.cpp
    data/instrument.h
    data/instrument/ticker.cpp
    data/instrument/ticker.h
    data/instrument/quotes.cpp
    data/instrument/quotes.h
    data/instrument/stability.cpp
    data/instrument/stability.h
    data/instrument/valuation.cpp
    data/instrument/valuation.h
    data/instrument/dividend.cpp
    data/instrument/dividend.h
    data/instrument/identity.cpp
    data/instrument/identity.h
    data/instrument/profitability.cpp
    data/instrument/profitability.h

    data/user.h
    data/user.cpp
    data/user/info.cpp
    data/user/info.h
    data/user/stock.cpp
    data/user/stock.h
    data/user/transaction.cpp
    data/user/transaction.h

    data/currency.cpp
    data/currency.h
    data/currency/data.cpp
    data/currency/data.h
    data/currency/exchange.cpp
    data/currency/exchange.h
    data/currency/name.cpp
    data/currency/name.h

    model/search_tag.h
    model/search_tag.cpp
    model/asset_list.cpp
    model/asset_list.h
    model/instrument_list.cpp
    model/instrument_list.h

    settings/network.cpp
    settings/network.h

    utilities/define.h
    utilities/res_dir.cpp
    utilities/res_dir.h
    utilities/features.cpp
    utilities/features.h
)

qt_add_executable(stock_manager ${src})
target_link_libraries (stock_manager PUBLIC ${Qt_libs})

if(ANDROID)
    include(${CMAKE_SOURCE_DIR}/android_openssl/android_openssl.cmake)

    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android")
    add_android_openssl_libraries(stock_manager)
endif()

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)
set(QT_QML_GENERATE_QMLLS_INI ON)

qt_add_qml_module (stock_manager
    URI Stock
    VERSION 1.0
    NAMESPACE ui
    SOURCES
    QML_FILES
        main.qml
        ui/Account.qml
        ui/Settings.qml
        ui/StockFull.qml
        ui/Favorite.qml
        ui/Main.qml
        ui/Planned.qml
        ui/Portfolio.qml
        ui/MarketFilter.qml
        ui/StockShort.qml
        ui/Charts.qml
        ui/Market.qml
)

qt_add_resources(stock_manager "rc_images"
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

file(GLOB tags_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} rc/tags/*.edsm)
qt_add_resources(stock_manager "rc_tags"
    PREFIX "/"
    FILES
        ${tags_files}
)

file(GLOB stock_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} rc/stocks/*.tdsm)
qt_add_resources(stock_manager "rc_stocks"
    PREFIX "/"
    FILES
        ${stock_files}
)
