// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "samconfConfigAdd_utest.h"

int samconfTestSamconfConfigAddSuccessChildAddedSetup(void **state) {
    samconfConfigAddUtestInit(state);
    return 0;
}

int samconfTestSamconfConfigAddSuccessChildAddedTeardown(void **state) {
    samconfConfigAddUtestCleanUp(state);
    return 0;
}

void samconfTestSamconfConfigAddSuccessChildAdded(void **state) {
    samconfUteststate_t *testState = *state;
    samconfConfig_t *parent = NULL;
    samconfConfig_t *child = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    status = samconfConfigNew(&parent);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    status = samconfConfigNew(&child);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    samconfConfig_t *syslogChildren[] = {
        &testState->childrenData[0],
        &testState->childrenData[1],
    };

    testState->childrenData[2].children = syslogChildren;
    testState->childrenData[2].childCount = 2;

    samconfConfig_t *scannerChildren[] = {
        &testState->childrenData[2],
    };

    testState->childrenData[4].children = scannerChildren;
    testState->childrenData[4].childCount = 1;

    samconfConfig_t *elosChildren[] = {
        &testState->childrenData[3],
        &testState->childrenData[4],
        &testState->childrenData[5],
    };

    parent->key = "parent";
    parent->type = SAMCONF_CONFIG_VALUE_STRING;

    parent->children = malloc(ARRAY_SIZE(elosChildren) * sizeof(samconfConfig_t *));
    assert_non_null(parent->children);

    memcpy(parent->children, elosChildren, ARRAY_SIZE(elosChildren) * sizeof(samconfConfig_t *));

    parent->childCount = 3;

    child->key = "child";
    child->type = SAMCONF_CONFIG_VALUE_NONE;
    child->childCount = 0;

    TEST("samconfConfigAdd");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since child is added to parent");

    status = samconfConfigAdd(parent, child);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_int_equal(parent->childCount, 4);
    assert_string_equal(child->parent->key, "parent");
    assert_string_equal(parent->children[3]->key, "child");

    free(parent->children);
    free(parent);
    free(child);
}
