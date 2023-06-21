// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGet_utest.h"

int samconfTestSamconfConfigGetSuccessEntryNotFoundSetup(UNUSED void **state) {
    samconfConfigGetUtestInit(state);
    return 0;
}

int samconfTestSamconfConfigGetSuccessEntryNotFoundTeardown(UNUSED void **state) {
    samconfConfigGetUtestCleanup(state);
    return 0;
}

static void _testSamconfconfigGetSuccessEntryNotFoundParam(const samconfConfig_t *root, const char *path,
                                                           struct timespec *sum) {
    const samconfConfig_t *result = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    struct timespec start = {0};
    struct timespec stop = {0};

    PARAM("%s", path);
    for (int i = 0; i < 1000; i++) {
        clock_gettime(CLOCK_REALTIME, &start);
        status = samconfConfigGet(root, path, &result);
        clock_gettime(CLOCK_REALTIME, &stop);
        sum->tv_sec += (stop.tv_sec - start.tv_sec);
        sum->tv_nsec += (stop.tv_nsec - start.tv_nsec);
        assert_int_equal(status, SAMCONF_CONFIG_NOT_FOUND);
        assert_null(result);
    }
}

void samconfTestSamconfConfigGetSuccessEntryNotFound(void **state) {
    samconfUteststate_t *testState = *state;
    const char *testPaths[] = {"/",
                               "",
                               "elos/Scanner/Syslog",
                               "elos//Scanner/Syslog",
                               "Scanner/Syslog/SyslogPath/NotExisting",
                               "/Scanner/Syslog/SyslogPath/NotExisting",
                               "/Scanner/Syslog/SyslogPath/NotExisting/",
                               "Scanner/Syslog/SyslogPath/NotExisting/",
                               "Scanner/Syslog/NotExisting",
                               "NotExisting",
                               "/NotExisting",
                               "/NotExisting/",
                               "NotExisting/"};
    struct timespec sum = {0};

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

    samconfConfig_t root = {
        .parent = NULL,
        .key = "elos",
        .type = SAMCONF_CONFIG_VALUE_INT,
        .value.integer = 36,
        .children = elosChildren,
        .childCount = 3,
    };

    TEST("samconfConfigGet");
    SHOULD("%s", "return SAMCONF_CONFIG_NOT_FOUND since for the specified path no config entry exists");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        _testSamconfconfigGetSuccessEntryNotFoundParam(&root, testPaths[h], &sum);
    }

    print_message("Average Test run  time : %lu s: %lu ns\n", sum.tv_sec / 1000, sum.tv_nsec / 1000);
}
