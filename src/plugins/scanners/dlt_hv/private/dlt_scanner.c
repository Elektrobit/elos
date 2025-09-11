// SPDX-License-Identifier: MIT

#include "dlt_hv/dlt_scanner.h"

#include <elos/libelosplugin/libelosplugin.h>
#include <fcntl.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <unistd.h>

#include "dlt_hv/scanner.h"
#include "dlt_hv/shmem_ring_buffer.h"
#include "dlt_hv/types.h"
#include "dlt_hv/utils.h"

static struct timespec _hzToTime(int32_t hz) {
    struct timespec ts = {0};
    ts.tv_nsec = 1000000000 / hz;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = ts.tv_nsec % 1000000000;
    return ts;
}

safuResultE_t elosDltScannerInit(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosDltScanner_t *dlt = safuAllocMem(NULL, sizeof(elosDltScanner_t));
    if (dlt == NULL) {
        safuLogErr("failed to allocate memory for dlt scanner");
        result = SAFU_RESULT_FAILED;
    } else {
        samconfConfigStatusE_t configResult =
            samconfConfigGetString(plugin->config, "Config/DeviceFile", (const char **)&dlt->shmemFile);
        if (configResult != SAMCONF_CONFIG_OK) {
            safuLogErrF("missing shared memory file in configuration for \"%s\"", plugin->config->key);
            result = SAFU_RESULT_FAILED;
        } else {
            dlt->offsetAddress = 0;
            configResult =
                elosConfigGetGenericInt64(plugin->config, "Config/OffsetAddress", (int64_t *)&dlt->offsetAddress);
            if (configResult == SAMCONF_CONFIG_NOT_FOUND) {
                dlt->offsetAddress = 0;
            } else if (configResult != SAMCONF_CONFIG_OK) {
                safuLogErrF("no offset address in configuration for \"%s\"", plugin->config->key);
                result = SAFU_RESULT_FAILED;
            } else {
                long pageSize = sysconf(_SC_PAGE_SIZE);
                if (dlt->offsetAddress % pageSize != 0) {
                    safuLogErrF("offset address %lu is not a multiple of page size (%ld)\n", dlt->offsetAddress,
                                pageSize);
                    result = SAFU_RESULT_FAILED;
                }
            }
            if (result == SAFU_RESULT_OK) {
                configResult =
                    elosConfigGetGenericInt64(plugin->config, "Config/BufferSize", (int64_t *)&dlt->bufferSize);
                if (configResult == SAMCONF_CONFIG_NOT_FOUND) {
                    dlt->bufferSize = 0;
                } else if (configResult != SAMCONF_CONFIG_OK) {
                    safuLogErrF("no buffer size in configuration for \"%s\"", plugin->config->key);
                    result = SAFU_RESULT_FAILED;
                }
            }
            if (result == SAFU_RESULT_OK) {
                result = elosPluginCreatePublisher(plugin, &dlt->publisher);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrF("failed to create publisher for \"%s\"", plugin->config->key);
                } else {
                    // NOTE:  the mesured time diffrences in the provided example are between 14.36598s and 0.000439s
                    dlt->sleepInterval = _hzToTime(
                        samconfConfigGetInt32Or(plugin->config, "Config/ScanFrequency", ELOS_DLT_SCAN_FREQUENCY));
                    dlt->moreToRead = eventfd(0, 0);
                    dlt->stopCmd = eventfd(0, 0);

                    plugin->data = dlt;
                }
            }
        }
    }
    if (result != SAFU_RESULT_OK) {
        free(dlt);
        dlt = NULL;
    }
    return result;
}

static safuResultE_t _deleteEbLogEntry(void *entry) {
    return elosFreeEbLogEntry((elosEbLogEntry_t *)entry);
}

safuResultE_t elosDltScannerOpenBuffer(elosDltScanner_t *dlt) {
    safuResultE_t result = SAFU_RESULT_OK;
    dlt->shmemFd = open(dlt->shmemFile, O_RDONLY | O_SYNC);
    if (dlt->shmemFd == -1) {
        result = SAFU_RESULT_FAILED;
    } else {
        safuLogDebugF("Opend dlt log ring buffer %s", dlt->shmemFile);
    }
    if (result == SAFU_RESULT_OK) {
        elosEbLogRingBuffer_t *head;
        safuLogDebugF("offset: 0x%lx", dlt->offsetAddress);
        dlt->shmemData =
            mmap(NULL, sizeof(elosEbLogRingBuffer_t), PROT_READ, MAP_SHARED, dlt->shmemFd, dlt->offsetAddress);
        head = dlt->shmemData;
        dlt->shmemLogEntries = (size_t)head->entryCount;
        dlt->shmemDataSize = ((size_t)head->entryCount * sizeof(elosEbLogEntry_t)) + sizeof(elosEbLogRingBuffer_t);
        dlt->idxRead = head->idxRead;
        if (dlt->idxRead >= dlt->shmemLogEntries) {
            safuLogErrF("Read pointer %u for dlt ring buffer outside of buffer[%lu]", dlt->idxRead,
                        dlt->shmemLogEntries);
            result = SAFU_RESULT_FAILED;
        } else if (dlt->bufferSize != 0 && dlt->shmemDataSize > dlt->bufferSize) {
            safuLogErrF("dlt log buffer size is bigger thant the shmem file (%lu/%ld)", dlt->shmemDataSize,
                        dlt->bufferSize);
            result = SAFU_RESULT_FAILED;
        }
        munmap(dlt->shmemData, sizeof(elosEbLogRingBuffer_t));
    }
    if (result == SAFU_RESULT_OK) {
        dlt->shmemData = mmap(NULL, dlt->shmemDataSize, PROT_READ, MAP_SHARED, dlt->shmemFd, dlt->offsetAddress);
        close(dlt->shmemFd);
        dlt->localBufferCopy = safuAllocMem(NULL, dlt->shmemDataSize);
        safuLogDebug("created memory for local copy");
        safuRingBufferParam_t rPar = {
            .deleteEntries = true, .deleteFunc = _deleteEbLogEntry, .elements = dlt->shmemLogEntries};
        safuRingBufferInitialize(&dlt->parserQueue, &rPar);
        safuLogDebug("created the parser queue");
    }
    return result;
}

