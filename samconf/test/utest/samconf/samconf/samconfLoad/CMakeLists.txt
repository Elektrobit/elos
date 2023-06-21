# SPDX-License-Identifier: MIT
find_package(cmocka_extensions REQUIRED)

create_unit_test(
  NAME
    test_event_samconfLoad_utest
  SOURCES
    case_success.c
    case_openerr.c
    case_loaderr.c
    case_closeerr.c
    case_lookuperr.c
    case_backenddeleteerr.c
    case_configdeleteerr.c
    samconfLoad_utest.c
  LIBRARIES
    cmocka_extensions::cmocka_extensions
    samconf_weak
    mock_samconf_static
)
