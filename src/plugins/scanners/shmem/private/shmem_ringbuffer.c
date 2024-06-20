// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include "shmem_ringbuffer.h"

#include <safu/log.h>
#include <samconf/samconf.h>
#include <stdio.h>

#include "elos/config/defines.h"
#include "elos/event/event.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "shmem.h"
#include "shmem_helpers.h"

static safuResultE_t _convertLogLevelToSeverity(int dltLogLevel, elosSeverityE_t *severity) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosSeverityE_t newSeverity = ELOS_SEVERITY_OFF;

    switch (dltLogLevel) {
        case ELOS_DLT_LOGLEVEL_OFF:
            break;
        case ELOS_DLT_LOGLEVEL_FATAL:
            newSeverity = ELOS_SEVERITY_FATAL;
            break;
        case ELOS_DLT_LOGLEVEL_ERROR:
            newSeverity = ELOS_SEVERITY_ERROR;
            break;
        case ELOS_DLT_LOGLEVEL_WARN:
            newSeverity = ELOS_SEVERITY_WARN;
            break;
        case ELOS_DLT_LOGLEVEL_INFO:
            newSeverity = ELOS_SEVERITY_INFO;
            break;
        case ELOS_DLT_LOGLEVEL_DEBUG:
            newSeverity = ELOS_SEVERITY_DEBUG;
            break;
        default:
            safuLogWarnF("eb_log_entry_type with logLevel '%d' can't be mapped to elosEvent severity", dltLogLevel);
            result = SAFU_RESULT_FAILED;
            break;
    }

    if (result == SAFU_RESULT_OK) {
        *severity = newSeverity;
    }

    return result;
}

static safuResultE_t _convertLogEntryToEvent(elosEbLogEntry_t const *entry, elosEvent_t *event) {
    safuResultE_t result;

    result = _convertLogLevelToSeverity(entry->logLevel, &event->severity);
    if (result == SAFU_RESULT_OK) {
        event->payload = strdup(entry->logString);
        if (event->payload == NULL) {
            safuLogErr("Duplicating logString failed");
            result = SAFU_RESULT_FAILED;
        } else {
            int retVal;

            // We have no information what creationTime exactly is, which means we'll change it down the road.
            // Due to this creationTime is now "seconds from epoch" for the time being (easiest to convert).
            event->date.tv_sec = entry->creationTime;
            event->date.tv_nsec = 0;

            // The producerId has no equivalent in an elosEvent. We'll stringify it into the appName field for now
            // so we don't lose the data, but this obviously needs to be amended in one way or another.
            retVal = asprintf(&event->source.appName, "producerId: %u", entry->producerId);
            if (retVal < 0) {
                safuLogErrErrnoValue("asprintf failed", retVal);
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

static inline safuResultE_t _checkRingBuffer(elosEbLogRingBuffer_t *ringBuffer) {
    safuResultE_t result = SAFU_RESULT_OK;
    uint16_t const entries = ringBuffer->entryCount;
    uint16_t const idxWrite = ringBuffer->idxWrite;
    uint16_t const idxRead = ringBuffer->idxRead;

    // These are somewhat complicated sanity checks that could be greatly simplified by using a int32_t
    // for the write index while replacing the read index with either a write counter or an overflow flag,
    // as they really are only two modes for reading a ringbuffer: From 0->idx-1 or from idx->idx-1 with wraparound.
    // We need more information on how the ringbuffer is supposed to work to prevent potential problems here.
    if ((entries == 0) || (entries >= SCANNER_SHMEM_IDX_INVALID)) {
        result = SAFU_RESULT_FAILED;
    } else if ((idxWrite == SCANNER_SHMEM_IDX_INVALID) && (idxWrite >= entries)) {
        result = SAFU_RESULT_FAILED;
    } else if (idxRead == SCANNER_SHMEM_IDX_INVALID) {
        if (idxWrite == 0) {
            result = SAFU_RESULT_NOT_FOUND;
        } else {
            result = SAFU_RESULT_FAILED;
        }
    } else if (idxRead > idxWrite) {
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

static safuResultE_t _readFromRingBuffer(elosScannerLegacySession_t *session) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosScannerLegacyCallbackData_t *cbData = &session->callback.scannerCallbackData;
    elosScannerContextShmem_t *context = session->context;
    elosEbLogRingBuffer_t *ringBuffer = context->shmemData;
    uint16_t const entryCount = ringBuffer->entryCount - 1;
    uint16_t const idxWrite = ringBuffer->idxWrite;
    uint16_t const idxRead = ringBuffer->idxRead;
    uint16_t idxStop = idxWrite;
    uint16_t idx = 0;

    if (idxRead == idxWrite) {  // If true the ringbuffer overflowed
        idx = idxRead;
        idxStop = idxRead;
    }

    do {
        elosEvent_t event = {0};

        result = _convertLogEntryToEvent(&ringBuffer->entries[idx], &event);
        if (result != SAFU_RESULT_OK) {
            break;
        }

        result = session->callback.eventPublish(cbData, &event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Publishing Event failed");
        }

        elosEventDeleteMembers(&event);

        idx += 1;
        if (idx >= entryCount) {
            idx = 0;
        }
    } while (idx != idxStop);

    return result;
}

safuResultE_t elosScannerRingBufferInitialize(elosScannerLegacySession_t *session) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosScannerContextShmem_t *context = session->context;
    elosEbLogRingBuffer_t *ringBuffer = context->shmemData;

    if (context->shmemCreate == true) {
        ringBuffer->entryCount = context->shmemLogEntries;
        ringBuffer->idxWrite = 0;
        ringBuffer->idxRead = SCANNER_SHMEM_IDX_INVALID;
    }

    return result;
}

safuResultE_t elosScannerRingBufferPublish(elosScannerLegacySession_t *session) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosScannerContextShmem_t *context = session->context;
    elosEbLogRingBuffer_t *ringBuffer = context->shmemData;

    SAFU_SEM_LOCK_WITH_RESULT(context->semData, result);
    if (result == SAFU_RESULT_OK) {
        unsigned short idxWr = ringBuffer->idxWrite;
        unsigned short idxRd = ringBuffer->idxRead;

        result = _checkRingBuffer(ringBuffer);
        if (result != SAFU_RESULT_OK) {
            if (result != SAFU_RESULT_NOT_FOUND) {
                char const errStr[] = "ringBuffer has invalid state (writeIdx: %u, readIdx: %u, entryCount: %u).";
                safuLogErrF(errStr, idxRd, idxWr, ringBuffer->entryCount);
            }
        } else {
            result = _readFromRingBuffer(session);

            ringBuffer->idxRead = SCANNER_SHMEM_IDX_INVALID;
            ringBuffer->idxWrite = 0;
        }
    }

    SAFU_SEM_UNLOCK_WITH_RESULT(context->semData, result);

    return result;
}
