// SPDX-License-Identifier: MIT
#ifndef SAMCONF_CONFIGGETREAL_UTEST_H
#define SAMCONF_CONFIGGETREAL_UTEST_H

#include <cmocka_extensions/mock_extensions.h>

#include "samconf_utest.h"

typedef struct {
    samconfConfig_t *childrenData;
} samconfUteststateT_t;

int samconfConfigGetRealUtestInit(void **state);
int samconfConfigGetRealUtestCleanUp(void **state);

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealSuccessEntryFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealSuccessEntryNotFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealSuccessNodeNotReal)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealExtErrConfigGet)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealErrorRootNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealErrorPathNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealErrorResultNull)
#endif /* SAMCONF_CONFIGGETREAL_UTEST_H */
