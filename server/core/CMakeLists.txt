add_executable(${execName} src/main.cpp)

target_include_directories(${execName} PRIVATE inc)

install(
    TARGETS ${execName}
    RUNTIME DESTINATION bin
)