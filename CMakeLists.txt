cmake_minimum_required(VERSION 3.10)
project(net-pixeld)

set(MODE "client" CACHE STRING "Build mode: client or server")
set_property(CACHE MODE PROPERTY STRINGS client server)
message(STATUS "Configuring build in ${MODE} mode")

set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/.install" CACHE PATH "Installation directory" FORCE)
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MODE STREQUAL "client")
    add_subdirectory(client)
elseif(MODE STREQUAL "server")
    add_subdirectory(server)
else()
    message(FATAL_ERROR "Unknown MODE: ${MODE}. Please specify -DMODE=client or -DMODE=server\nDefault: MODE = client")
endif()
