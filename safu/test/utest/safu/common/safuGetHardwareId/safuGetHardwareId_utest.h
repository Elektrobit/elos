// SPDX-License-Identifier: MIT

#ifndef SAFU_GET_HARDWAREID_UTEST_H
#define SAFU_GET_HARDWAREID_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "safu/common.h"

TEST_CASE_FUNC_PROTOTYPES(safuTestSafuGetHardwareIdSuccess)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuGetHardwareIdErrFile)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuGetHardwareIdEnv)

MOCK_FUNC_VAR_EXTERN(safuReadFileToString);

#define HARDWAREID_CACHE SAFU_HARDWAREID_ENVIRONMENT_PREFIX "CACHE"
#define HARDWAREID_FILE  SAFU_HARDWAREID_ENVIRONMENT_PREFIX "FILE"

#endif /* SAFU_GET_HARDWAREID_UTEST_H */
