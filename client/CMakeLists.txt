set(execName net-pixeld-client)

include(${PROJECT_SOURCE_DIR}/vendors/raylib.cmake)
include(${PROJECT_SOURCE_DIR}/vendors/raylib_cpp.cmake)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(${execName}
    src/main.cpp
    src/Application.cpp
    src/NetworkClient.cpp
    src/SceneManager.cpp
    src/GameScene.cpp
    src/LoginScene.cpp
    src/Player.cpp
)

target_include_directories(${execName}
    PRIVATE
    inc
)

target_link_libraries(${execName} 
    PRIVATE
    common_lib
    raylib
    raylib_cpp
    Boost::system
)

install(
    TARGETS ${execName}
    RUNTIME DESTINATION bin
)