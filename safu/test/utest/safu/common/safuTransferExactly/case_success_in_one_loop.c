// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_FD     5
#define TEST_LENGTH 19

void safuTestSafuTransferExactlySuccessInOneLoop(UNUSED void **state) {
    int fileDescriptor = TEST_FD, result;
    char buffer[] = "Hello cruel world!";
    size_t length = TEST_LENGTH;

    TEST("safuTransferExactly");
    SHOULD("%s %d %s", "transfer successfully", TEST_LENGTH, "bytes in one loop");

    expect_value(safuMockTransferFunc, fd, fileDescriptor);
    expect_value(safuMockTransferFunc, buf, buffer);
    expect_value(safuMockTransferFunc, len, length);
    expect_value(safuMockTransferFunc, flags, 0);
    will_return(safuMockTransferFunc, TEST_LENGTH);

    result = safuTransferExactly(fileDescriptor, buffer, length, 0, safuMockTransferFunc);
    assert_int_equal(result, TEST_LENGTH);
}
