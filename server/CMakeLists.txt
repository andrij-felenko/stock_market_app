cmake_minimum_required(VERSION 3.24)
project(server)

set (src
    nexus.cpp
    nexus.h
    market.cpp
    market.h
    roster.cpp
    roster.h
    session.cpp
    session.h
)

qt_add_executable(server main.cpp ${src})
target_link_libraries (server PUBLIC sdk)
