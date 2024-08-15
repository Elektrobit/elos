// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>
#include <samconf/samconf.h>
#include <semaphore.h>

#include "scanner/shmem/types.h"

#define SCANNER_SHMEM_FLAG_RUNNING              SAFU_FLAG_CUSTOM_START_BIT
#define SCANNER_SHMEM_FLAG_HAS_RUNNING_BIT(__f) ((atomic_load(__f) & SCANNER_SHMEM_FLAG_RUNNING) != 0)

#if EAGAIN != EWOULDBLOCK
#define _CASE_EWOULDBLOCK case EWOULDBLOCK:
#else
#define _CASE_EWOULDBLOCK
#endif

#define SCANNER_NAME "shmem"

typedef struct elosScannerContextShmem {
    safuFlags_t flags;
    int shmemFd;
    char *shmemFile;
    bool shmemCreate;
    uint32_t shmemLogEntries;
    uint32_t shmemOffset;
    elosEbLogRingBuffer_t *shmemData;
    int shmemDataSize;
    sem_t *semData;
    char *semFile;
    bool semCreate;
    struct elosPublisher *publisher;
} elosScannerContextShmem_t;
