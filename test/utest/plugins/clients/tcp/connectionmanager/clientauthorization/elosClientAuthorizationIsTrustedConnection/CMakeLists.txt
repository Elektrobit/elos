# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosClientAuthorizationIsTrustedConnection_utest
  SOURCES
  #case_err_param.c
  #  case_exterr_mnl_socket_close.c
    case_success.c
    elosClientAuthorizationIsTrustedConnection_utest.c
  LIBRARIES
    connectionmanager_static_weak
    cmocka_mocks::mock_libmnl
)
