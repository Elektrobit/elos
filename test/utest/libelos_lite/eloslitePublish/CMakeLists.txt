# SPDX-License-Identifier: MIT

find_package(safu 0.56.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos-lite_eloslitePublish_utest
  SOURCES
    case_success_all_permutations.c
    eloslitePublish_utils.c
    eloslitePublish_utest.c
  LIBRARIES
    libelos-lite
    libelos_common
    safu::safu
)

target_link_options(
    test_libelos-lite_eloslitePublish_utest
    PUBLIC
    -Wl,--wrap=send
    -Wl,--wrap=recv
)
