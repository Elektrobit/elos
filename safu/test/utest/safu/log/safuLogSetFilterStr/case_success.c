// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

#include "safuLogSetFilterStr_utest.h"

static char safuBuffer[200];

static void safuTestSafuLogSetFilterStringSuccessParam(const char *filterStr, bool shouldLog) {
    PARAM("filterStr = \"%s\"", filterStr);

    memset(safuBuffer, 0, sizeof(safuBuffer));
    FILE *stream = fmemopen(safuBuffer, sizeof(safuBuffer), "w+");
    safuLogSetStream(SAFU_LOG_LEVEL_ERR, stream);

    safuLogStatusE_t result = safuLogSetFilterStr(filterStr);
    assert_int_equal(result, SAFU_LOG_STATUS_SUCCESS);
    safuLogErr("An Error logged by this test");

    fclose(stream);
    if (shouldLog) {
        char *expectedError = "ERROR: An Error logged by this test - file 'case_success.c'";
        safuBuffer[59] = '\0';
        assert_string_equal(expectedError, safuBuffer);
    } else {
        assert_string_equal("", safuBuffer);
    }
}

void safuTestSafuLogSetFilterStringSuccess(UNUSED void **state) {
    TEST("safuLogSetFilterStr");
    SHOULD("%s", "set a filter string");

    safuTestSafuLogSetFilterStringSuccessParam("", true);
    safuTestSafuLogSetFilterStringSuccessParam("case_success.c", true);
    safuTestSafuLogSetFilterStringSuccessParam("main.c", false);
    safuTestSafuLogSetFilterStringSuccessParam("main.c;case_success.c", true);
}
