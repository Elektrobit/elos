// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <time.h>

#include "samconfConfigAdd_utest.h"

int samconfTestSamconfConfigAddSuccessEmptyNodeSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigAddSuccessEmptyNodeTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigAddSuccessEmptyNode(UNUSED void **state) {
    samconfConfig_t *parent = NULL;
    samconfConfig_t *child = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    status = samconfConfigNew(&parent);
    status = samconfConfigNew(&child);

    parent->key = "parent";
    child->key = "child";

    TEST("samconfConfigAdd");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since child added to parent with no children");

    status = samconfConfigAdd(parent, child);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_int_equal(parent->childCount, 1);
    assert_string_equal(child->parent->key, "parent");
    assert_string_equal(parent->children[0]->key, "child");

    free(parent->children);
    free(parent);
    free(child);
}
