// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_FD     5
#define TEST_LENGTH 19

void safuTestSafuTransferExactlyErrBuffNull(UNUSED void **state) {
    int fileDescriptor = TEST_FD, result;
    char *buffer = NULL;
    size_t length = TEST_LENGTH;

    TEST("safuTransferExactly");
    SHOULD("%s", "return -1 when buffer is a null pointer");

    result = safuTransferExactly(fileDescriptor, buffer, length, 0, safuMockTransferFunc);
    assert_int_equal(result, -1);
}
