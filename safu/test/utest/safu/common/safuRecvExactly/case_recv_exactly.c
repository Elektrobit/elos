// SPDX-License-Identifier: MIT

#include "safuRecvExactly_utest.h"

void safuTestSafuRecvExactly(UNUSED void **state) {
    int fileDescriptor = 0;
    char buffer[] = "Hello cruel world!";
    size_t length = 0;
    ssize_t result;

    TEST("safuRecvExactly");
    SHOULD("%s", "call safuRecvExactly successfully");

    expect_value(safuTransferExactly, fd, fileDescriptor);
    expect_value(safuTransferExactly, buf, buffer);
    expect_value(safuTransferExactly, len, length);
    expect_value(safuTransferExactly, flags, 0);
    will_return(safuTransferExactly, sizeof(buffer));

    result = safuRecvExactly(fileDescriptor, buffer, length);
    assert_int_equal(result, sizeof(buffer));
    assert_string_equal(buffer, "Hello cruel world!");
}
