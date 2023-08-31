// SPDX-License-Identifier: MIT

#include "safuSendExactly_utest.h"

void safuTestSafuSendExactly(UNUSED void **state) {
    int fileDescriptor = 0;
    char buffer[] = "Hello cruel world!";
    size_t length = 0;
    safuResultE_t result;
    size_t transferred;

    TEST("safuSendExactly");
    SHOULD("%s", "call safuSendExactly successfully");

    expect_value(safuTransferExactly, fd, fileDescriptor);
    expect_value(safuTransferExactly, buf, buffer);
    expect_value(safuTransferExactly, len, length);
    expect_value(safuTransferExactly, flags, MSG_NOSIGNAL);
    will_set_parameter(safuTransferExactly, transferred, sizeof(buffer))
        will_return(safuTransferExactly, SAFU_RESULT_OK);

    result = safuSendExactly(fileDescriptor, buffer, length, &transferred);
    assert_int_equal(transferred, sizeof(buffer));
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_string_equal(buffer, "Hello cruel world!");
}
