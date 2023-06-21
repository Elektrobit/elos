// SPDX-License-Identifier: MIT

#ifndef SAFU_GET_ENV_OR_UTEST_H
#define SAFU_GET_ENV_OR_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/common.h"

TEST_CASE_FUNC_PROTOTYPES(safuTestSafuGetEnvOrErrKeyIsNull)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuGetEnvOrSuccessKeyExists)
TEST_CASE_FUNC_PROTOTYPES(safuTestSafuGetEnvOrSuccessKeyNotExists)

#endif /* SAFU_GET_ENV_OR_UTEST_H */
