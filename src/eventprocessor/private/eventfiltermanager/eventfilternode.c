// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventfilternode.h"

#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/eventfilter/eventfilter.h"
#include "elos/eventfilter/vector.h"

safuResultE_t elosEventFilterNodeInitialize(elosEventFilterNode_t *efe, elosEventFilterNodeParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efe == NULL) || (param == NULL)) {
        safuLogErr("Null parameter given");
    } else if (efe->state != EVENTFILTERNODE_STATE_INVALID) {
        safuLogErrF("The given eventFilterNode is in state=%d instead of 'INVALID'", efe->state);
    } else {
        efe->mutex = safuAllocMem(NULL, sizeof(pthread_mutex_t));
        if (efe->mutex != NULL) {
            SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(efe->mutex, NULL, result);
            SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT_IF_OK(efe->mutex, result);
        }

        if (result == SAFU_RESULT_OK) {
            elosRpnFilterResultE_t filterResult;

            filterResult = elosEventFilterVectorInit(&efe->eventFilterVector, param->filterStringCount);
            if (filterResult != RPNFILTER_RESULT_OK) {
                safuLogErr("elosEventFilterInit failed");
            } else {
                efe->state = EVENTFILTERNODE_STATE_INITIALIZED;
                efe->eventQueue = param->eventQueue;
                efe->id = param->id;
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(efe->mutex, result = SAFU_RESULT_FAILED);
        }

        if ((result == SAFU_RESULT_OK) && (param->filterStringCount > 0)) {
            result = elosEventFilterNodePush(efe, param->filterStrings, param->filterStringCount);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosEventFilterNodePush failed");
            }
        }

        if (result != SAFU_RESULT_OK) {
            elosRpnFilterResultE_t filterResult = elosEventFilterVectorDeleteMembers(&efe->eventFilterVector);
            if (filterResult != RPNFILTER_RESULT_OK) {
                safuLogWarn("elosEventFilterDeleteMembers failed (possible memory leak)");
                result = SAFU_RESULT_FAILED;
            }

            efe->state = EVENTFILTERNODE_STATE_INVALID;
            efe->id = ELOS_ID_INVALID;
            efe->eventQueue = NULL;

            free(efe->mutex);
            efe->mutex = NULL;
        }
    }

    return result;
}

safuResultE_t elosEventFilterNodeDeleteMembers(elosEventFilterNode_t *eventFilterNode) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((eventFilterNode != NULL) && (eventFilterNode->state != EVENTFILTERNODE_STATE_INVALID)) {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(eventFilterNode->mutex, result);
        if (result == SAFU_RESULT_OK) {
            elosRpnFilterResultE_t filterResult;

            filterResult = elosEventFilterVectorDeleteMembers(&eventFilterNode->eventFilterVector);
            if (filterResult != RPNFILTER_RESULT_OK) {
                safuLogWarn("elosEventFilterDeleteMembers failed (possible memory leak)");
                result = SAFU_RESULT_FAILED;
            }

            eventFilterNode->state = EVENTFILTERNODE_STATE_INVALID;
            eventFilterNode->id = ELOS_ID_INVALID;
            eventFilterNode->eventQueue = NULL;

            SAFU_PTHREAD_MUTEX_DESTROY(eventFilterNode->mutex, result = SAFU_RESULT_FAILED);
            free(eventFilterNode->mutex);
        }
    }

    return result;
}

safuResultE_t elosEventFilterNodePush(elosEventFilterNode_t *efe, char const **filterStrings,
                                      size_t filterStringCount) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efe == NULL) || (filterStrings == NULL) || (filterStringCount == 0)) {
        safuLogErrF("%s", "Invalid parameter given");
    } else if (efe->state != EVENTFILTERNODE_STATE_INITIALIZED) {
        safuLogErrF("The given eventFilterNode is in state=%d instead of 'INITIALIZED'", efe->state);
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(efe->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (efe->state != EVENTFILTERNODE_STATE_INITIALIZED) {
                safuLogErrF("The given eventFilterNode is in state=%d instead of 'INITIALIZED'", efe->state);
                result = SAFU_RESULT_OK;
            } else {
                for (size_t i = 0; i < filterStringCount; i += 1) {
                    char const *filterString = filterStrings[i];
                    if (filterString == NULL) {
                        safuLogErrF("filterString idx:%lu is NULL", i);
                        result = SAFU_RESULT_FAILED;
                        break;
                    } else {
                        elosEventFilterParam_t const eventFilterParam = {.filterString = filterString};
                        elosRpnFilterResultE_t filterResult;
                        elosEventFilter_t eventFilter = {0};

                        filterResult = elosEventFilterCreate(&eventFilter, &eventFilterParam);
                        if (filterResult != RPNFILTER_RESULT_OK) {
                            safuLogErrF("%s", "elosEventFilterCreate failed");
                            result = SAFU_RESULT_FAILED;
                            break;
                        } else {
                            filterResult = elosEventFilterVectorPush(&efe->eventFilterVector, &eventFilter);
                            if (filterResult != RPNFILTER_RESULT_OK) {
                                safuLogErrF("%s", "elosEventFilterVectorPush failed");
                                result = SAFU_RESULT_FAILED;
                                break;
                            }
                        }
                    }
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(efe->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

typedef struct _processData {
    elosEventFilterStack_t const *filterStack;
    elosEvent_t const *event;
    elosEventQueue_t *eventQueue;
} _processData_t;

static int _processFunc(const void *element, const void *data) {
    elosEventFilter_t const *eventFilter = (elosEventFilter_t const *)element;
    _processData_t *processData = (_processData_t *)data;
    elosRpnFilterResultE_t retVal;
    int result = 0;

    safuLogDebugF("check queue %u", processData->eventQueue->id);
    retVal = elosEventFilterExecute(eventFilter, processData->filterStack, (elosEvent_t *)processData->event);
    if (retVal == RPNFILTER_RESULT_MATCH) {
        safuResultE_t resVal;

        safuLogDebugF("add to queue %u", processData->eventQueue->id);
        resVal = elosEventQueuePush(processData->eventQueue, processData->event);
        if (resVal != SAFU_RESULT_OK) {
            safuLogErr("elosEventQueuePush failed");
            result = 1;
        }
    } else if (retVal != RPNFILTER_RESULT_NO_MATCH) {
        safuLogErr("elosEventFilterExecute failed");
        result = 1;
    }

    return result;
}

safuResultE_t elosEventFilterNodeProcess(elosEventFilterNode_t *efe, elosEventFilterStack_t const *filterStack,
                                         elosEvent_t const *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efe == NULL) || (efe->eventQueue == NULL) || (event == NULL)) {
        safuLogErr("Null parameter given");
    } else if (efe->state != EVENTFILTERNODE_STATE_INITIALIZED) {
        safuLogErrF("The given eventFilterNode is in state=%d instead of 'INITIALIZED'", efe->state);
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(efe->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (efe->state != EVENTFILTERNODE_STATE_INITIALIZED) {
                safuLogErrF("The given eventFilterNode is in state=%d instead of 'INITIALIZED'", efe->state);
                result = SAFU_RESULT_FAILED;
            } else {
                int retVal;
                _processData_t processData = {
                    .filterStack = filterStack,
                    .event = event,
                    .eventQueue = efe->eventQueue,
                };

                retVal = safuVecIterate(&efe->eventFilterVector, _processFunc, (void const *)&processData);
                if ((retVal < 0) || (retVal == 1)) {
                    safuLogWarn("safuVecIterate failed (events might be lost)");
                    result = SAFU_RESULT_FAILED;
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(efe->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}
