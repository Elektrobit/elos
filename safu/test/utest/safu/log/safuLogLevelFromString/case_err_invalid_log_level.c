// SPDX-License-Identifier: MIT

#include "safuLogLevelFromString_utest.h"

void safuTestSafuLogLevelFromStringErrInvalidLogLevel(UNUSED void **state) {
    TEST("safuLogLevelFromString");
    SHOULD("%s", "return SAFU_LOG_LEVEL_INVALID on string not representing a valid log level");

    const char *testInput[] = {"SAFU_LOG_LEVEL_NONE",
                               "SAFU_LOG_LEVEL_CRIT",
                               "SAFU_LOG_LEVEL_ERR",
                               "SAFU_LOG_LEVEL_WARN",
                               "SAFU_LOG_LEVEL_INFO",
                               "SAFU_LOG_LEVEL_DEBUG",
                               "SAFU_LOG_LEVEL_COUNT",
                               "SAFU_LOG_LEVEL_ALL",
                               "SAFU_LOG_LEVEL_INVALID",
                               "COUNT",
                               "ALL",
                               "INVALID",
                               "SOMETHING_ELSE"};

    for (size_t i = 0; i < ARRAY_SIZE(testInput); i++) {
        assert_int_equal(SAFU_LOG_LEVEL_INVALID, safuLogLevelFromString(testInput[i]));
    }
}
