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
 * like spaces, tab `\\t`, carriage return `\\r`, form feed `\f`
 * newline `\\n`, vertical tab `\\v` etc.
 *
 * Parameters:
 *      stringToCheck (const char *): string to be checked.
 * Returns:
 *      - `SAFU_RESULT_OK` when the stringToCheck is empty
 *      - `SAFU_RESULT_FAILED` when the stringToCheck is not empty
 ******************************************************************/
safuResultE_t safuStringIsEmpty(const char *stringToCheck);

/*******************************************************************
 * (re-)allocate Memory of a given size. This function is essentially a wrapper
 * for the IOS C standard `realloc` command.
 *
 * Parameters:
 *      oldptr (void *): pointer to memory that should be reallocated (or NULL)
 *      newlen (size_t): size of the newly allocated memory area
 * Returns:
 *      - pointer to newly allocated memory
 ******************************************************************/
void *safuAllocMem(void *oldptr, size_t newlen);

/*******************************************************************
 * Allows to transfer a precise length of data using a given transfer method that
 * would usually not allow to write that length of data with just one command.
 * The primary purpose of this function is to be called by `safuSendExactly`
 * and `safuRecvExactly`, since both use share a lot of the same code.
 *
 * Parameters:
 *      fd (int): file descriptor of the socket to/from which the data should be
 *            transferred.
 *      buf (void *): pointer to data to read from or write to.
 *      len (size_t): length/amount of data to be transferred.
 *      flags (int): flags for transfer method.
 *      transferFunc (safuTransferFunc_t *): function to be called to actually
 *                                           interact with file descriptor.
 *      transfered (size_t *): length/amount of data which has been transferred.
 * Returns:
 *      - length of transferred data
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_CLOSED` if connection was closed by remote peer
 *      - `SAFU_RESULT_FAILED' on error, the connection state `fd` , `buf` and `transfered` is undefined
 ******************************************************************/
safuResultE_t safuTransferExactly(int fd, void *buf, size_t len, int flags, safuTransferFunc_t *transferFunc,
                                  size_t *transferred);

/*******************************************************************
 * Obtain data from a file descriptor(socket) using `recv`.
 *
 * Parameters:
 *      fd (int): file descriptor of the socket from which the data should be
 *            received to.
 *      buf (void *): buffer to store the received data.
 *      len (size_t): length/amount of data to be transferred.
 *      flags (int): recv flags
 *      transfered (size_t *): length/amount of data which has been transferred.
 * Returns:
 *      - length of received data
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_CLOSED` if connection was closed by remote peer
 *      - `SAFU_RESULT_FAILED' on error, the connection state `fd` , `buf` and `transfered` is undefined
 ******************************************************************/
safuResultE_t safuRecvExactly(int fd, void *buf, size_t len, size_t *transferred);

/*******************************************************************
 * Send data to a file descriptor(socket) using `send`.
 *
 * Parameters:
 *      fd (int): file descriptor of the socket to which the data should be
 *              send to.
 *      buf (void *): pointer to data that should be transferred.
 *      len (size_t): length/amount of data to be transferred.
 *      flags (int): send flags
 *      transfered (size_t *): length/amount of data which has been transferred.
 * Returns:
 *      - length of send data
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_CLOSED` if connection was closed by remote peer
 *      - `SAFU_RESULT_FAILED' on error, the connection state `fd` , `buf` and `transfered` is undefined
 ******************************************************************/
safuResultE_t safuSendExactly(int fd, const void *buf, size_t len, size_t *transferred);

/*******************************************************************
 * Get a string representing the hardware the system runs on from a file.
 * The default location for this file is `/etc/machine-id`, which is defined by
 * https://www.freedesktop.org/software/systemd/man/machine-id.html.
 * If `/etc/machine-id` is not present, the value of the compile define
 * `SAFU_HARDWAREID_DEFAULT_VALUE` will be used. The location of the hardware-id
 * file can be overwritten by defining the environment variable
 * `SAFU_HARDWAREID_FILE`.
 *
 * In Addition, it is possible to define the environment variable
 * `SAFU_HARDWAREID_CACHE`. If it is set to `0`, the value of the hardware-id
 * will be read on every call of the function. If it is set to any other value,
 * the value of the first call will be cached and reused for every function
 * call.
 *
 * Parameters:
 *      void
 * Returns:
 *      - string containing hardware id
 ******************************************************************/
const char *safuGetHardwareId(void);

/*******************************************************************
 * Read the contents of a file and write it to a string buffer.
 *
 * Parameters:
 *      fileName (char *): String containing the path to the file to be read
 *      maxLength (ssize_t): maximum size of file in bytes
 *      string (char **): pointer to string that should contain the file contents,
 *               on failure the pointer remains unchanged
 * Returns:
 *      - Amount of bytes that were written to `*string`
 *      - `-1` if An error occurred during the function execution.
 ******************************************************************/
ssize_t safuReadFileToString(const char *fileName, ssize_t maxLength, char **string);

/*******************************************************************
 * Get the size of a file in bytes.
 *
 * Parameters:
 *      fd (int): file descriptor of file
 * Returns:
 *      - size of file in bytes
 *      - `-1` on error
 ******************************************************************/
ssize_t safuGetFileSize(int fd);

/*******************************************************************
 * Retrieve value of a environment variable. If the variable is not set, the
 * function returns the provided default value.
 *
 * Parameters:
 *      key (const char *): name of environment variable
 *      defaultValue (const char *): Default value to be returned, if the
 *              environment variable is not defined
 * Returns:
 *      - value of the environment variable or default Value
 ******************************************************************/
const char *safuGetEnvOr(const char *key, const char *defaultValue);

/*******************************************************************
 * Write data from buffer to file. This if the given buffer contains more
 * bytes the can be written at once, the data will be written with multiple
 * iterations until either an error occurs, or until the given length has been
 * written.
 *
 * Parameters:
 *      fd (int): file descriptor of file to be written
 *      buf (const void *): pointer to data buffer to write out
 *      len (size_t): length of data to write out
 * Returns:
 *      - length of data that was written in byte
 *      - `-1` on invalid parameters and set `errno` to `EINVAL`
 ******************************************************************/
ssize_t safuWriteExactly(int fd, const void *buf, size_t len);
