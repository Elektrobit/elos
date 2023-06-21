// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <time.h>

#include "samconfConfigGet_utest.h"

int samconfTestSamconfConfigGetSuccessEntryFoundSetup(void **state) {
    samconfConfigGetUtestInit(state);
    return 0;
}

int samconfTestSamconfConfigGetSuccessEntryFoundTeardown(void **state) {
    samconfConfigGetUtestCleanup(state);
    return 0;
}

static void _testSamconfconfigGetSuccessEntryFoundParam(const samconfConfig_t *root, const char *path, const char *key,
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
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_non_null(result);
        assert_string_equal(result->key, key);
    }
}

void samconfTestSamconfConfigGetSuccessEntryFound(void **state) {
    samconfUteststate_t *testState = *state;

    const char *testPaths[] = {
        "Scanner/Syslog/SyslogPath",  "Scanner/Syslog", "Scanner",   "logger",   "ip",
        "/Scanner/Syslog/SyslogPath", "Scanner/",       "/Scanner/", "/Scanner", "/Scanner/Syslog/SyslogPath/",
        "Scanner/Syslog/SyslogPath/"};

    const char *resultKey[] = {"SyslogPath", "Syslog",  "Scanner", "logger",     "ip",        "SyslogPath",
                               "Scanner",    "Scanner", "Scanner", "SyslogPath", "SyslogPath"};
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
    SHOULD("%s", "return SAMCONF_CONFIG_OK since all parameters are correct and node was found");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        _testSamconfconfigGetSuccessEntryFoundParam(&root, testPaths[h], resultKey[h], &sum);
    }

    print_message("Average Test run  time : %lu s: %lu ns\n", sum.tv_sec / 1000, sum.tv_nsec / 1000);
}
