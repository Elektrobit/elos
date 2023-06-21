// SPDX-License-Identifier: MIT

#include <limits.h>

#include "safuLogLevelToString_utest.h"

void safuTestSafuLogLevelToStringErrInvalidLogLevel(UNUSED void **state) {
    TEST("safuLogLevelToString");
    SHOULD("%s", "return \"INVALID\" for log level thats not valid to be set");

    safuLogLevelE_t testInput[] = {SAFU_LOG_LEVEL_COUNT, SAFU_LOG_LEVEL_INVALID, 12, -1, INT_MAX, INT_MIN};

    for (size_t i = 0; i < ARRAY_SIZE(testInput); i++) {
        assert_string_equal("INVALID", safuLogLevelToString(testInput[i]));
    }
}
