// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include "shmem.h"

#include <fcntl.h>
#include <safu/common.h>
#include <safu/flags.h>
#include <safu/log.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "elos/scanner/scanner.h"
#include "shmem_config.h"
#include "shmem_ringbuffer.h"

static void _updatePermissionOnPath(char const *const name, mode_t mode) {
    char *semPath = NULL;
    int retVal = 0;

    retVal = asprintf(&semPath, "/dev/shm/sem.%s", name);
    if (retVal == -1) {
        safuLogErrErrno("asprintf failed");
    } else if (retVal < -1) {
        safuLogWarnF("Unexpected return value of asprintf: %d", retVal);
    } else {
        retVal = chmod(semPath, mode);
        if (retVal == -1) {
            safuLogErrErrno("chmod failed");
        } else if (retVal < -1 || retVal > 0) {
            safuLogWarnF("Unexpected return value of chmod: %d", retVal);
        }
        free(semPath);
    }
}

static void _updatePermissionsOnFd(int fd, mode_t mode) {
    int retVal = fchmod(fd, mode);
    if (retVal == -1) {
        safuLogErrErrno("fchmod failed");
    } else if (retVal < -1 || retVal > 0) {
        safuLogWarnF("Unexpected return value of fchmod: %d", retVal);
    }
}

