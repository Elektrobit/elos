// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_FD     5
#define TEST_LENGTH 19

void safuTestSafuTransferExactlyErrSendBytesEqZero(UNUSED void **state) {
    int fileDescriptor = TEST_FD, result;
    char buffer[] = "Hello cruel world!";
    size_t length = TEST_LENGTH;

    TEST("safuTransferExactly");
    SHOULD("%s", "return value lower than length if the transfer function returns zero");

    expect_value(safuMockTransferFunc, fd, fileDescriptor);
    expect_value(safuMockTransferFunc, buf, buffer);
    expect_value(safuMockTransferFunc, len, length);
    expect_value(safuMockTransferFunc, flags, 0);
    will_return(safuMockTransferFunc, 0);

    result = safuTransferExactly(fileDescriptor, buffer, length, 0, safuMockTransferFunc);
    assert_int_not_equal(result, length);
}
