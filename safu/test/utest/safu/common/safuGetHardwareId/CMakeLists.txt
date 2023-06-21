# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
  test_common_safuGetHardwareId_utest
  SOURCES
    case_env.c
    case_err_file.c
    case_success.c
    safuGetHardwareId_utest.c
    safuGetHardwareId_utils.c
  LIBRARIES
    safu_weak
    safu
)
