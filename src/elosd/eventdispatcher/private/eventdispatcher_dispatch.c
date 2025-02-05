// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/vector.h>

#include "elos/event/event.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventprocessor/eventprocessor.h"

static safuResultE_t _eventPtrVectorDeleteMembers(elosEventPtrVector_t *eventPtrVector) {
    uint32_t const elements = safuVecElements(eventPtrVector);
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    for (uint32_t i = 0; i < elements; i += 1) {
        elosEvent_t **eventPtr;

        eventPtr = safuVecGet(eventPtrVector, i);
        if ((eventPtr == NULL) || (*eventPtr == NULL)) {
            result = SAFU_RESULT_FAILED;
        } else {
            result = elosEventDelete(*eventPtr);
            *eventPtr = NULL;
        }
    }

    retVal = safuVecFree(eventPtrVector);
    if (retVal < 0) {
        result = SAFU_RESULT_FAILED;
    }

    if (result != SAFU_RESULT_OK) {
        safuLogErr("Cleanup of EventPtrVector failed (likely resulting in a memory leak)");
    }

    return result;
}

static safuResultE_t _publishEventVector(elosEventDispatcher_t *eventDispatcher, elosEventPtrVector_t *eventPtrVector) {
    safuResultE_t result = SAFU_RESULT_OK;
    uint32_t const elements = safuVecElements(eventPtrVector);

    for (uint32_t i = 0; i < elements; i += 1) {
        safuResultE_t iterResult;
        elosEvent_t **eventPtr;

        eventPtr = safuVecGet(eventPtrVector, i);
        if ((eventPtr == NULL) || (*eventPtr == NULL)) {
            safuLogErrF("safuVecGet for event[%d] failed", i);
            result = SAFU_RESULT_FAILED;
        } else {
            iterResult = elosEventProcessorPublish(eventDispatcher->eventProcessor, *eventPtr);
            if (iterResult != SAFU_RESULT_OK) {
                safuLogErrF("Publishing event[%d] failed", i);
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

static safuResultE_t _publishEventBuffer(elosEventDispatcher_t *eventDispatcher, elosEventBuffer_t *eventBufferPtr,
                                         int32_t priority) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventPtrVector_t eventPtrVector = {0};
    size_t elementsWritten = 0;

    result = elosEventBufferRead(eventBufferPtr, priority, &eventPtrVector, &elementsWritten);
    if (result != SAFU_RESULT_OK) {
        safuLogErrF("elosEventBufferRead with priority:%d failed", priority);
    } else if (elementsWritten > 0) {
        eventDispatcher->worker.eventsPublished += elementsWritten;
        result = _publishEventVector(eventDispatcher, &eventPtrVector);
    }

    _eventPtrVectorDeleteMembers(&eventPtrVector);

    return result;
}

safuResultE_t elosEventDispatcherDispatch(elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventDispatcher == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
        safuLogErr("The given eventDispatcher is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eventDispatcher->lock, result);
        if (result == SAFU_RESULT_OK) {
            uint32_t const elements = safuVecElements(&eventDispatcher->eventBufferPtrVector);
            int32_t const priorityStart = ELOS_EVENTBUFFER_PRIORITY_COUNT - 1;

            for (int32_t priority = priorityStart; priority >= 0; priority -= 1) {
                for (uint32_t i = 0; i < elements; i += 1) {
                    safuResultE_t iterResult;
                    elosEventBuffer_t **eventBufferPtr;
                    eventBufferPtr = safuVecGet(&eventDispatcher->eventBufferPtrVector, i);
                    if (eventBufferPtr == NULL || *eventBufferPtr == NULL) {
                        safuLogErr("safuVecGet failed");
                        result = SAFU_RESULT_FAILED;
                    } else {
                        if ((*eventBufferPtr)->requestRemoval == true) {
                            (*eventBufferPtr)->permitRemoval = true;
                        }
                        iterResult = _publishEventBuffer(eventDispatcher, *eventBufferPtr, priority);
                        if (iterResult != SAFU_RESULT_OK) {
                            safuLogErrF("Dispatching eventBuffer[%d] failed (Events are probably lost)", i);
                            result = SAFU_RESULT_FAILED;
                        }
                    }
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
            pthread_cond_broadcast(&eventDispatcher->eventBufferRemoveCondition);
        }
    }

    return result;
}
