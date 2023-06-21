# SPDX-License-Identifier: MIT
find_package(safu)
find_package(cmocka_mocks)

create_unit_test(
  NAME
  test_jsonBackend_elosCompleteStoragePath_utest
  SOURCES
    case_success.c
    elosCompleteStoragePath_utest.c
  LIBRARIES
    backend_json_static
    safu::mock_safu
)
