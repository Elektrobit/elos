// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigDeleteMembers_utest.h"

int samconfTestSamconfConfigDeleteMembersNullConfigurationSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigDeleteMembersNullConfigurationTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigDeleteMembersNullConfiguration(UNUSED void **state) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    status = samconfConfigDeleteMembers(NULL);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
