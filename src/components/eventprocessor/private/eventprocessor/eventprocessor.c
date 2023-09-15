// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventprocessor.h"

#include <safu/log.h>

#include "elos/eventprocessor/defines.h"
#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventqueue.h"
#include "elos/eventprocessor/eventqueuemanager.h"

safuResultE_t elosEventProcessorDeleteMembers(elosEventProcessor_t *eventProcessor) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (eventProcessor != NULL) {
        safuResultE_t funcResult;

        funcResult = elosEventFilterManagerDeleteMembers(&eventProcessor->eventFilterManager);
        if (funcResult != SAFU_RESULT_OK) {
            safuLogWarn("elosEventFilterManagerDeleteMembers failed (possible memory leak)");
            result = SAFU_RESULT_FAILED;
        }

        funcResult = elosEventQueueManagerDeleteMembers(&eventProcessor->eventQueueManager);
        if (funcResult != SAFU_RESULT_OK) {
            safuLogWarn("elosEventQueueManagerDeleteMembers failed (possible memory leak)");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosEventProcessorInitialize(elosEventProcessor_t *eventProcessor,
                                           elosEventProcessorParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventProcessor == NULL) || (param == NULL)) {
        safuLogErr("Null parameter given");
    } else if (param->config == NULL) {
        safuLogErr("'param' struct has invalid values");
    } else {
        // Note: param=NULL is a temporary solution, the struct should be filled by config values in the long run
        elosEventQueueManagerParam_t const *queueManagerParam = NULL;
        result = elosEventQueueManagerInitialize(&eventProcessor->eventQueueManager, queueManagerParam);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosEventQueueManagerInitialize failed");
        }

        if (result == SAFU_RESULT_OK) {
            elosEventFilterManagerParam_t const filterManagerParam = {
                .eventQueueManager = &eventProcessor->eventQueueManager,
                .idManager = NULL,
                .elements = ELOS_EVENTFILTERNODEVECTOR_DEFAULT_SIZE,  // Will also be pulled from config in the future
            };

            result = elosEventFilterManagerInitialize(&eventProcessor->eventFilterManager, &filterManagerParam);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosEventFilterManagerInitialize failed");
            }
        }

        if (result != SAFU_RESULT_OK) {
            elosEventProcessorDeleteMembers(eventProcessor);
        }
    }

    return result;
}

safuResultE_t elosEventProcessorFilterNodeCreateWithQueue(elosEventProcessor_t *eventProcessor,
                                                          char const **filterStrings, size_t filterStringCount,
                                                          elosEventQueueId_t *eventQueueId,
                                                          elosEventFilterNodeId_t *eventFilterNodeId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventProcessor == NULL) || (eventQueueId == NULL) || (eventFilterNodeId == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosEventFilterNodeId_t newEventFilterNodeId = ELOS_ID_INVALID;
        elosEventQueueId_t newEventQueueId = ELOS_ID_INVALID;

        result = elosEventQueueManagerEntryNew(&eventProcessor->eventQueueManager, &newEventQueueId);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventQueueManagerEntryNew failed");
        } else {
            result = elosEventFilterManagerNodeCreate(&eventProcessor->eventFilterManager, filterStrings,
                                                      filterStringCount, newEventQueueId, &newEventFilterNodeId);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosEventFilterManagerNodeCreate failed");
                elosEventQueueManagerEntryDelete(&eventProcessor->eventQueueManager, newEventQueueId);
            } else {
                *eventFilterNodeId = newEventFilterNodeId;
                *eventQueueId = newEventQueueId;
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}

safuResultE_t elosEventProcessorFilterNodeCreate(elosEventProcessor_t *eventProcessor, char const **filterStrings,
                                                 size_t filterStringCount, elosEventQueueId_t eventQueueId,
                                                 elosEventFilterNodeId_t *eventFilterNodeId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventProcessor == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        result = elosEventFilterManagerNodeCreate(&eventProcessor->eventFilterManager, filterStrings, filterStringCount,
                                                  eventQueueId, eventFilterNodeId);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventFilterManagerNodeCreate failed");
        }
    }

    return result;
}

safuResultE_t elosEventProcessorFilterNodeRemove(elosEventProcessor_t *eventProcessor,
                                                 elosEventFilterNodeId_t eventFilterNodeId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventProcessor == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        result = elosEventFilterManagerNodeRemove(&eventProcessor->eventFilterManager, eventFilterNodeId);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventFilterManagerNodeRemove failed");
        }
    }

    return result;
}

safuResultE_t elosEventProcessorQueueCreate(elosEventProcessor_t *eventProcessor, elosEventQueueId_t *eventQueueId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventProcessor == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        result = elosEventQueueManagerEntryNew(&eventProcessor->eventQueueManager, eventQueueId);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventQueueManagerEntryNew failed");
        }
    }

    return result;
}

safuResultE_t elosEventProcessorQueueRemove(elosEventProcessor_t *eventProcessor, elosEventQueueId_t eventQueueId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventProcessor == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        safuResultE_t resVal;

        result = elosEventFilterManagerNodeRemoveByEventQueueId(&eventProcessor->eventFilterManager, eventQueueId);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosEventFilterManagerNodeRemoveByEventQueueId failed (possible memory leak)");
        }

        resVal = elosEventQueueManagerEntryDelete(&eventProcessor->eventQueueManager, eventQueueId);
        if (resVal != SAFU_RESULT_OK) {
            safuLogErr("elosEventQueueManagerEntryDelete failed");
            result = resVal;
        }
    }

    return result;
}

safuResultE_t elosEventProcessorQueueRead(elosEventProcessor_t *eventProcessor, elosEventQueueId_t eventQueueId,
                                          elosEventVector_t **eventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventProcessor == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        elosEventQueue_t *eventQueue = NULL;

        result = elosEventQueueManagerEntryGet(&eventProcessor->eventQueueManager, eventQueueId, &eventQueue);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosEventQueueManagerEntryGet failed");
        } else {
            result = elosEventQueueRead(eventQueue, eventVector);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosEventQueueRead failed");
            }
        }
    }

    return result;
}

safuResultE_t elosEventProcessorPublish(elosEventProcessor_t *eventProcessor, elosEvent_t const *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventProcessor == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        result = elosEventFilterManagerProcess(&eventProcessor->eventFilterManager, event);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventFilterManagerProcess failed");
        }
    }

    return result;
}
