// SPDX-License-Identifier: MIT

#include "mock_transferFunc.h"

#include <cmocka_extensions/cmocka_extensions.h>

ssize_t safuMockTransferFunc(int fd, void *buf, size_t len, int flags) {
    check_expected(fd);
    check_expected_ptr(buf);
    check_expected(len);
    check_expected(flags);
    return mock_type(ssize_t);
}
