// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigDeleteMembers_utest.h"

int samconfTestSamconfConfigDeleteMembersExtErrConfigDeleteSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigDeleteMembersExtErrConfigDeleteTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigDeleteMembersExtErrConfigDelete(UNUSED void **state) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    samconfConfig_t *testConfig =
        samconfTestSamconfConfigDeleteMembersConfigInit(CHILD_COUNT, SAMCONF_CONFIG_VALUE_STRING);

    MOCK_FUNC_ALWAYS(samconfConfigDelete);
    expect_value(samconfConfigDelete, config, testConfig->children[0]);
    will_return(samconfConfigDelete, SAMCONF_CONFIG_ERROR);

    expect_value(samconfConfigDelete, config, testConfig->children[1]);
    will_return(samconfConfigDelete, SAMCONF_CONFIG_OK);

    status = samconfConfigDeleteMembers(testConfig);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);

    MOCK_FUNC_DISABLE(samconfConfigDelete);
    free(testConfig);
}
