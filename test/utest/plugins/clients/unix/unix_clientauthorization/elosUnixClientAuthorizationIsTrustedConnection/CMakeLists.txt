# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.55.7 REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosUnixClientAuthorizationIsTrustedConnection_utest
  SOURCES
    case_success.c
    elosUnixClientAuthorizationIsTrustedConnection_utest.c
  LIBRARIES
    unix_clientauthorization_static
    mock_libelosplugin_static
    libelosplugin_static_weak
    cmocka_mocks::mock_libc
)
