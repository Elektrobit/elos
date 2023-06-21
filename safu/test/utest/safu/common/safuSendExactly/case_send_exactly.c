// SPDX-License-Identifier: MIT

#include "safuSendExactly_utest.h"

void safuTestSafuSendExactly(UNUSED void **state) {
    int fileDescriptor = 0;
    char buffer[] = "Hello cruel world!";
    size_t length = 0;
    ssize_t result;

    TEST("safuSendExactly");
    SHOULD("%s", "call safuSendExactly successfully");

    expect_value(safuTransferExactly, fd, fileDescriptor);
    expect_value(safuTransferExactly, buf, buffer);
    expect_value(safuTransferExactly, len, length);
    expect_value(safuTransferExactly, flags, MSG_NOSIGNAL);
    will_return(safuTransferExactly, sizeof(buffer));

    result = safuSendExactly(fileDescriptor, buffer, length);
    assert_int_equal(result, sizeof(buffer));
    assert_string_equal(buffer, "Hello cruel world!");
}
