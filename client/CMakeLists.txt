set(execName net-pixeld-client)

include(${PROJECT_SOURCE_DIR}/vendors/raylib.cmake)
include(${PROJECT_SOURCE_DIR}/vendors/raylib_cpp.cmake)

add_executable(${execName}
    src/main.cpp
    src/Application.cpp
)

target_include_directories(${execName}
    PRIVATE
    inc
)

target_link_libraries(${execName} 
    PRIVATE
    raylib
    raylib_cpp
)

install(
    TARGETS ${execName}
    RUNTIME DESTINATION bin
)