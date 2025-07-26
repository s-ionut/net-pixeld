set(execName net-pixeld-client)

include(${PROJECT_SOURCE_DIR}/vendors/raylib.cmake)
include(${PROJECT_SOURCE_DIR}/vendors/raylib_cpp.cmake)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(${execName}
    src/main.cpp
    src/font/CFont.cpp
    src/gui/Textbox.cpp
    src/app/Timer.cpp
    src/app/Application.cpp
    src/network/NetworkClient.cpp
    src/scenes/SceneManager.cpp
    src/scenes/GameScene.cpp
    src/scenes/LoginScene.cpp
    src/game/Player.cpp
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