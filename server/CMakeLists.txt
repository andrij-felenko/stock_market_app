cmake_minimum_required(VERSION 3.24)
project(server)

# app -------------------------------------------------------------------------
set (src
    nexus.h nexus.cpp
)

qt_add_executable(server main.cpp ${src})
target_link_libraries (server PUBLIC sdk)

# qt_add_resources(server "rc_images"
#     PREFIX "/"
#     FILES
#         # rc/images/presentation.png
#         # rc/images/website.png
#         # rc/images/briefcase.png
#         # rc/images/folder.png
#         # rc/images/browser.png
#         # rc/images/bull-market.png
#         # rc/images/star.png
#         # rc/images/star_black.png
#         # rc/images/star_black_border.png
#         # rc/images/star_empty.png
#         # rc/images/stock.png
#         # rc/images/Stock_app_logo.png
#         # rc/images/Stock_app_logo2.png
#         # rc/images/person.png
# )

# file(GLOB tickers_meta_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} rc/tickers_meta/*.edsm)
# qt_add_resources(server "rc_tickers_meta"
#     PREFIX "/"
#     FILES
#         ${tickers_meta_files}
# )

# file(GLOB stock_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} rc/stocks/*.tdsm)
# qt_add_resources(server "rc_stocks"
#     PREFIX "/"
#     FILES
#         ${stock_files}
# )


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
