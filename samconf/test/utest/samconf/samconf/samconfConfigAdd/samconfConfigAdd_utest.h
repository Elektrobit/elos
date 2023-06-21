// SPDX-License-Identifier: MIT
#ifndef SAMCONF_CONFIGADD_UTEST_H
#define SAMCONF_CONFIGADD_UTEST_H

#include <cmocka_extensions/mock_extensions.h>

#include "samconf_utest.h"

typedef struct {
    samconfConfig_t *childrenData;
} samconfUteststate_t;

int samconfConfigAddUtestInit(void **state);
int samconfConfigAddUtestCleanUp(void **state);

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigAddSuccessChildAdded)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigAddSuccessEmptyNode)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigAddErrorParentNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigAddErrorChildNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigAddAllocMemError)

#endif /* SAMCONF_CONFIGADD_UTEST_H */
