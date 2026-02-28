cmake_minimum_required(VERSION 3.16)
project(vmti LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(vmti
    src/vmtibuilder.cpp
)

target_include_directories(vmti
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()

add_executable(vmti_tests
    tests/test_main.cpp
)

target_link_libraries(vmti_tests
    PRIVATE
        vmti
)

add_test(NAME vmti_tests COMMAND vmti_tests)
