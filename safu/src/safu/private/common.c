// SPDX-License-Identifier: MIT

#include "safu/common.h"

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "safu/log.h"

void *safuAllocMem(void *oldptr, size_t newlen) {
    void *newptr = NULL;

    if (oldptr && newlen == 0) {
        /*
         * IMHO the POSIX standard is ambiguous about the behavior wrt the
         * old memory when newlen is 0 and realloc returns NULL
         */
        free(oldptr);
    } else {
        newptr = realloc(oldptr, newlen);
        if (newptr == NULL && newlen > 0) {
            safuLogErrErrno("realloc");
            free(oldptr);
        }
    }
    return newptr;
}

ssize_t safuTransferExactly(int fd, void *buf, size_t len, int flags,
                            ssize_t (*transferFunc)(int, void *, size_t, int)) {
    ssize_t status;
    size_t initialLen = len;

    if ((fd < 0) || (buf == NULL) || (len < 1)) {
        errno = EINVAL;
        status = -1;
    } else {
        size_t bytesTransferred = 0;
        char *msgBuffer = buf;

        while (bytesTransferred < initialLen) {
            int bytes;
            bytes = transferFunc(fd, &msgBuffer[bytesTransferred], len, flags);
            if (bytes < 0) {
                switch (errno) {
                    case EINTR:
                    case EAGAIN:
// POSIX.1-2001 allows either EAGAIN or EWOULDBLOCK to be returned on blocking socket,
// which might have the same value on some platforms.
#if EAGAIN != EWOULDBLOCK
                    case EWOULDBLOCK:
#endif
                        continue;
                    default:
                        break;
                }
            } else if (bytes == 0) {
                break;
            }
            len -= bytes;
            bytesTransferred += bytes;
        }
        status = bytesTransferred;
    }
    return status;
}

// Find a way to avoid duplicated code
ssize_t safuReadExactly(int fd, void *buf, size_t len) {
    ssize_t status;
    size_t initialLen = len;

    if ((fd < 0) || (buf == NULL) || (len < 1)) {
        errno = EINVAL;
        status = -1;
    } else {
        size_t bytesTransferred = 0;
        char *msgBuffer = buf;

        while (bytesTransferred < initialLen) {
            int bytes;
            bytes = read(fd, &msgBuffer[bytesTransferred], len);
            if (bytes < 0) {
                switch (errno) {
                    case EINTR:
                    case EAGAIN:
                        continue;
                    default:
                        break;
                }
            } else if (bytes == 0) {
                break;
            }
            len -= bytes;
            bytesTransferred += bytes;
        }
        status = bytesTransferred;
    }
    return status;
}

ssize_t safuWriteExactly(int fd, const void *buf, size_t len) {
    ssize_t status;
    size_t initialLen = len;

    if ((fd < 0) || (buf == NULL) || (len < 1)) {
        errno = EINVAL;
        status = -1;
    } else {
        size_t bytesTransferred = 0;
        const char *msgBuffer = buf;

        while (bytesTransferred < initialLen) {
            int bytes;
            bytes = write(fd, &msgBuffer[bytesTransferred], len);
            if (bytes < 0) {
                switch (errno) {
                    case EINTR:
                    case EAGAIN:
                        continue;
                    default:
                        break;
                }
            } else if (bytes == 0) {
                break;
            }
            len -= bytes;
            bytesTransferred += bytes;
        }
        status = bytesTransferred;
    }
    return status;
}

ssize_t safuRecvExactly(int fd, void *buf, size_t len) {
    return safuTransferExactly(fd, buf, len, 0, recv);
}

ssize_t safuSendExactly(int fd, const void *buf, size_t len) {
    // We ensure by tests and guarantee that transferExactly will not modify buf
    return safuTransferExactly(fd, (void *)buf, len, MSG_NOSIGNAL, (safuTransferFunc_t *)send);
}

ssize_t safuGetFileSize(int fd) {
    struct stat fdStat;
    ssize_t result = -1;

    if (fd < 0) {
        safuLogErr("invalid parameter");
    } else {
        result = fstat(fd, &fdStat);
        if (result < 0) {
            safuLogErrErrno("fstat failed");
        } else if (S_ISREG(fdStat.st_mode) == 0) {
            safuLogErr("S_ISREG failed");
        } else {
            result = fdStat.st_size;
        }
    }

    return result;
}

