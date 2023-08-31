// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_FD     5
#define TEST_LENGTH 19
#define FIRST_PART  10
#define SECOND_PART 9

void safuTestSafuTransferExactlySuccessInTwoLoops(UNUSED void **state) {
    int fileDescriptor = TEST_FD, result;
    char buffer[] = "Hello cruel world!";
    size_t length = TEST_LENGTH;
    size_t transferred = 0xdeadb33f;

    TEST("safuTransferExactly");
    SHOULD("%s %d %s", "transfer successfully", TEST_LENGTH, "bytes in two loops");

    expect_value(safuMockTransferFunc, fd, fileDescriptor);
    expect_value(safuMockTransferFunc, buf, buffer);
    expect_value(safuMockTransferFunc, len, length);
    expect_value(safuMockTransferFunc, flags, MSG_NOSIGNAL);
    will_return(safuMockTransferFunc, FIRST_PART);

    expect_value(safuMockTransferFunc, fd, fileDescriptor);
    expect_value(safuMockTransferFunc, buf, buffer + FIRST_PART);
    expect_value(safuMockTransferFunc, len, length - FIRST_PART);
    expect_value(safuMockTransferFunc, flags, MSG_NOSIGNAL);
    will_return(safuMockTransferFunc, SECOND_PART);

    result = safuTransferExactly(fileDescriptor, buffer, length, MSG_NOSIGNAL, safuMockTransferFunc, &transferred);
    assert_int_equal(transferred, length);
    assert_int_equal(result, SAFU_RESULT_OK);
}
