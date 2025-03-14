# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosUnixClientAuthorizationDelete_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosUnixClientAuthorizationDelete_utest.c
  LIBRARIES
    mock_libelosplugin_static
    libelosplugin_static_weak
    unix_clientauthorization_static
)
