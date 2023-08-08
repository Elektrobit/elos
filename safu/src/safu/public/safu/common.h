// SPDX-License-Identifier: MIT
#pragma once

#include <safu/defines.h>
#include <safu/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

/*******************************************************************
 * Checks if the given string argument is empty. The given string
 * is treated as empty if it is NULL or if it consists only of white spaces.
 * White space characters are defined by libc's `isspace()` function
 * like spaces ` `, tab `\t`, carriage return `\r`, form feed `\f`
 * newline `\n`, vertical tab `\v` etc.
 *
 * Parameters:
 *      stringToCheck (const char *): string to be checked.
 * Returns:
 *      - `SAFU_RESULT_OK` when the stringToCheck is empty
 *      - `SAFU_RESULT_FAILED` when the stringToCheck is not empty
 ******************************************************************/
safuResultE_t safuStringIsEmpty(const char *stringToCheck);

void *safuAllocMem(void *oldptr, size_t newlen);
ssize_t safuTransferExactly(int fd, void *buf, size_t len, int flags, safuTransferFunc_t *transferFunc);
ssize_t safuRecvExactly(int fd, void *buf, size_t len);
ssize_t safuSendExactly(int fd, const void *buf, size_t len);
const char *safuGetHardwareId(void);
ssize_t safuReadFileToString(const char *fileName, ssize_t maxLength, char **string);
ssize_t safuGetFileSize(int fd);
const char *safuGetEnvOr(const char *key, const char *defaultValue);
ssize_t safuWriteExactly(int fd, const void *buf, size_t len);
