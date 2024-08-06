# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.50.1 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosEventPublish_utest
  SOURCES
    case_err_new_object.c
    case_err_convert.c
    case_err_communication.c
    case_err_param.c
    case_success.c
    elosEventPublish_utest.c
  LIBRARIES
    mock_libelos_static
    mock_elos_event_static
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_jsonc
    safu::mock_safu
    libelos_static_weak
)
