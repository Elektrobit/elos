// SPDX-License-Identifier: MIT

#include "elos/eventbuffer/eventbuffer.h"

#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/ringbuffer.h>
#include <stdlib.h>

#include "elos/event/event_vector.h"

static safuResultE_t _deleteRings(elosEventBuffer_t *eventBuffer) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (eventBuffer->ring != NULL) {
        for (size_t i = 0; i < eventBuffer->ringCount; i += 1) {
            elosEventBufferRing_t *ring = &eventBuffer->ring[i];
            safuResultE_t iterResult;

            iterResult = safuRingBufferDeleteMembers(&ring->ringBuffer);
            if (iterResult != SAFU_RESULT_OK) {
                result = SAFU_RESULT_FAILED;
            }
        }

        free(eventBuffer->ring);
        eventBuffer->ring = NULL;
    }

    return result;
}

safuResultE_t elosEventBufferInitialize(elosEventBuffer_t *eventBuffer, elosEventBufferParam_t const *const param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventBuffer == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventBuffer->flags) == true) {
        safuLogErr("The given EventBuffer is already initialized");
    } else {
        size_t const defaultRingCount = ELOS_EVENTBUFFER_PRIORITY_COUNT;
        elosEventBufferRing_t *newRings = NULL;

        newRings = calloc(defaultRingCount, sizeof(elosEventBufferRing_t));
        if (newRings == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            uint32_t limitEventCount = param->limitEventCount;

            if (limitEventCount == 0) {
                limitEventCount = ELOS_EVENTBUFFER_DEFAULT_LIMIT;
            }

            eventBuffer->ringCount = defaultRingCount;
            eventBuffer->ring = newRings;

            for (size_t i = 0; i < eventBuffer->ringCount; i += 1) {
                elosEventBufferRing_t *ring = &eventBuffer->ring[i];
                safuRingBufferParam_t const ringParam = {
                    .deleteEntries = true,
                    .deleteFunc = (safuRingBufferEntryDeleteFunc_t *)elosEventDelete,
                    .elements = limitEventCount,
                };

                result = safuRingBufferInitialize(&ring->ringBuffer, &ringParam);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("Buffer initialization failed");
                    break;
                } else {
                    ring->limitEventCount = limitEventCount;
                }
            }

            if (result == SAFU_RESULT_FAILED) {
                _deleteRings(eventBuffer);
            } else {
                atomic_store(&eventBuffer->flags, SAFU_FLAG_INITIALIZED_BIT);
            }
        }
    }

    return result;
}

safuResultE_t elosEventBufferNew(elosEventBuffer_t **eventBuffer, elosEventBufferParam_t const *const param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventBuffer == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter given");
    } else {
        elosEventBuffer_t *newEventBuffer;

        newEventBuffer = calloc(1, sizeof(elosEventBuffer_t));
        if (newEventBuffer == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            result = elosEventBufferInitialize(newEventBuffer, param);
            if (result != SAFU_RESULT_OK) {
                free(newEventBuffer);
            } else {
                *eventBuffer = newEventBuffer;
            }
        }
    }

    return result;
}

safuResultE_t elosEventBufferRead(elosEventBuffer_t *eventBuffer, size_t priority, elosEventVector_t *eventVector,
                                  size_t *elementsWritten) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventBuffer == NULL) || (eventVector == NULL) || (priority >= eventBuffer->ringCount)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventBuffer->flags) == false) {
        safuLogErr("The given EventBuffer is not initialized");
    } else {
        elosEventBufferRing_t *ring = &eventBuffer->ring[priority];

        result = safuRingBufferRead(&ring->ringBuffer, eventVector, elementsWritten);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("Reading from EventBuffer (priority:%lu) failed", priority);
        }
    }

    return result;
}

safuResultE_t elosEventBufferWrite(elosEventBuffer_t *eventBuffer, elosEvent_t const *const event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventBuffer == NULL) || (event == NULL)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventBuffer->flags) == false) {
        safuLogErr("The given EventBuffer is not initialized");
    } else {
        size_t priority = ELOS_EVENTBUFFER_PRIORITY_NORMAL;
        elosEvent_t *clonedEvent = NULL;

        if (event->severity == ELOS_SEVERITY_FATAL) {
            priority = ELOS_EVENTBUFFER_PRIORITY_HIGH;
        }

        result = elosEventClone(&clonedEvent, event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Copying Event failed");
        } else {
            elosEventBufferRing_t *ring = &eventBuffer->ring[priority];

            result = safuRingBufferWrite(&ring->ringBuffer, clonedEvent);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("Writing to EventBuffer (priority:%lu) failed", priority);
                elosEventDelete(clonedEvent);
            }
        }
    }

    return result;
}

safuResultE_t elosEventBufferDeleteMembers(elosEventBuffer_t *eventBuffer) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventBuffer != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventBuffer->flags) == true) {
            if (eventBuffer->ring != NULL) {
                result = _deleteRings(eventBuffer);
                if (result != SAFU_RESULT_OK) {
                    safuLogWarn("EventBuffer cleanup failed (possible memory leak)");
                }

                free(eventBuffer->ring);
            }

            eventBuffer->ringCount = 0;
            eventBuffer->ring = NULL;
            atomic_fetch_and(&eventBuffer->flags, ~SAFU_FLAG_INITIALIZED_BIT);
        }
    }

    return result;
}

safuResultE_t elosEventBufferDelete(elosEventBuffer_t **eventBuffer) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventBuffer != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&(*eventBuffer)->flags) == true) {
            result = elosEventBufferDeleteMembers(*eventBuffer);
            free(*eventBuffer);
            *eventBuffer = NULL;
        }
    }

    return result;
}
