// SPDX-License-Identifier: MIT

#include "safuRecvExactly_utest.h"

void safuTestSafuRecvExactly(UNUSED void **state) {
    int fileDescriptor = 0;
    char buffer[] = "Hello cruel world!";
    size_t length = 0;
    size_t transferred = 0xdeadb33f;
    safuResultE_t result;

    TEST("safuRecvExactly");
    SHOULD("%s", "call safuRecvExactly successfully");

    expect_value(safuTransferExactly, fd, fileDescriptor);
    expect_value(safuTransferExactly, buf, buffer);
    expect_value(safuTransferExactly, len, length);
    expect_value(safuTransferExactly, flags, 0);
    will_set_parameter(safuTransferExactly, transferred, 0);
    will_return(safuTransferExactly, SAFU_RESULT_OK);

    result = safuRecvExactly(fileDescriptor, buffer, length, &transferred);
    assert_int_equal(transferred, 0);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(buffer, "Hello cruel world!");
}
