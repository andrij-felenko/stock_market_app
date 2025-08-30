cmake_minimum_required(VERSION 3.24)
project(stock_manager)

set(project_name "Stock manager")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)

# destinations folders
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/bin/app)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/bin/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../cpp/bin/archive)

find_package (Qt6 REQUIRED COMPONENTS
    Core Network WebSockets
    Gui Widgets Svg
    Qml QuickControls2 Quick
    Test
)

set (Qt_lib_core  Qt6::Core   Qt6::Network         Qt6::WebSockets)
set (Qt_lib_gui   Qt6::Gui    Qt6::Widgets         Qt6::Svg)
set (Qt_lib_quick Qt6::Qml    Qt6::QuickControls2  Qt6::Quick)
set (Qt_libs ${Qt_lib_core} ${Qt_lib_gui} ${Qt_lib_quick})

add_subdirectory(client)
add_subdirectory(sdk)
# add_subdirectory(server)


# # ---------------------------- Unit test project ----------------------------------------
# set (test_src
#     test.cpp
#     data/instrument_test.h
#     data/instrument/ticker_test.h
# )

# add_executable (test_app ${test_src})

# set_target_properties(test_app PROPERTIES PROJECT_NAME "Tests")
# target_include_directories(test_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(test_app PUBLIC ${Qt_libs} Qt6::Test)
# target_sources(test_app PRIVATE ${src})

# if (TARGET test_app)
#     source_group("Tests" FILES ${test_src})
# endif ()

# add_test(NAME test_app COMMAND test_app)
# set_target_properties(test_app PROPERTIES FOLDER "Tests")

# enable_testing()
# # =======================================================================================
