cmake_minimum_required(VERSION 3.16)
project(vmti LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(vmti
    src/main.cpp
    src/vmtibuilder.cpp
)

target_include_directories(vmti
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()

add_executable(vmti_tests
    src/vmtibuilder.cpp
    tests/test_main.cpp
)

target_include_directories(vmti_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(NAME vmti_tests COMMAND vmti_tests)
