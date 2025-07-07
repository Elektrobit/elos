# SPDX-License-Identifier: MIT

find_package(GTest REQUIRED)

add_executable(nightwatch_unit_tests
  test_application.cpp
  test_monitoringunit.cpp
  test_systemmonitoring.cpp
)

target_link_libraries(
  nightwatch_unit_tests
  PRIVATE
    scanner_nightwatch
)

target_include_directories(
  nightwatch_unit_tests
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(nightwatch_unit_tests
  PRIVATE
    GTest::gtest_main
    elos::libelosplugin
    elos::libelos
    samconf::samconf
    safu::safu
    yaml-cpp
)

