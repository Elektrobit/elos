// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGet_utest.h"

int samconfTestSamconfConfigGetSuccessRootChildCountZeroSetup(UNUSED void **state) {
    samconfConfigGetUtestInit(state);
    return 0;
}

int samconfTestSamconfConfigGetSuccessRootChildCountZeroTeardown(UNUSED void **state) {
    samconfConfigGetUtestCleanup(state);
    return 0;
}

void samconfTestSamconfConfigGetSuccessRootChildCountZero(UNUSED void **state) {
    samconfUteststate_t *testState = *state;
    const samconfConfig_t *result = NULL;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    struct timespec start = {0};
    struct timespec stop = {0};
    struct timespec sum = {0};

    samconfConfig_t *elosChildren[] = {
        &testState->childrenData[3],
        &testState->childrenData[4],
        &testState->childrenData[5],
    };

    samconfConfig_t root = {
        .parent = NULL,
        .key = "elos",
        .type = SAMCONF_CONFIG_VALUE_INT,
        .value.integer = 36,
        .children = elosChildren,
        .childCount = 0,
    };

    TEST("samconfConfigGet");
    SHOULD("%s", "return SAMCONF_CONFIG_NOT_FOUND since root child count is zero");

    for (int i = 0; i < 1000; i++) {
        clock_gettime(CLOCK_REALTIME, &start);
        status = samconfConfigGet(&root, testPath, &result);
        clock_gettime(CLOCK_REALTIME, &stop);
        sum.tv_sec += (stop.tv_sec - start.tv_sec);
        sum.tv_nsec += (stop.tv_nsec - start.tv_nsec);
        assert_int_equal(status, SAMCONF_CONFIG_NOT_FOUND);
        assert_null(result);
    }

    print_message("Average Test run  time : %lu s: %lu ns\n", sum.tv_sec / 1000, sum.tv_nsec / 1000);
}
