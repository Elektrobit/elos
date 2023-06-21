// SPDX-License-Identifier: MIT

#include "safuLogLevelFromString_utest.h"

struct _testData {
    safuLogLevelE_t level;
    char *name;
};

void safuTestSafuLogLevelFromStringSuccess(UNUSED void **state) {
    TEST("safuLogLevelFromString");
    SHOULD("%s", "successfully get log level enum variants from valid name strings");

    struct _testData testData[] = {
        {.level = SAFU_LOG_LEVEL_NONE, .name = "NONE"},     {.level = SAFU_LOG_LEVEL_NONE, .name = "none"},
        {.level = SAFU_LOG_LEVEL_NONE, .name = "None"},     {.level = SAFU_LOG_LEVEL_CRIT, .name = "CRITICAL"},
        {.level = SAFU_LOG_LEVEL_CRIT, .name = "critical"}, {.level = SAFU_LOG_LEVEL_CRIT, .name = "Critical"},
        {.level = SAFU_LOG_LEVEL_ERR, .name = "ERROR"},     {.level = SAFU_LOG_LEVEL_ERR, .name = "error"},
        {.level = SAFU_LOG_LEVEL_ERR, .name = "Error"},     {.level = SAFU_LOG_LEVEL_WARN, .name = "WARNING"},
        {.level = SAFU_LOG_LEVEL_WARN, .name = "warning"},  {.level = SAFU_LOG_LEVEL_WARN, .name = "Warning"},
        {.level = SAFU_LOG_LEVEL_INFO, .name = "INFO"},     {.level = SAFU_LOG_LEVEL_INFO, .name = "info"},
        {.level = SAFU_LOG_LEVEL_INFO, .name = "Info"},     {.level = SAFU_LOG_LEVEL_DEBUG, .name = "DEBUG"},
        {.level = SAFU_LOG_LEVEL_DEBUG, .name = "debug"},   {.level = SAFU_LOG_LEVEL_DEBUG, .name = "Debug"},
        {.level = SAFU_LOG_LEVEL_ALL, .name = "VERBOSE"},   {.level = SAFU_LOG_LEVEL_ALL, .name = "verbose"},
        {.level = SAFU_LOG_LEVEL_ALL, .name = "Verbose"}};

    for (size_t i = 0; i < ARRAY_SIZE(testData); i++) {
        assert_int_equal(testData[i].level, safuLogLevelFromString(testData[i].name));
    }
}
