add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE inc)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)