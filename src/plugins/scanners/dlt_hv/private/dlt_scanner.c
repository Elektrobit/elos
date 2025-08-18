// SPDX-License-Identifier: MIT

#include "dlt_scanner.h"

#include <elos/libelosplugin/libelosplugin.h>
#include <fcntl.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <unistd.h>

#include "dlt_hv/scanner.h"
#include "dlt_hv/types.h"
#include "utils.h"

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
            result = elosPluginCreatePublisher(plugin, &dlt->publisher);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("failed to create publisher for \"%s\"", plugin->config->key);
            } else {
                dlt->sleepInterval.tv_sec = 5;
                dlt->sleepInterval.tv_nsec = 0;
                dlt->moreToRead = eventfd(0, 0);
                dlt->stopCmd = eventfd(0, 0);
                dlt->bufferIdx = 0;
                plugin->data = dlt;
            }
        }
    }
    if (result != SAFU_RESULT_OK) {
        free(dlt);
        dlt = NULL;
    }
    return result;
}

safuResultE_t elosDltScannerOpenBuffer(elosDltScanner_t *dlt) {
    safuResultE_t result = SAFU_RESULT_OK;
    dlt->shmemFd = shm_open(dlt->shmemFile, O_RDONLY, 0);
    if (dlt->shmemFd == -1) {
        result = SAFU_RESULT_FAILED;
    }
    if (result == SAFU_RESULT_OK) {
        elosEbLogRingBuffer_t *head;
        dlt->shmemData = mmap(NULL, sizeof(elosEbLogRingBuffer_t), PROT_READ, MAP_SHARED, dlt->shmemFd, 0);
        head = dlt->shmemData;
        dlt->shmemDataSize = ((size_t)head->entryCount * sizeof(elosEbLogEntry_t)) + sizeof(elosEbLogRingBuffer_t);
        safuLogDebugF("buffer: %lu (header) %u (entry count) * %lu (entry size) = %lu", sizeof(elosEbLogRingBuffer_t),
                      head->entryCount, sizeof(elosEbLogEntry_t), dlt->shmemDataSize);
        safuLogDebugF("DLT buffer \"%s\" size: %lu (%u) => %lu", dlt->shmemFile, dlt->shmemDataSize, head->entryCount,
                      dlt->shmemDataSize);
        dlt->shmemData = mmap(NULL, dlt->shmemDataSize, PROT_READ, MAP_SHARED, dlt->shmemFd, 0);
        close(dlt->shmemFd);
        for (size_t i = 0; i < NUM_BUFFER; i += 1) {
            dlt->localData[i] = safuAllocMem(NULL, dlt->shmemDataSize);
            safuLogDebugF("Allocating local buffer %lu with size %lu => %p", i, dlt->shmemDataSize, dlt->localData[i]);
        }
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
        size_t idx = dlt->bufferIdx;
        safuLogDebugF("copying %lu bytes from shmem to local buffer %lu", dlt->shmemDataSize, idx);
        memcpy(dlt->localData[idx], dlt->shmemData, dlt->shmemDataSize);
        uint64_t elosBufferWritten = 1;
        dlt->bufferIdx = (idx + 1) % NUM_BUFFER;
        ssize_t res = write(dlt->moreToRead, &elosBufferWritten, sizeof(elosBufferWritten));
        if (res != sizeof(elosBufferWritten)) {
            safuLogErr("failed to sync to publish thread");
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

static safuResultE_t _publishDltBuffer(elosPlugin_t *plugin, size_t idx) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosDltScanner_t *dlt = plugin->data;

    elosEbLogRingBuffer_t *logBuffer = dlt->localData[idx];
    elosEvent_t event = {0};
    event.source.fileName = dlt->shmemFile;
    event.source.appName = plugin->config->key;
    char buffer[200];
    snprintf(buffer, 200, "DLT Buffer: { .idxWrite = %u, .idxRead = %u, .entryCount = %u }", logBuffer->idxWrite,
             logBuffer->idxRead, logBuffer->entryCount);
    event.payload = buffer;
    event.messageCode = 2042;
    event.severity = ELOS_SEVERITY_DEBUG;
    result = elosPublishAndStore(plugin, dlt->publisher, &event);
    for (size_t i = 0; i < logBuffer->entryCount && result == SAFU_RESULT_OK; ++i) {
        elosEbLogEntry_t *logEntry = &logBuffer->entries[i];
        elosEventFromLogEntry(logEntry, &event);
        result = elosPublishAndStore(plugin, dlt->publisher, &event);
    }
    return result;
}

static long elosPublishCounter = 0;

static safuResultE_t _publishNextDltBuffer(elosPlugin_t *plugin, uint64_t behind) {
    elosPublishCounter += 1;
    safuResultE_t result = SAFU_RESULT_OK;
    elosDltScanner_t *dlt = plugin->data;
    size_t idx;
    if (behind >= NUM_BUFFER) {
        idx = (dlt->bufferIdx + 1) % NUM_BUFFER;
        behind = NUM_BUFFER;
    } else {
        idx = (dlt->bufferIdx + NUM_BUFFER - behind) % NUM_BUFFER;
    }
    for (size_t i = 0; i < behind; i += 1) {
        safuLogDebugF("publishing (%ld)", elosPublishCounter);
        safuResultE_t res = _publishDltBuffer(plugin, idx);
        if (res != SAFU_RESULT_OK) {
            result = res;
        }
        idx = (idx + 1) % NUM_BUFFER;
    }
    return result;
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
                _publishNextDltBuffer(plugin, elosBufferWritten);
            } else if (fds[1].revents & POLLIN) {
                safuLogDebug("received Stop Command");
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
    for (size_t i = 0; i < NUM_BUFFER; i += 1) {
        free(dlt->localData[i]);
        dlt->localData[i] = NULL;
    }
    result = elosPluginDeletePublisher(plugin, dlt->publisher);

    return result;
}
