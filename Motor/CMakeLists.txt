project(motor)

add_library(${PROJECT_NAME} src/Motor.c)
add_library(motor::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
