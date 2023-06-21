# SPDX-License-Identifier: MIT
find_package(cmocka_extensions REQUIRED)

create_unit_test(
  NAME
    test_event_samconfLookupBackend_utest
  SOURCES
    case_success.c
    case_error_no_loop.c
    case_error_support_fail.c
    case_error_backend_alloc.c
    samconfLookupBackend_utest.c
  LIBRARIES
    cmocka_extensions::cmocka_extensions
    samconf_weak
    mock_samconf_static
)
