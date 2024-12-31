project(finite_state_machine)

add_library(${PROJECT_NAME} src/FiniteStateMachine.c)
add_library(fsm::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
