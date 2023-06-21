// SPDX-License-Identifier: MIT
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>

#include "safuGetHardwareId_utest.h"

#define TMP_TEMPLATE        "/tmp/ut_safuTestSafuGetHardwareIdSuccess-XXXXXX"
#define MAX_SIZE            2048
#define TEST_HARDWAREID     "deadbeef1234"
#define TEST_HARDWAREID_LEN strlen(TEST_HARDWAREID)

typedef struct {
    int fd;
    char *fileName;
} safuTestState_t;

int safuTestSafuGetHardwareIdSuccessSetup(void **state) {
    static char tmpFileName[] = TMP_TEMPLATE;
    static safuTestState_t ts;
    int retval;

    // Create a temporary file with a unique name for the test to use
    ts.fd = mkstemp(tmpFileName);
    assert_false(ts.fd < 0);

    retval = write(ts.fd, TEST_HARDWAREID "\r\n", TEST_HARDWAREID_LEN);
    assert_false(retval < 0);

    retval = setenv(HARDWAREID_FILE, tmpFileName, 1);
    assert_false(retval < 0);

    // Enable the cache
    retval = unsetenv(HARDWAREID_CACHE);
    assert_false(retval < 0);

    ts.fileName = tmpFileName;
    *state = &ts;

    return 0;
}

int safuTestSafuGetHardwareIdSuccessTeardown(void **state) {
    safuTestState_t *ts = *(safuTestState_t **)state;
    int retval;

    retval = unlink(ts->fileName);  // Will automatically remove the file once it's closed
    assert_false(retval < 0);

    retval = close(ts->fd);
    assert_false(retval < 0);

    return 0;
}

void safuTestSafuGetHardwareIdSuccess(UNUSED void **state) {
    const char *hardwareId;
    int retval;

    TEST("safuGetHardwareId");
    SHOULD("%s", "test safuGetHardwareId()'s basic functionality");

    // Run the test for the first time, read from a tempfile set by the environment
    hardwareId = safuGetHardwareId();
    assert_string_equal(hardwareId, TEST_HARDWAREID);

    // Run the test for the second time using an invalid environment filename this time.
    // The test should still succeed, as we're using the buffered value now
    retval = setenv(HARDWAREID_FILE, "/abdefg/abcdefg/gfedcba", 1);
    assert_false(retval < 0);

    hardwareId = safuGetHardwareId();
    assert_string_equal(hardwareId, TEST_HARDWAREID);
}
