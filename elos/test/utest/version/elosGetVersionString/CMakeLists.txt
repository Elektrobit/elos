# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_version_elosGetVersionString_utest
  SOURCES
    elosGetVersionString_utest.c
    case_function_call.c
  LIBRARIES
    version
)
