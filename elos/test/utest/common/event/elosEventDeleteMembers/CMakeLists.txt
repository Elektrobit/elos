# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_event_elosEventDeleteMembers_utest
  SOURCES
    case_success.c
    case_err_wrong_parameter.c
    elosEventDeleteMembers_utest.c
  LIBRARIES
    elos_event_static
)
