# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_event_elosEventClone_utest
  SOURCES
    case_err_event_source.c
    case_err_event_dest.c
    case_exterr_event_new.c
    case_exterr_event_deep_copy.c
    case_success.c
    elosEventClone_utest.c
  LIBRARIES
    elos_event_static_weak
    mock_elos_event_static
)
