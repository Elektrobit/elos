// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <fcntl.h>
#include <safu/result.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define _SHMEM_IDX_INVALID UINT16_MAX

typedef enum elosDltLogLevelE {
    ELOS_DLT_LOGLEVEL_OFF = 1,
    ELOS_DLT_LOGLEVEL_FATAL,
    ELOS_DLT_LOGLEVEL_ERROR,
    ELOS_DLT_LOGLEVEL_WARN,
    ELOS_DLT_LOGLEVEL_INFO,
    ELOS_DLT_LOGLEVEL_DEBUG,
    ELOS_DLT_LOGLEVEL_DEFAULT = ELOS_DLT_LOGLEVEL_INFO,
} elosDltLogLevelE_t;

/* Copy from official definition */
typedef struct elosEbLogEntry {
    uint64_t creationTime;
    uint8_t producerId;
    uint8_t logLevel;
    uint8_t pad[6];
    char logString[128];
} elosEbLogEntry_t;

typedef struct elosEbLogRingBuffer {
    uint16_t idxWrite;
    uint16_t idxRead;
    uint16_t entryCount;
    uint16_t pad;
    elosEbLogEntry_t entries[];
} elosEbLogRingBuffer_t;

typedef struct elosDemo {
    char *shmemName;
    char *semName;
    int shmemFd;
    sem_t *sem;
    elosEbLogRingBuffer_t *ringBuffer;
    off_t shmemOffset;
} elosDemo_t;

static bool elosRunning = true;

static void _signalHandler(int signalno) {
    switch (signalno) {
        case SIGINT:
            elosRunning = false;
            signal(SIGINT, SIG_DFL);
            break;
        case SIGTERM:
            signal(SIGTERM, SIG_DFL);
            elosRunning = false;
            break;
        default:
            break;
    }
}

static safuResultE_t _demoSetup(elosDemo_t *demo) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t size = sizeof(elosEbLogRingBuffer_t);

    demo->shmemFd = shm_open(demo->shmemName, O_RDWR, 0);
    if (demo->shmemFd == -1) {
        perror("shm_open");
    } else {
        demo->ringBuffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, demo->shmemFd, demo->shmemOffset);
        if (demo->ringBuffer == MAP_FAILED) {
            perror("mmap");
        } else if (demo->ringBuffer->entryCount < 1) {
            fprintf(stderr, "ringBuffer is not initialized or has invalid number of entries (entryCount < 1)\n");
        } else {
            size_t const newSize = size + (sizeof(elosEbLogEntry_t) * demo->ringBuffer->entryCount);
            void *ptr = NULL;

            ptr = mremap(demo->ringBuffer, size, newSize, MREMAP_MAYMOVE);
            if (ptr == MAP_FAILED) {
                perror("mremap");
            } else {
                demo->ringBuffer = ptr;

                demo->sem = sem_open(demo->semName, 0);
                if (demo->sem == SEM_FAILED) {
                    perror("sem_open");
                } else {
                    sighandler_t sig;
                    sig = signal(SIGINT, _signalHandler);
                    if (sig == SIG_ERR) {
                        perror("signal(SIGINT, _signalHandler)");
                    } else {
                        sig = signal(SIGTERM, _signalHandler);
                        if (sig == SIG_ERR) {
                            perror("signal(SIGTERM, _signalHandler)");
                        } else {
                            srand(time(NULL));
                            result = SAFU_RESULT_OK;
                        }
                    }
                }
            }
        }
    }

    return result;
}

static safuResultE_t _demoTeardown(elosDemo_t *demo) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (demo->ringBuffer != NULL) {
        size_t const size = sizeof(elosEbLogRingBuffer_t) + (sizeof(elosEbLogEntry_t) * demo->ringBuffer->entryCount);

        munmap(demo->ringBuffer, size);
        demo->ringBuffer = NULL;
    }

    if (demo->shmemFd != -1) {
        close(demo->shmemFd);
        demo->shmemFd = -1;
    }

    if (demo->sem != NULL) {
        sem_close(demo->sem);
        demo->sem = NULL;
    }

    if (demo->shmemName != NULL) {
        free(demo->shmemName);
        demo->shmemName = NULL;
    }

    if (demo->semName != NULL) {
        free(demo->semName);
        demo->semName = NULL;
    }

    return result;
}

