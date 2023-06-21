# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_event_elosEventInitialize_utest
  SOURCES
    case_err_event.c
    case_success.c
    elosEventInitialize_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_common_static
)
