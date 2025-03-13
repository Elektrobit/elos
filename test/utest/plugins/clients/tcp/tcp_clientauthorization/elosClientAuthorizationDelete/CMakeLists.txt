# SPDX-License-Identifier: MIT
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_clientAuthorization_elosClientAuthorizationDelete_utest
  SOURCES
    case_err_param.c
    case_exterr_mnl_socket_close.c
    case_success.c
    case_success_socket_is_null.c
    elosClientAuthorizationDelete_utest.c
  LIBRARIES
    libelosplugin_static_weak
    tcp_clientauthorization_static
    cmocka_mocks::mock_libmnl
)