void _cleanupHandler(UNUSED void *_) {
    // without this asan would find a problem with the call stack on pthread_cancel
    return;
}

void *elosDltScannerLoop(elosDltScanner_t *dlt) {
    int oldtype;
    pthread_cleanup_push(_cleanupHandler, NULL);
    int s = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    dlt->scannerThread = pthread_self();
    while (true) {
        s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if (s != 0) {
            safuLogErrErrno("pthread_setcancelstate");
            break;
        }
        memcpy(dlt->localBufferCopy, dlt->shmemData, dlt->shmemDataSize);
        safuResultE_t pubRes = elosQueueNextLogEntries(dlt->localBufferCopy, &dlt->idxRead, &dlt->parserQueue);
        if (pubRes == SAFU_RESULT_OK) {
            uint64_t elosBufferWritten = 1;
            ssize_t res = write(dlt->moreToRead, &elosBufferWritten, sizeof(elosBufferWritten));
            if (res != sizeof(elosBufferWritten)) {
                safuLogErr("failed to sync to publish thread");
                break;
            }
        } else if (pubRes != SAFU_RESULT_NOT_FOUND) {
            safuLogErr("failed to queue the next dlt entries");
            break;
        }
        s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (s != 0) {
            safuLogErrErrno("pthread_setcancelstate");
            break;
        }
        nanosleep(&dlt->sleepInterval, NULL);
    }
    pthread_cleanup_pop(NULL);
    return NULL;
}

safuResultE_t elosDltPublisherLoop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosDltScanner_t *dlt = plugin->data;

    struct pollfd fds[] = {
        {.fd = dlt->moreToRead, .events = POLLIN},
        {.fd = dlt->stopCmd, .events = POLLIN},
    };
    bool running = true;
    safuLogDebug("start run parser loop");
    while (result == SAFU_RESULT_OK && running) {
        int res = poll(fds, ARRAY_SIZE(fds), -1);
        if (res > 0) {
            safuLogDebugF("fds length: %lu", ARRAY_SIZE(fds));
            if (fds[0].revents & POLLIN) {
                uint64_t elosBufferWritten;
                ssize_t re = read(dlt->moreToRead, &elosBufferWritten, sizeof(elosBufferWritten));
                if (re != sizeof(elosBufferWritten)) {
                    safuLogErr("failed to read sync from scanner thread");
                    result = SAFU_RESULT_FAILED;
                    break;
                }
                elosPublishEntriesFromQueue(&dlt->parserQueue, plugin, dlt->publisher);
            } else if (fds[1].revents & POLLIN) {
                safuLogDebug("got Stop Command");
                running = false;
                break;
            } else {
                safuLogDebug("Something else entirely");
            }
        } else if (errno == EINTR) {
            safuLogDebug("continuing some error in poll");
            continue;
        } else {
            safuLogErrErrno("polling failed, terminating");
            result = SAFU_RESULT_FAILED;
            pthread_cancel(dlt->scannerThread);
            return SAFU_RESULT_FAILED;
        }
    }
    return result;
}

safuResultE_t elosDltScannerStopLoops(elosDltScanner_t *dlt) {
    safuResultE_t result = SAFU_RESULT_OK;

    safuLogDebug("Cancel the Scanner Thread");
    int s = pthread_cancel(dlt->scannerThread);
    safuLogDebug("done Cancel the Scanner Thread");
    if (s != 0) {
        safuLogErrErrno("pthread_cancel");
        result = SAFU_RESULT_FAILED;
    }
    safuLogDebug("send stop cmd");
    uint64_t stop = 0xf;
    ssize_t res = write(dlt->stopCmd, &stop, sizeof(stop));
    if (res != sizeof(stop)) {
        safuLogWarn("failed to send stop cmd");
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

safuResultE_t elosDltScannerFree(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosDltScanner_t *dlt = plugin->data;

    munmap(dlt->shmemData, dlt->shmemDataSize);
    close(dlt->moreToRead);
    close(dlt->stopCmd);
    free(dlt->localBufferCopy);
    safuRingBufferDeleteMembers(&dlt->parserQueue);
    dlt->localBufferCopy = NULL;
    result = elosPluginDeletePublisher(plugin, dlt->publisher);

    return result;
}
