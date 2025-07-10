# SPDX-License-Identifier: MIT

project(punisher_unit_tests LANGUAGES CXX)

find_package(GTest REQUIRED)
find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    punisher_unit_tests
  SOURCES
    test_punisher.cpp
  LIBRARIES
    client_punisher
    safu::safu
    GTest::gtest_main
)
