// SPDX-License-Identifier: MIT

#include <limits.h>

#include "safuLogGetStreamLevel_utest.h"

struct _testData {
    safuLogLevelE_t set;
    safuLogLevelE_t get;
};

void safuTestSafuLogGetStreamLevelSuccess(UNUSED void **state) {
    TEST("safuLogGetStreamLevel");
    SHOULD("%s", "successfully get log level enum variants from current log context");

    struct _testData testData[] = {{.set = SAFU_LOG_LEVEL_NONE, .get = SAFU_LOG_LEVEL_NONE},
                                   {.set = SAFU_LOG_LEVEL_CRIT, .get = SAFU_LOG_LEVEL_CRIT},
                                   {.set = SAFU_LOG_LEVEL_ERR, .get = SAFU_LOG_LEVEL_ERR},
                                   {.set = SAFU_LOG_LEVEL_WARN, .get = SAFU_LOG_LEVEL_WARN},
                                   {.set = SAFU_LOG_LEVEL_INFO, .get = SAFU_LOG_LEVEL_INFO},
                                   {.set = SAFU_LOG_LEVEL_DEBUG, .get = SAFU_LOG_LEVEL_DEBUG},
                                   {.set = SAFU_LOG_LEVEL_ALL, .get = SAFU_LOG_LEVEL_ALL},
                                   {.set = SAFU_LOG_LEVEL_COUNT},
                                   {.set = SAFU_LOG_LEVEL_INVALID},
                                   {.set = 12},
                                   {.set = -1},
                                   {.set = INT_MAX},
                                   {.set = INT_MIN}};

    for (size_t i = 0, j = 0; i < ARRAY_SIZE(testData); i++) {
        safuLogStatusE_t result = safuLogSetStreamLevel(testData[i].set);
        if (result == SAFU_LOG_STATUS_SUCCESS) {
            j = i;
        }
        assert_int_equal(testData[j].get, safuLogGetStreamLevel());
    }
}
