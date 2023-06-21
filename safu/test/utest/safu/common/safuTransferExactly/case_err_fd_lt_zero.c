// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_LENGTH 19

void safuTestSafuTransferExactlyErrFdLtZero(UNUSED void **state) {
    int fileDescriptor = -1, result;
    char buffer[] = "Hello cruel world!";
    size_t length = TEST_LENGTH;

    TEST("safuTransferExactly");
    SHOULD("%s", "return -1 if fd is lower than zero");

    result = safuTransferExactly(fileDescriptor, buffer, length, 0, safuMockTransferFunc);
    assert_int_equal(result, -1);
}
