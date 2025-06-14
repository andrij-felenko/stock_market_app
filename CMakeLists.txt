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
    api/eod.cpp
    api/eod.h
    api/finnhub.cpp
    api/finnhub.h
    api/freedom24.cpp
    api/freedom24.h
    api/ibkr.cpp
    api/ibkr.h
    api/openai.cpp
    api/openai.h
    api/yahoo.cpp
    api/yahoo.h
    api/reply.cpp
    api/reply.h
    api/twelvedata.cpp
    api/twelvedata.h

    chart/quote.cpp
    chart/quote.h

    data/market.cpp
    data/market.h
    data/ticker.cpp
    data/ticker.h
    data/ticker/quotes.cpp
    data/ticker/quotes.h
    data/ticker/stability.cpp
    data/ticker/stability.h
    data/ticker/valuation.cpp
    data/ticker/valuation.h
    data/ticker/dividend.cpp
    data/ticker/dividend.h
    data/ticker/identity.cpp
    data/ticker/identity.h
    data/ticker/profitability.cpp
    data/ticker/profitability.h
    data/currency.cpp
    data/currency.h
    data/currency/data.cpp
    data/currency/data.h
    data/currency/exchange.cpp
    data/currency/exchange.h
    data/currency/name.cpp
    data/currency/name.h

    settings/network.cpp
    settings/network.h

    utilities/define.h
    utilities/res_dir.cpp
    utilities/res_dir.h
)

qt_add_executable(stock_manager ${src})
target_link_libraries (stock_manager PUBLIC ${Qt_libs})

qt_add_qml_module (stock_manager
    URI Stock
    VERSION 0.1
    NAMESPACE ui
    SOURCES
    QML_FILES
        main.qml
    RESOURCES
)
