// SPDX-License-Identifier: MIT

#include "safuLogLevelToString_utest.h"

struct _testData {
    safuLogLevelE_t level;
    char *name;
};

void safuTestSafuLogLevelToStringSuccess(UNUSED void **state) {
    TEST("safuLogLevelToString");
    SHOULD("%s", "successfully get log level name string from enum variants");

    struct _testData testData[] = {
        {.level = SAFU_LOG_LEVEL_NONE, .name = "NONE"},   {.level = SAFU_LOG_LEVEL_CRIT, .name = "CRITICAL"},
        {.level = SAFU_LOG_LEVEL_ERR, .name = "ERROR"},   {.level = SAFU_LOG_LEVEL_WARN, .name = "WARNING"},
        {.level = SAFU_LOG_LEVEL_INFO, .name = "INFO"},   {.level = SAFU_LOG_LEVEL_DEBUG, .name = "DEBUG"},
        {.level = SAFU_LOG_LEVEL_ALL, .name = "VERBOSE"},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testData); i++) {
        assert_string_equal(testData[i].name, safuLogLevelToString(testData[i].level));
    }
}
