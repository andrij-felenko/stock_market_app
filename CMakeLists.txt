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
    api/finnhub.cpp
    api/finnhub.h
    # api/tradernet.cpp
    # api/tradernet.h
    # api/ibkr.cpp
    # api/ibkr.h
    api/openai.cpp
    api/openai.h
    # api/yahoo.cpp
    # api/yahoo.h
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
    data/ticker.cpp
    data/ticker.h
    data/stock.cpp
    data/stock.h
    data/portfolio.cpp
    data/portfolio.h
    data/transaction.cpp
    data/transaction.h
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

    model/search_tag.h
    model/search_tag.cpp

    settings/network.cpp
    settings/network.h

    utilities/define.h
    utilities/res_dir.cpp
    utilities/res_dir.h
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
)

qt_add_resources(stock_manager "rc_tags"
    PREFIX "/"
    FILES
        rc/tags/bond.edsm
        rc/tags/common_stock.edsm
        rc/tags/etc.edsm
        rc/tags/etf.edsm
        rc/tags/fund.edsm
        rc/tags/index.edsm
        rc/tags/mutual_fund.edsm
        rc/tags/notes.edsm
        rc/tags/preferred_stock.edsm
        rc/tags/unit.edsm
)

qt_add_resources(stock_manager "rc_stocks"
    PREFIX "/"
    FILES
        rc/stocks/C.US.tdsm
        rc/stocks/CHCT.US.tdsm
        rc/stocks/CMCL.US.tdsm
        rc/stocks/COLO-B.CO.tdsm
        rc/stocks/CVX.US.tdsm
        rc/stocks/DELL.US.tdsm
        rc/stocks/DOW.US.tdsm
        rc/stocks/EBR.B.US.tdsm
        rc/stocks/ET.US.tdsm
        rc/stocks/FMC.US.tdsm
        rc/stocks/gtn.us.tdsm
        rc/stocks/GXI.DE.tdsm
        rc/stocks/HPQ.US.tdsm
        rc/stocks/IBM.US.tdsm
        rc/stocks/INTC.US.tdsm
        rc/stocks/LMT.US.tdsm
        rc/stocks/LNVGY.US.tdsm
        rc/stocks/MA.US.tdsm
        rc/stocks/MBG.DEX.tdsm
        rc/stocks/MCD.US.tdsm
        rc/stocks/MCHP.US.tdsm
        rc/stocks/MS.US.tdsm
        rc/stocks/NHY.OL.tdsm
        rc/stocks/NOC.US.tdsm
        rc/stocks/NTOIF.US.tdsm
        rc/stocks/O.tdsm
        rc/stocks/O.US.tdsm
        rc/stocks/OGN.US.tdsm
        rc/stocks/OLED.US.tdsm
        rc/stocks/PAA.US.tdsm
        rc/stocks/PG.US.tdsm
        rc/stocks/PINE.US.tdsm
        rc/stocks/POLN.EU.tdsm
        rc/stocks/QCOM.US.tdsm
        rc/stocks/RHM.DE.tdsm
        rc/stocks/RIO1.DE.tdsm
        rc/stocks/RITM.US.tdsm
        rc/stocks/RNECY.US.tdsm
        rc/stocks/RNO.PA.tdsm
        rc/stocks/RPG.US.tdsm
        rc/stocks/RUI.PA.tdsm
        rc/stocks/SAN.US.tdsm
        rc/stocks/SRG.US.tdsm
        rc/stocks/TROX.US.tdsm
        rc/stocks/TSQ.US.tdsm
        rc/stocks/ULVR.L.tdsm
        rc/stocks/V.US.tdsm
        rc/stocks/VOW3.BE.tdsm
        rc/stocks/VOW3.DE.tdsm
        rc/stocks/WDS.US.tdsm
        rc/stocks/WPC.US.tdsm
        rc/stocks/XRX.US.tdsm
        rc/stocks/0JOP.L.tdsm
        rc/stocks/0NBX.L.tdsm
        rc/stocks/AAPL.US.tdsm
        rc/stocks/AES.US.tdsm
        rc/stocks/AGNC.US.tdsm
        rc/stocks/AM.US.tdsm
        rc/stocks/ANDE.US.tdsm
        rc/stocks/APAM.MC.tdsm
        rc/stocks/BBZA.DE.tdsm
        rc/stocks/BWMX.US.tdsm
)
