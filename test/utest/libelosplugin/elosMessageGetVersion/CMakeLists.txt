# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_libelos_elosMessageGetVersion_utest
  SOURCES
    case_exterr_new_object.c
    case_exterr_add_new_string.c
    case_success.c
    elosMessageGetVersion_utest.c
  LIBRARIES
    mock_libelosplugin_static
    libelosplugin_static_weak
    libelosplugin_interface
    elos_event_static_weak
    version
    safu::mock_safu
)
