cmake_minimum_required(VERSION 3.24)
project(client)

set(project_name "Client")

set (QT_QML_GENERATE_QMLLS_INI ON)

# app -------------------------------------------------------------------------
set (client_cpp
    chart/quote.cpp
    chart/quote.h

    model/search_tag.h
    model/search_tag.cpp
    model/asset_list.cpp
    model/asset_list.h
    model/ticker_list.cpp
    model/ticker_list.h
)

set (client_qml
    client.h client.cpp
)

set (client_src
    ${client_cpp} ${client_qml}
)

qt_add_executable(client main.cpp ${client_cpp})
target_link_libraries (client PUBLIC ${Qt_libs} sdk)

if (ANDROID)
    include(${CMAKE_CURRRENT_SOURCE_DIR}/android_openssl/android_openssl.cmake)

    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    add_android_openssl_libraries(client)
endif()

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_qml_module (client
    URI Stock
    VERSION 1.0
    SOURCES
        ${client_qml}
    QML_FILES
        main.qml
        SplashScreen.qml
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
