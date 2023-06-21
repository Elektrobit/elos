// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigDeleteMembers_utest.h"

int samconfTestSamconfConfigDeleteMembersSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigDeleteMembersSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    samconfConfig_t *testConfig =
        samconfTestSamconfConfigDeleteMembersConfigInit(CHILD_COUNT, SAMCONF_CONFIG_VALUE_STRING);

    for (size_t i = 0; i < CHILD_COUNT; i++) {
        testConfig->children[i] = samconfTestSamconfConfigDeleteMembersConfigInit(0, SAMCONF_CONFIG_VALUE_INT);
    }

    status = samconfConfigDeleteMembers(testConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    free(testConfig);
}
