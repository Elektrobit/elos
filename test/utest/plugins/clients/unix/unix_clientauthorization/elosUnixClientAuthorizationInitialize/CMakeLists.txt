# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosUnixClientAuthorizationInitialize_utest
  SOURCES
    case_err_param.c
    case_success.c
    elosUnixClientAuthorizationInitialize_utest.c
  LIBRARIES
    libelosplugin_static_weak
    unix_clientauthorization_static
)
