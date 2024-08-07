# SPDX-License-Identifier: MIT
find_package(cmocka_mocks REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosClientAuthorizationInitialize_utest
  SOURCES
    case_err_param.c
    case_exterr_mnl_socket_bind.c
    case_exterr_mnl_socket_open.c
    case_success.c
    elosClientAuthorizationInitialize_utest.c
  LIBRARIES
    connectionmanager_static_weak
    cmocka_mocks::mock_libmnl
)
