// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "cmocka_mocks/mock_libc.h"
#include "samconfConfigGet_utest.h"

int samconfTestSamconfConfigGetExtErrorStrdupSetup(UNUSED void **state) {
    samconfConfigGetUtestInit(state);
    return 0;
}

int samconfTestSamconfConfigGetExtErrorStrdupTeardown(UNUSED void **state) {
    samconfConfigGetUtestCleanup(state);
    return 0;
}

void samconfTestSamconfConfigGetExtErrorStrdup(void **state) {
    samconfUteststate_t *testState = *state;
    const samconfConfig_t *result = NULL;
    const char *testPath = "Scanner/Syslog/SyslogPath";
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

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_string(__wrap_strdup, string, testPath);
    will_return(__wrap_strdup, NULL);

    TEST("samconfConfigGet");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since strdup return error");

    status = samconfConfigGet(&root, testPath, &result);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_null(result);
}
