// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetString_utest.h"

int samconfTestSamconfConfigGetStringSuccessEntryNotFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringSuccessEntryNotFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringSuccessEntryNotFound(void **state) {
    samconfUteststateT_t *testState = *state;
    const char *result = NULL;
    const char *testPaths[] = {"",
                               "elos/Scanner/Syslog",
                               "Scanner/Syslog/SyslogPath/NotExisting",
                               "Scanner/Syslog/NotExisting",
                               "NotExisting",
                               "NotExisting/"};
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

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

    TEST("samconfConfigGetString");
    SHOULD("%s", "return config not found since specified path does not have a valid config");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
        expect_value(samconfConfigGet, root, &root);
        expect_value(samconfConfigGet, path, testPaths[h]);
        expect_any(samconfConfigGet, result);

        will_return(samconfConfigGet, NULL);
        will_return(samconfConfigGet, SAMCONF_CONFIG_NOT_FOUND);

        status = samconfConfigGetString(&root, testPaths[h], &result);
        assert_int_equal(status, SAMCONF_CONFIG_NOT_FOUND);
        assert_null(result);
    }
}
