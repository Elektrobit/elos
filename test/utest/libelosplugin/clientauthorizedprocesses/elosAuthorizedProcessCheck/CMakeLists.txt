# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(safu 0.56.2 REQUIRED)
find_package(samconf 0.56.3 REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosAuthorizedProcessCheck_utest
  SOURCES
    case_success.c
    case_err_authorizedprocess_param_null.c
    case_err_process_param_null.c
    case_success_no_match.c
    elosAuthorizedProcessCheck_utest.c
  LIBRARIES
    mock_libelosplugin_static
    libelosplugin_static_weak
    safu::mock_safu
    samconf::mock_samconf
    samconf::samconf_test_utils
)
