# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosClientAuthorizationIsTrustedConnection_utest
  SOURCES
  #case_err_param.c
  #  case_exterr_mnl_socket_close.c
    case_success.c
    elosClientAuthorizationIsTrustedConnection_utest.c
  LIBRARIES
  tcp_clientauthorization_static
    libelosplugin_static_weak
    cmocka_mocks::mock_libmnl
)
