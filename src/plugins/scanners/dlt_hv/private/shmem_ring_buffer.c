// SPDX-License-Identifier: MIT

#include "shmem_ring_buffer.h"

#include <elos/event/event.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>
#include <stdlib.h>

#include "dlt_hv/types.h"
#include "dlt_scanner.h"
#include "elos/event/event_types.h"
#include "utils.h"

static safuResultE_t _queueEntry(elosEbLogRingBuffer_t *buff, uint16_t idxRead, safuRingBuffer_t *readQueue) {
    elosEbLogEntry_t *entry = safuAllocMem(NULL, sizeof(elosEbLogEntry_t));
    memcpy(entry, (void *)(&buff->entries[idxRead]), sizeof(elosEbLogEntry_t));
    return safuRingBufferWrite(readQueue, entry);
}

safuResultE_t elosQueueNextLogEntries(elosEbLogRingBuffer_t *buff, uint16_t *idxRead, safuRingBuffer_t *readQueue) {
    safuResultE_t result = SAFU_RESULT_OK;
    if (buff->idxWrite >= buff->entryCount) {
        safuLogErr("the Write index is outside the buffer");
        return SAFU_RESULT_FAILED;
    }

    if (buff->idxWrite > *idxRead) {
        for (; *idxRead < buff->idxWrite && result == SAFU_RESULT_OK; *idxRead += 1) {
            result = _queueEntry(buff, *idxRead, readQueue);
        }
    } else if (buff->idxWrite < *idxRead) {
        for (; *idxRead < buff->entryCount && result == SAFU_RESULT_OK; *idxRead += 1) {
            result = _queueEntry(buff, *idxRead, readQueue);
        }
        for (*idxRead = 0; *idxRead < buff->idxWrite && result == SAFU_RESULT_OK; *idxRead += 1) {
            result = _queueEntry(buff, *idxRead, readQueue);
        }
    } else {
        result = SAFU_RESULT_NOT_FOUND;
    }
    return result;
}

safuResultE_t elosPublishEntriesFromQueue(safuRingBuffer_t *readQueue, elosPlugin_t *plugin,
                                          struct elosPublisher *publisher) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosDltScanner_t *dlt = plugin->data;
    safuVec_t elemVec = {0};
    safuVecCreate(&elemVec, readQueue->elements, sizeof(elosEbLogEntry_t *));
    size_t numElems = 0;
    safuRingBufferRead(readQueue, &elemVec, &numElems);
    for (size_t i = 0; i < elemVec.elementCount; i += 1) {
        elosEbLogEntry_t **entry = safuVecGet(&elemVec, i);
        elosEvent_t event = {0};

        event.source.fileName = dlt->shmemFile;
        event.source.appName = plugin->config->key;
        event.messageCode = ELOS_DLT_MESSAGE_CODE;
        event.severity = ELOS_SEVERITY_DEBUG;
        elosEventFromLogEntry(*entry, &event);
        elosPublishAndStore(plugin, publisher, &event);

        readQueue->callback.deleteFunc((void *)*entry);
        free(event.payload);
        event.payload = NULL;
    }
    safuVecFree(&elemVec);
    return result;
}

safuResultE_t elosFreeEbLogEntry(elosEbLogEntry_t *entry) {
    free(entry);
    return SAFU_RESULT_OK;
}
