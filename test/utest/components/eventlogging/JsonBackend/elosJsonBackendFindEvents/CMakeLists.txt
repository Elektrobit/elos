# SPDX-License-Identifier: MIT
find_package(cmocka_mocks)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
  test_jsonBackend_elosJsonBackendFindEvents_utest
  SOURCES
    case_success.c
    elosJsonBackendFindEvents_utest.c
  LIBRARIES
    backend_json_static
    cmocka_mocks::mock_libc
    safu::mock_safu
)
