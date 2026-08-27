# SPDX-License-Identifier: MIT

find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos-lite_eloslite_utest
  SOURCES
    case_success.c
    eloslite_utest.c
  LIBRARIES
    libelos-lite
    libelos_common
    safu::safu
)

target_link_options(
    test_libelos-lite_eloslite_utest
    PUBLIC
    -Wl,--wrap=eloslitePublish
)
