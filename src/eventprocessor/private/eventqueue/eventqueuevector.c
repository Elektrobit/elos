// SPDX-License-Identifier: MIT

#include "eventprocessor/eventqueuevector.h"

#include <safu/log.h>
#include <stdlib.h>

safuResultE_t elosEventQueueVectorNew(elosEventQueueVector_t **eventQueueVector, uint32_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventQueueVector != NULL) {
        elosEventQueueVector_t *newEventQueueVector;

        newEventQueueVector = safuAllocMem(NULL, sizeof(elosEventQueueVector_t));
        if (newEventQueueVector != NULL) {
            result = elosEventQueueVectorInitialize(newEventQueueVector, elements);
            if (result != SAFU_RESULT_OK) {
                free(newEventQueueVector);
            } else {
                *eventQueueVector = newEventQueueVector;
            }
        }
    }

    return result;
}

safuResultE_t elosEventQueueVectorInitialize(elosEventQueueVector_t *eventQueueVector, uint32_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventQueueVector != NULL) {
        int retVal;

        retVal = safuVecCreate(eventQueueVector, elements, sizeof(elosEventQueue_t));
        if (retVal == 0) {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosEventQueueVectorPushByParam(elosEventQueueVector_t *eventQueueVector,
                                              elosEventQueueParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventQueueVector == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosEventQueue_t newEventQueue = {0};
        result = elosEventQueueInitialize(&newEventQueue, param);
        if (result == SAFU_RESULT_OK) {
            int retVal;
            retVal = safuVecPush(eventQueueVector, &newEventQueue);
            if (retVal < 0) {
                elosEventQueueDeleteMembers(&newEventQueue);
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

static int _matchElementById(const void *element, const void *data) {
    elosEventQueue_t const *eventQueue = (elosEventQueue_t const *)element;
    elosEventQueueId_t const id = *(elosEventQueueId_t const *)data;
    int result = 0;

    if (eventQueue->id == id) {
        result = 1;
    }

    return result;
}

safuResultE_t elosEventQueueVectorGetById(elosEventQueueVector_t *eventQueueVector, elosEventQueueId_t id,
                                          elosEventQueue_t **eventQueue) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventQueueVector == NULL) || (id == ELOS_EVENTQUEUE_ID_INVALID) || (eventQueue == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        uint32_t idx = 0;
        int retVal;

        retVal = safuVecFindGet(eventQueueVector, (void **)eventQueue, &idx, _matchElementById, &id);
        if (retVal < 0) {
            safuLogErr("safuVecFindGet failed");
        } else if (retVal == 0) {
            safuLogErrF("No EventQueue with id:%d found", id);
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosEventQueueVectorRemoveById(elosEventQueueVector_t *eventQueueVector, elosEventQueueId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventQueueVector == NULL) || (id == ELOS_EVENTQUEUE_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else {
        elosEventQueue_t *eventQueue = NULL;
        uint32_t idx = 0;
        int retVal;

        retVal = safuVecFindGet(eventQueueVector, (void **)&eventQueue, &idx, _matchElementById, &id);
        if (retVal == 1) {
            elosEventQueueDeleteMembers(eventQueue);
            retVal = safuVecRemove(eventQueueVector, idx);
            if (retVal == 0) {
                result = SAFU_RESULT_OK;
            }
        } else {
            safuLogErrF("EventQueueId '%u' not found", id);
        }
    }

    return result;
}

void elosEventQueueVectorDeleteMembers(elosEventQueueVector_t *eventQueueVector) {
    if (eventQueueVector != NULL) {
        int64_t i = safuVecElements(eventQueueVector);
        while (--i >= 0) {
            elosEventQueue_t *eventQueue;
            eventQueue = (elosEventQueue_t *)safuVecGet(eventQueueVector, i);
            if (eventQueue != NULL) {
                elosEventQueueDeleteMembers(eventQueue);
            }
            safuVecPop(eventQueueVector);
        }
        safuVecFree(eventQueueVector);
    }
}

void elosEventQueueVectorDelete(elosEventQueueVector_t *eventQueueVector) {
    if (eventQueueVector != NULL) {
        elosEventQueueVectorDeleteMembers(eventQueueVector);
        free(eventQueueVector);
    }
}
