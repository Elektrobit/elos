// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"
#include "safuTransferExactly_utest.h"

#define TEST_FD     5
#define TEST_LENGTH 19

void safuTestSafuTransferExactlyErrBuffNull(UNUSED void **state) {
    int fileDescriptor = TEST_FD, result;
    char *buffer = NULL;
    size_t length = TEST_LENGTH;
    size_t transferred = 0xdeadb33f;

    TEST("safuTransferExactly");
    SHOULD("%s", "return SAFU_RESULT_FAILED when buffer is a null pointer");

    result = safuTransferExactly(fileDescriptor, buffer, length, 0, safuMockTransferFunc, &transferred);
    assert_int_equal(transferred, 0xdeadb33f);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
