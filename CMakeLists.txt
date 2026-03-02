cmake_minimum_required(VERSION 3.16)
project(vmti LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(vmti
    src/main.cpp
)

target_include_directories(vmti
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()

find_package(GTest REQUIRED)

add_executable(vmti_tests
    tests/test_main.cpp
)

target_include_directories(vmti_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(vmti_tests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(vmti_tests)
