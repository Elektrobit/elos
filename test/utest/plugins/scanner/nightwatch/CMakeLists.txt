# SPDX-License-Identifier: MIT

project(nightwatch_unit_tests LANGUAGES CXX)

find_package(GTest REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    nightwatch_unit_tests
  SOURCES
    test_application.cpp
    test_monitoringunit.cpp
    test_systemmonitoring.cpp
  INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
  LIBRARIES
    scanner_nightwatch
    safu::safu
    GTest::gtest_main
    yaml-cpp
)
