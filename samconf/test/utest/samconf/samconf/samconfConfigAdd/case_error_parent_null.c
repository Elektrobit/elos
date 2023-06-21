// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <time.h>

#include "samconfConfigAdd_utest.h"

int samconfTestSamconfConfigAddErrorParentNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigAddErrorParentNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigAddErrorParentNull(UNUSED void **state) {
    samconfConfig_t *child = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    status = samconfConfigNew(&child);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    child->key = "child";
    child->type = SAMCONF_CONFIG_VALUE_NONE;
    child->childCount = 0;

    TEST("samconfConfigAdd");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since parent is null");

    status = samconfConfigAdd(NULL, child);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);

    free(child);
}
