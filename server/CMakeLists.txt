set(execName net-pixeld-server)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(${execName}
    src/main.cpp
    src/Server.cpp
    src/Session.cpp
)

target_include_directories(${execName} PRIVATE inc)

target_link_libraries(${execName}
    PRIVATE
    common_lib
    Boost::system
)

install(
    TARGETS ${execName}
    RUNTIME DESTINATION bin
)