static safuResultE_t _openSharedMemory(elosScannerContextShmem_t *context) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int oflag = O_RDWR;
    int retVal = 0;

    if (context->shmemCreate == true) {
        oflag |= O_CREAT | O_TRUNC;
    }

    context->shmemFd = shm_open(context->shmemFile, oflag, mode);
    if (context->shmemFd == -1) {
        safuLogErrErrnoValue("shm_open failed", retVal);
    } else {
        _updatePermissionsOnFd(context->shmemFd, mode);
        result = SAFU_RESULT_OK;

        if (context->shmemCreate == true) {
            retVal = ftruncate(context->shmemFd, context->shmemDataSize);
            if (retVal == -1) {
                safuLogErrErrnoValue("ftruncate failed", retVal);
                result = SAFU_RESULT_FAILED;
            }
        }

        if (result == SAFU_RESULT_OK) {
            size_t const len = context->shmemDataSize;
            int const prot = PROT_READ | PROT_WRITE;
            int const flags = MAP_SHARED;

            context->shmemData = mmap(NULL, len, prot, flags, context->shmemFd, (off_t)context->shmemOffset);
            if (context->shmemData == MAP_FAILED) {
                safuLogErrErrnoValue("Mapping shared memory failed", retVal);
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

static safuResultE_t _closeSharedMemory(elosScannerContextShmem_t *context) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    if (context->shmemData != NULL) {
        retVal = munmap(context->shmemData, context->shmemDataSize);
        if (retVal == -1) {
            safuLogWarnErrnoValue("munmap failed", retVal);
            result = SAFU_RESULT_FAILED;
        }
        context->shmemData = NULL;
    }

    if (context->shmemFd != -1) {
        retVal = close(context->shmemFd);
        if (retVal == -1) {
            safuLogWarnErrnoValue("close failed", retVal);
            result = SAFU_RESULT_FAILED;
        }
        context->shmemFd = -1;
    }

    if (context->shmemCreate == true) {
        retVal = shm_unlink(context->shmemFile);
        if (retVal == -1) {
            safuLogWarnErrnoValue("shm_unlink failed", retVal);
            result = SAFU_RESULT_FAILED;
        }
    }

    if (context->shmemFile != NULL) {
        free(context->shmemFile);
        context->shmemFile = NULL;
    }

    context->shmemDataSize = 0;

    return result;
}

static safuResultE_t _openSemaphore(elosScannerContextShmem_t *context) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int oflag = 0;
    int retVal = 0;

    if (context->semCreate == true) {
        oflag |= O_CREAT;
    }

    context->semData = sem_open(context->semFile, oflag, mode, 1);
    if (context->semData == SEM_FAILED) {
        safuLogErrErrnoValue("sem_open failed", retVal);
    } else {
        _updatePermissionOnPath(context->semFile, mode);
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _closeSemaphore(elosScannerContextShmem_t *context) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    if (context->semData != NULL) {
        retVal = sem_close(context->semData);
        if (retVal == -1) {
            safuLogWarnErrnoValue("sem_close failed", retVal);
            result = SAFU_RESULT_FAILED;
        }
    }

    if (context->shmemCreate == true && context->semFile != NULL) {
        retVal = sem_unlink(context->semFile);
        if (retVal == -1) {
            safuLogWarnErrnoValue("sem_unlink failed", retVal);
            result = SAFU_RESULT_FAILED;
        }
    }

    if (context->semFile != NULL) {
        free(context->semFile);
        context->semFile = NULL;
    }

    return result;
}

static safuResultE_t _waitForNextIteration(UNUSED elosScannerContextShmem_t *context) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    struct timespec const timeout = {.tv_sec = SCANNER_SHMEM_TIMEOUT_SEC, .tv_nsec = SCANNER_SHMEM_TIMEOUT_NSEC};
    int retVal;

    // This is a placeholder solution to keep the scanner from spinning with 100% cpu-time;
    // Ideally we can replace this with something that can be used together with ppoll or epoll
    // that triggers when something was written to the ringbuffer (based on the lock maybe?)
    retVal = nanosleep(&timeout, NULL);
    if (retVal == -1) {
        switch (errno) {
            _CASE_EWOULDBLOCK
            case EAGAIN:
            case EINTR:
                break;
            default:
                safuLogErrErrnoValue("nanosleep failed", retVal);
                break;
        }
    } else if (retVal != 0) {
        safuLogErrErrnoValue("nanosleep failed", retVal);
    } else {
        result = SAFU_RESULT_OK;
    }

    return result;
}

elosScannerResultE_t elosScannerFree(elosScannerSession_t *session) {
    elosScannerResultE_t result = SCANNER_OK;
    elosScannerContextShmem_t *context = session->context;

    if (context != NULL) {
        safuResultE_t retResult;

        retResult = _closeSharedMemory(context);
        if (retResult != SAFU_RESULT_OK) {
            result = SCANNER_ERROR;
        }

        retResult = _closeSemaphore(context);
        if (retResult != SAFU_RESULT_OK) {
            result = SCANNER_ERROR;
        }

        context->shmemCreate = false;

        free(session->context);
        session->context = NULL;
    }

    return result;
}

elosScannerResultE_t elosScannerInitialize(elosScannerSession_t *session, const elosScannerParam_t *param) {
    elosScannerResultE_t result = SCANNER_ERROR;
    size_t const contextSize = sizeof(elosScannerContextShmem_t);
    elosScannerContextShmem_t *context = NULL;

    context = safuAllocMem(NULL, contextSize);
    if (context != NULL) {
        safuResultE_t retResult;

        memset(context, 0, contextSize);

        session->name = SCANNER_SHMEM_NAME;
        context->config = param->config;
        context->shmemFd = -1;
        session->context = context;

        retResult = elosScannerConfigLoad(context);
        if (retResult != SAFU_RESULT_OK) {
            safuLogErrF("Loading configuration for Scanner '%s' failed with %d.", SCANNER_SHMEM_NAME, retResult);
        } else {
            retResult = _openSharedMemory(context);
            if (retResult != SAFU_RESULT_OK) {
                safuLogErrF("Opening shmem for Scanner '%s' failed with %d.", SCANNER_SHMEM_NAME, retResult);
            } else {
                // Semaphores may not be the best thing in this case. It is used for now as it is the easiest to add
                // without modifying the dlt header. Ideally we have some sort of locking that resides inside shmem
                // and the dlt data structure later on (while not using a realtime clock with absolute time for waits)
                retResult = _openSemaphore(context);
                if (retResult != SAFU_RESULT_OK) {
                    safuLogErrF("Opening sem for Scanner '%s' failed with %d.", SCANNER_SHMEM_NAME, retResult);
                } else {
                    retResult = elosScannerRingBufferInitialize(session);
                    result = SCANNER_OK;
                }
            }
        }

        if (result != SCANNER_OK) {
            elosScannerFree(session);
        }
    }

    return result;
}

elosScannerResultE_t elosScannerRun(elosScannerSession_t *session) {
    elosScannerResultE_t result = SCANNER_OK;
    elosScannerContextShmem_t *context = session->context;

    atomic_fetch_or(&context->flags, SCANNER_SHMEM_FLAG_RUNNING);

    while (SCANNER_SHMEM_FLAG_HAS_RUNNING_BIT(&context->flags) == true) {
        safuResultE_t iterResult;

        iterResult = _waitForNextIteration(context);
        if (iterResult != SAFU_RESULT_OK) {
            atomic_fetch_and(&context->flags, ~SCANNER_SHMEM_FLAG_RUNNING);
            result = SCANNER_ERROR;
            break;
        } else {
            iterResult = elosScannerRingBufferPublish(session);
            if (iterResult != SAFU_RESULT_OK) {
                safuLogErrValue("Publishing contents of DltRingBuffer failed.", iterResult);
            }
        }
    }

    return result;
}

elosScannerResultE_t elosScannerStop(elosScannerSession_t *session) {
    elosScannerResultE_t result = SCANNER_OK;
    elosScannerContextShmem_t *context = session->context;

    atomic_fetch_and(&context->flags, ~SCANNER_SHMEM_FLAG_RUNNING);

    return result;
}
