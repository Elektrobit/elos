// SPDX-License-Identifier: MIT

#include <errno.h>

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_FD     5
#define TEST_LENGTH 19

void safuTestSafuTransferExactlyErrSendBytesLtZero(UNUSED void **state) {
    int fileDescriptor = TEST_FD, result;
    char buffer[] = "Hello cruel world!";
    size_t length = TEST_LENGTH;
    errno = EINVAL;

    TEST("safuTransferExactly");
    SHOULD("%s", "return transfered bytes if the transfer function returns a value lower than zero");

    expect_value(safuMockTransferFunc, fd, fileDescriptor);
    expect_value(safuMockTransferFunc, buf, buffer);
    expect_value(safuMockTransferFunc, len, length);
    expect_value(safuMockTransferFunc, flags, MSG_NOSIGNAL);
    will_return(safuMockTransferFunc, -1);

    result = safuTransferExactly(fileDescriptor, buffer, length, MSG_NOSIGNAL, safuMockTransferFunc);
    assert_int_not_equal(result, TEST_LENGTH);
}