ssize_t safuReadFileToString(const char *fileName, ssize_t maxLength, char **string) {
    char *buffer = NULL;
    off_t size = 0;
    ssize_t result = 0;
    int fd = -1;

    if ((fileName == NULL) || (maxLength == 0) || (maxLength < -1) || (string == NULL)) {
        safuLogErr("invalid parameter");
        result = -1;
    }

    if (result == 0) {
        fd = open(fileName, O_RDONLY);
        if (fd < 0) {
            safuLogErrErrno("open");
            result = -1;
        }
    }

    size = safuGetFileSize(fd);
    if (size < 0) {
        safuLogErr("safuGetFileSize failed");
        result = -1;
    } else if (size == 0) {
        safuLogErr("file is empty");
        result = -1;
    } else if ((maxLength > 0) && (size > maxLength)) {
        safuLogErrF("file is bigger (%ld) than the specified maximum length (%ld)", size, maxLength);
        result = -1;
    }

    if (result == 0) {
        buffer = safuAllocMem(NULL, size + 1);
        if (buffer == NULL) {
            safuLogErr("safuAllocMem failed");
            result = -1;
        }
    }

    if (result == 0) {
        result = safuReadExactly(fd, buffer, size);
        if (result < 0) {
            safuLogErrErrno("safuRecvExactly failed");
            result = -1;
        } else if (result != size) {
            safuLogErrF("safuRecvExactly incomplete (%ld of %ld bytes)", result, size);
            result = -1;
        } else {
            buffer[size] = '\0';
        }
    }

    if (fd >= 0) {
        int retval;
        retval = close(fd);
        if (retval < 0) {
            safuLogWarnF("close failed - %s", strerror(errno));
            result = -1;
        }
    }

    if (result < 0) {
        free(buffer);
    } else {
        *string = buffer;
        result = size;
    }

    return result;
}

const char *safuGetHardwareId(void) {
    static char hardwareIdString[SAFU_HARDWAREID_MAX_LENGTH];
    static bool initialized = false;
    const ssize_t maxLength = SAFU_HARDWAREID_MAX_LENGTH - 1;
    const ssize_t minLength = 1;

    if (initialized == false) {
        char *hardwareIdFile;
        char *envValue;
        char *buffer = NULL;
        bool useCache = true;
        ssize_t bytes;

        // These getenv() calls are a temporary solution until safu_config_t is implemented,
        // which will come with an ability to ignore environment variables completely

        envValue = getenv(SAFU_HARDWAREID_ENVIRONMENT_PREFIX "CACHE");
        if ((envValue != NULL) && (strncmp(envValue, "0", 2) == 0)) {
            useCache = false;
        }

        envValue = getenv(SAFU_HARDWAREID_ENVIRONMENT_PREFIX "FILE");
        if ((envValue == NULL) || (strlen(envValue) == 0)) {
            hardwareIdFile = SAFU_HARDWAREID_FILE;
        } else {
            hardwareIdFile = envValue;
        }

        bytes = safuReadFileToString(hardwareIdFile, maxLength, &buffer);
        if (bytes < minLength) {
            if (bytes < 0) {
                const char err[] = "safuReadFileToString failed, using default value '%s' (file: '%s')";
                safuLogErrF(err, SAFU_HARDWAREID_DEFAULT_VALUE, hardwareIdFile);
            } else {
                const char err[] = "hardwareId too short (%ld), using default value '%s' (file: '%s')";
                safuLogErrF(err, bytes, SAFU_HARDWAREID_DEFAULT_VALUE, hardwareIdFile);
            }
            strcpy(hardwareIdString, SAFU_HARDWAREID_DEFAULT_VALUE);
        } else {
            int i = 0;
            while (i < bytes) {
                // Get rid of unwanted characters, like the LF at the end of the file
                switch (buffer[i]) {
                    case '\r':
                    case '\n':
                        buffer[i] = 0;
                        break;
                    case '\0':
                        i = bytes;
                        break;
                    default:
                        break;
                }
                i += 1;
            }
            strncpy(hardwareIdString, buffer, bytes);
            hardwareIdString[bytes] = '\0';
        }

        free(buffer);

        if (useCache == true) {
            initialized = true;
        }
    }

    return hardwareIdString;
}

const char *safuGetEnvOr(const char *key, const char *defaultValue) {
    const char *envValue = defaultValue;
    if (key != NULL) {
        envValue = getenv(key);
        if (envValue == NULL) {
            envValue = defaultValue;
        }
    }
    return envValue;
}
