// SPDX-License-Identifier: MIT

#ifndef SAFU_TRANSFEREXACTLY_MOCK_H
#define SAFU_TRANSFEREXACTLY_MOCK_H

#include <stdio.h>

ssize_t safuMockTransferFunc(int fd, void *buf, size_t len, int flags);

#endif /* SAFU_TRANSFEREXACTLY_MOCK_H */
