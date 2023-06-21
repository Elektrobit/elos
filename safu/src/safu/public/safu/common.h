// SPDX-License-Identifier: MIT
#pragma once

#include <safu/defines.h>
#include <safu/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void *safuAllocMem(void *oldptr, size_t newlen);
ssize_t safuTransferExactly(int fd, void *buf, size_t len, int flags, safuTransferFunc_t *transferFunc);
ssize_t safuRecvExactly(int fd, void *buf, size_t len);
ssize_t safuSendExactly(int fd, const void *buf, size_t len);
const char *safuGetHardwareId(void);
ssize_t safuReadFileToString(const char *fileName, ssize_t maxLength, char **string);
ssize_t safuGetFileSize(int fd);
const char *safuGetEnvOr(const char *key, const char *defaultValue);
ssize_t safuWriteExactly(int fd, const void *buf, size_t len);
