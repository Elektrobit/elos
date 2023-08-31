// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/common.h"

safuResultE_t safuTransferExactly(int fd, void *buf, size_t len, int flags,
                                  UNUSED ssize_t (*transferFunc)(int, void *, size_t, int), size_t *transferred) {
    check_expected(fd);
    check_expected_ptr(buf);
    check_expected(len);
    check_expected(flags);
    *transferred = mock_type(size_t);
    return mock_type(safuResultE_t);
}
