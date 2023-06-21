// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "safuGetHardwareId_utest.h"

int safuTestSafuGetHardwareIdErrFileSetup(UNUSED void **state) {
    int retval;

    // Make sure we don't buffer the value during testing
    retval = setenv(HARDWAREID_CACHE, "0", 1);
    assert_false(retval < 0);

    return 0;
}

int safuTestSafuGetHardwareIdErrFileTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuGetHardwareIdErrFile(UNUSED void **state) {
    const char *hardwareId;

    TEST("safuGetHardwareId");
    SHOULD("%s", "test safuGetHardwareId()'s reaction to file reading errors");

    // In this test we're only interested in safuGetHardwareId()'s reactions
    // to failed safuReadFileToString() calls, so we can ignore its parameters here
    expect_any_always(safuReadFileToString, fileName);
    expect_any_always(safuReadFileToString, maxLength);
    expect_any_always(safuReadFileToString, string);

    // Simulate a file access failure
    MOCK_FUNC_AFTER_CALL(safuReadFileToString, 0);
    will_return(safuReadFileToString, -1);
    hardwareId = safuGetHardwareId();
    assert_string_equal(hardwareId, SAFU_HARDWAREID_DEFAULT_VALUE);

    // Simulate a zero length file
    MOCK_FUNC_AFTER_CALL(safuReadFileToString, 0);
    will_return(safuReadFileToString, 0);
    hardwareId = safuGetHardwareId();
    assert_string_equal(hardwareId, SAFU_HARDWAREID_DEFAULT_VALUE);
}
