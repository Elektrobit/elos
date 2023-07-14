// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventqueue.h"

#include <safu/common.h>
#include <stdlib.h>

#include "elos/event/event_vector.h"
#include "safu/mutex.h"

safuResultE_t elosEventQueueNew(elosEventQueue_t **eventQueue, elosEventQueueParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventQueue_t *newEventQueue;

    if (eventQueue == NULL) {
        safuLogErr("Invalid parameter");
        return result;
    }

    newEventQueue = safuAllocMem(NULL, sizeof(elosEventQueue_t));
    if (newEventQueue == NULL) {
        safuLogErr("safuAllocMem failed");
        return result;
    }

    result = elosEventQueueInitialize(newEventQueue, param);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("elosEventQueueInitialize failed");
        free(newEventQueue);
    } else {
        *eventQueue = newEventQueue;
    }

    return result;
}

safuResultE_t elosEventQueueInitialize(elosEventQueue_t *eventQueue, elosEventQueueParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventQueue != NULL) && (param != NULL)) {
        int retVal;

        retVal = elosEventVectorNew(&eventQueue->eventVector, param->elements);
        if (retVal == 0) {
            eventQueue->mutex = safuAllocMem(NULL, sizeof(pthread_mutex_t));
            if (eventQueue->mutex == NULL) {
                safuLogErr("safuAllocMem for mutex failed");
            } else {
                retVal = pthread_mutex_init(eventQueue->mutex, NULL);
                if (retVal == 0) {
                    eventQueue->id = param->id;
                    eventQueue->limitEvents = param->limitEvents;
                    eventQueue->limitTime = param->limitTime;
                    eventQueue->defaultElements = param->elements;
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }

    return result;
}

void elosEventQueueDeleteMembers(elosEventQueue_t *eventQueue) {
    if (eventQueue != NULL) {
        SAFU_PTHREAD_MUTEX_LOCK(eventQueue->mutex, return );
        elosEventVectorDelete(eventQueue->eventVector);
        SAFU_PTHREAD_MUTEX_DESTROY(eventQueue->mutex, return );
        free(eventQueue->mutex);
    }
}

void elosEventQueueDelete(elosEventQueue_t *eventQueue) {
    if (eventQueue != NULL) {
        elosEventQueueDeleteMembers(eventQueue);
        free(eventQueue);
    }
}

safuResultE_t elosEventQueuePush(elosEventQueue_t *eventQueue, elosEvent_t const *event) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((eventQueue == NULL) || (eventQueue->eventVector == NULL) || (event == NULL)) {
        result = SAFU_RESULT_FAILED;
    } else {
        elosEvent_t newEvent = {0};

        SAFU_PTHREAD_MUTEX_LOCK(eventQueue->mutex, result = SAFU_RESULT_FAILED);
        if (result == SAFU_RESULT_OK) {
            result = elosEventDeepCopy(&newEvent, event);
        }

        if (result == SAFU_RESULT_OK) {
            int retVal = safuVecPush(eventQueue->eventVector, &newEvent);
            if (retVal < 0) {
                result = SAFU_RESULT_FAILED;
            }
        }
        SAFU_PTHREAD_MUTEX_UNLOCK(eventQueue->mutex, result = SAFU_RESULT_FAILED);
    }

    return result;
}

safuResultE_t elosEventQueueRead(elosEventQueue_t *eventQueue, elosEventVector_t **eventVector) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((eventQueue == NULL) || (eventQueue->eventVector == NULL) || (eventVector == NULL)) {
        result = SAFU_RESULT_FAILED;
    } else {
        SAFU_PTHREAD_MUTEX_LOCK(eventQueue->mutex, result = SAFU_RESULT_FAILED);
        if (result == SAFU_RESULT_OK) {
            elosEventVector_t *newEventVector = NULL;
            uint32_t elements = 0;

            elements = safuVecElements(eventQueue->eventVector);
            if (elements < 1) {
                *eventVector = NULL;
            } else {
                result = elosEventVectorNew(&newEventVector, eventQueue->defaultElements);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventVectorNew failed");
                } else {
                    *eventVector = eventQueue->eventVector;
                    eventQueue->eventVector = newEventVector;
                }
            }
        }
        SAFU_PTHREAD_MUTEX_UNLOCK(eventQueue->mutex, result = SAFU_RESULT_FAILED);
    }

    return result;
}
