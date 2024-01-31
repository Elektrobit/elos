# SPDX-License-Identifier: MIT
find_package(safu 0.50.1 REQUIRED)
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_jsonBackend_elosJsonBackendPersist_utest
  SOURCES
    case_exterr_eventSerialize.c
    case_exterr_writeExactly.c
    case_success.c
    elosJsonBackendPersist_utest.c
  LIBRARIES
    backend_json_static
    mock_elos_event_static
    safu::mock_safu
    cmocka_mocks::mock_libc
)
