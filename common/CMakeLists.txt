set(libName common_lib)

add_library(${libName} STATIC dummy.cpp)

target_include_directories(${libName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})