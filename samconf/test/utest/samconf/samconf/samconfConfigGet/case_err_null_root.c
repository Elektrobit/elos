// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGet_utest.h"

int samconfTestSamconfConfigGetErrorNullRootSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetErrorNullRootTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetErrorNullRoot(UNUSED void **state) {
    const samconfConfig_t *result = NULL;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfig_t *root = NULL;

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    struct timespec start = {0};
    struct timespec stop = {0};
    struct timespec sum = {0};

    TEST("samconfConfigGet");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since root is null");

    for (int i = 0; i < 1000; i++) {
        clock_gettime(CLOCK_REALTIME, &start);
        status = samconfConfigGet(root, testPath, &result);
        clock_gettime(CLOCK_REALTIME, &stop);
        sum.tv_sec += (stop.tv_sec - start.tv_sec);
        sum.tv_nsec += (stop.tv_nsec - start.tv_nsec);
        assert_int_equal(status, SAMCONF_CONFIG_ERROR);
        assert_null(result);
    }

    print_message("Average Test run  time : %lu s: %lu ns\n", sum.tv_sec / 1000, sum.tv_nsec / 1000);
}
