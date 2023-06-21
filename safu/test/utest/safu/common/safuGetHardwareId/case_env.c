// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "safuGetHardwareId_utest.h"

#define TEST_FILE "/tmp/test.id"

int safuTestSafuGetHardwareIdEnvSetup(UNUSED void **state) {
    int retval;

    // Make sure we don't buffer the value during testing
    retval = setenv(HARDWAREID_CACHE, "0", 1);
    assert_false(retval < 0);

    return 0;
}

int safuTestSafuGetHardwareIdEnvTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuGetHardwareIdEnv(UNUSED void **state) {
    int retval;

    TEST("safuGetHardwareId");
    SHOULD("%s", "test safuGetHardwareId()'s file selection via the environment");

    // We're only interested in the file selection mechanism in this test,
    // so we use the mocked safuReadFileToString to verify the used fileName
    // and fail the function execution afterwards to keep the test simple
    expect_any_always(safuReadFileToString, maxLength);
    expect_any_always(safuReadFileToString, string);
    will_return_always(safuReadFileToString, -1);

    // Use the default file (environment not set)
    retval = unsetenv(HARDWAREID_FILE);
    assert_true(retval == 0);
    MOCK_FUNC_AFTER_CALL(safuReadFileToString, 0);
    expect_string(safuReadFileToString, fileName, SAFU_HARDWAREID_FILE);
    safuGetHardwareId();

    // Use the default file (environment set but zero length)
    retval = setenv(HARDWAREID_FILE, "", 1);
    assert_true(retval == 0);
    MOCK_FUNC_AFTER_CALL(safuReadFileToString, 0);
    expect_string(safuReadFileToString, fileName, SAFU_HARDWAREID_FILE);
    safuGetHardwareId();

    // Use the environment file
    retval = setenv(HARDWAREID_FILE, TEST_FILE, 1);
    assert_true(retval == 0);
    MOCK_FUNC_AFTER_CALL(safuReadFileToString, 0);
    expect_string(safuReadFileToString, fileName, TEST_FILE);
    safuGetHardwareId();
}