static safuResultE_t _demoWriteRingbuffer(elosDemo_t *demo, elosEbLogEntry_t *entry) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal;

    retVal = sem_wait(demo->sem);
    if (retVal == -1) {
        perror("sem_open");
    } else {
        struct timespec date = {0};

        retVal = clock_gettime(CLOCK_REALTIME, &date);
        if (retVal == -1) {
            perror("clock_gettime");
        } else {
            uint16_t idxWrite = demo->ringBuffer->idxWrite;
            uint16_t idxRead = demo->ringBuffer->idxRead;
            uint16_t idxMax = demo->ringBuffer->entryCount - 1;

            entry->creationTime = date.tv_sec;

            memcpy(&demo->ringBuffer->entries[idxWrite], entry, sizeof(elosEbLogEntry_t));

            if (idxRead == _SHMEM_IDX_INVALID) {  // RingBuffer is empty
                demo->ringBuffer->idxRead = 0;
                demo->ringBuffer->idxWrite = 1;
            } else if (idxRead != idxWrite) {  // RingBuffer has not overflown
                idxWrite += 1;
                if (idxWrite > idxMax) {
                    idxWrite = 0;
                }
                demo->ringBuffer->idxWrite = idxWrite;
            } else {  // RingBuffer has overflown
                idxWrite += 1;
                if (idxWrite > idxMax) {
                    idxWrite = 0;
                    idxRead = 0;
                } else {
                    idxRead += 1;
                }
                demo->ringBuffer->idxWrite = 1;
                demo->ringBuffer->idxRead = 0;
            }

            result = SAFU_RESULT_OK;
        }

        retVal = sem_post(demo->sem);
        if (retVal == -1) {
            perror("sem_open");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

static safuResultE_t _demoLoop(elosDemo_t *demo) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    while (elosRunning == true) {
        elosEbLogEntry_t entry = {0};
        int toSend = (rand() % 8) + 1;

        printf("Writing %d random messages into the ringBuffer...\n", toSend);

        for (int i = 0; i < toSend; i++) {
            char const *logString = "Demonstration log with value %u";

            entry.logLevel = (rand() % (ELOS_DLT_LOGLEVEL_DEBUG - 1)) + 1;
            entry.producerId = rand() % UINT8_MAX;
            snprintf((char *restrict)&entry.logString, 127, logString, rand() % UINT16_MAX);

            result = _demoWriteRingbuffer(demo, &entry);
            if (result == SAFU_RESULT_FAILED) {
                break;
            }
        }

        usleep(500 * 1000);
    }

    return result;
}

int main(int argc, char *argv[]) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosDemo_t demo = {0};

    if (argc < 3) {
        fprintf(stderr, "ERROR: At least two parameters are needed.\n");
        fprintf(stderr, "Usage: %s <SHMEM_NAME> <SHMEM_OFFSET> [SEM_NAME]\n", argv[0]);
    } else {
        int retVal;

        demo.shmemOffset = strtol(argv[2], NULL, 10);
        demo.shmemName = strdup(argv[1]);
        if (demo.shmemName == NULL) {
            perror("strdup");
        } else {
            if (argc > 3) {
                retVal = asprintf(&demo.semName, "%s", argv[3]);
            } else {
                retVal = asprintf(&demo.semName, "%s_sem", demo.shmemName);  // NOLINT false positiv
            }

            if (retVal == -1) {
                perror("asprintf");
            } else {
                printf("use sem name : %s\n", demo.semName);
                result = _demoSetup(&demo);
                if (result == SAFU_RESULT_OK) {
                    result = _demoLoop(&demo);
                }
            }
        }
    }

    _demoTeardown(&demo);

    return (result == SAFU_RESULT_OK) ? 0 : 1;
}
