// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_LENGTH 19

void safuTestSafuTransferExactlyErrFdLtZero(UNUSED void **state) {
    int fileDescriptor = -1, result;
    char buffer[] = "Hello cruel world!";
    size_t length = TEST_LENGTH;
    size_t transferred = 0xdeadb33f;

    TEST("safuTransferExactly");
    SHOULD("%s", "return SAFU_RESULT_FAILED if fd is lower than zero");

    result = safuTransferExactly(fileDescriptor, buffer, length, 0, safuMockTransferFunc, &transferred);
    assert_int_equal(transferred, 0xdeadb33f);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
