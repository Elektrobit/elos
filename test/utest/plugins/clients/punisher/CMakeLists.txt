# SPDX-License-Identifier: MIT

find_package(GTest REQUIRED)

add_executable(punisher_unit_tests
  test_punisher.cpp
)

target_link_libraries(
  punisher_unit_tests
  PRIVATE
    client_punisher
)

target_link_libraries(punisher_unit_tests
  PRIVATE
    GTest::gtest_main
    elos::libelosplugin
    elos::libelos
    samconf::samconf
    safu::safu
    yaml-cpp
)

