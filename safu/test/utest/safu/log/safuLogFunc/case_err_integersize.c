// SPDX-License-Identifier: MIT

#include "safuLogFunc_utest.h"

void safuTestSafuLogFuncErrIntSize(UNUSED void **state) {
    char *message = "safuLogInfoF: DEADBEEF";
    FILE *tmp;
    char *buf = NULL;
    size_t bufSize = 0;
    int64_t x = 0xDEADBEEF;

    TEST("safuLogFuncF");
    SHOULD("%s", "print warning for integer of wrong size");

    tmp = open_memstream(&buf, &bufSize);
    if (tmp == NULL) {
        fail_msg("ERROR: open memstream failed");
        return;
    }

    safuLogSetStream(SAFU_LOG_LEVEL_ALL, tmp);
    safuLogInfoF("safuLogInfoF: %0lX", x);

    fclose(tmp);

    assert_int_equal('\n', buf[bufSize - 1]);
    buf[bufSize - 1] = '\0';  // trim new line
    assert_string_equal(message, buf);

    free(buf);
}
