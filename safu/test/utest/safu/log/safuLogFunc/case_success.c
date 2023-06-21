// SPDX-License-Identifier: MIT

#include "safuLogFunc_utest.h"

void safuTestSafuLogFuncSuccess(UNUSED void **state) {
    char *message = "safuLogInfo works!";
    FILE *tmp;
    char *buf = NULL;
    size_t bufSize = 0;

    TEST("safuLogFunc");
    SHOULD("%s", "print the log message");

    tmp = open_memstream(&buf, &bufSize);
    if (tmp == NULL) {
        fail_msg("ERROR: open memstream failed");
        return;
    }

    safuLogSetStream(SAFU_LOG_LEVEL_ALL, tmp);
    safuLogInfo(message);

    fclose(tmp);

    assert_int_equal('\n', buf[bufSize - 1]);
    buf[bufSize - 1] = '\0';  // trim new line
    assert_string_equal(message, buf);

    free(buf);
}
