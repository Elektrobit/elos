// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventfiltermanager.h"

#include <safu/log.h>
#include <safu/mutex.h>
#include <stdio.h>

#include "elos/eventprocessor/eventqueuemanager.h"
#include "eventprocessor/eventfilternodevector.h"

safuResultE_t elosEventFilterManagerInitialize(elosEventFilterManager_t *efm,
                                               elosEventFilterManagerParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efm == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (param->eventQueueManager == NULL) {
        safuLogErr("Invalid values in 'param' struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags)) {
        safuLogErr("The given eventFilterManager is already initialized");
    } else {
        SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(&efm->mutex, NULL, result);
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT_IF_OK(&efm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            result = elosEventFilterNodeVectorInitialize(&efm->eventFilterNodeVector, param->elements);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosEventFilterNodeVectorInitialize failed");
                SAFU_PTHREAD_MUTEX_DESTROY(&efm->mutex, result = SAFU_RESULT_FAILED);
            } else {
                atomic_store(&efm->flags, SAFU_FLAG_INITIALIZED_BIT);
                efm->eventQueueManager = param->eventQueueManager;
                efm->idManager = efm->idManager;
                efm->idCount = 0;
                result = SAFU_RESULT_OK;
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&efm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventFilterManagerDeleteMembers(elosEventFilterManager_t *efm) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((efm != NULL) && SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags)) {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&efm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            result = elosEventFilterNodeVectorDeleteMembers(&efm->eventFilterNodeVector);
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("elosEventFilterNodeVectorDeleteMembers failed (possible memory leak)");
            }

            atomic_store(&efm->flags, SAFU_FLAG_NONE);
            efm->idCount = 0;
            efm->eventQueueManager = NULL;
            efm->idManager = NULL;

            SAFU_PTHREAD_MUTEX_DESTROY(&efm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventFilterManagerNodeCreate(elosEventFilterManager_t *efm, char const **filterStrings,
                                               size_t filterStringCount, elosEventQueueId_t eqId,
                                               elosEventFilterNodeId_t *efnId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efm == NULL) || (filterStrings == NULL) || (filterStringCount == 0) || (eqId == 0) || (efnId == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
        safuLogErr("The given eventFilterManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&efm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
                safuLogErr("The given eventFilterManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                elosEventFilterNodeParam_t param = {
                    .filterStrings = filterStrings,
                    .filterStringCount = filterStringCount,
                    .id = efm->idCount + 1,
                };

                result = elosEventQueueManagerEntryGet(efm->eventQueueManager, eqId, &param.eventQueue);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventQueueManagerEntryGet failed");
                } else {
                    result = elosEventFilterNodeVectorPushByParam(&efm->eventFilterNodeVector, &param);
                    if (result != SAFU_RESULT_OK) {
                        safuLogErr("elosEventFilterNodeVectorPushByParam failed");
                    } else {
                        efm->idCount += 1;
                        *efnId = param.id;
                    }
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&efm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventFilterManagerNodeRemove(elosEventFilterManager_t *efm, elosEventFilterNodeId_t efId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efm == NULL) || (efId == 0)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
        safuLogErr("The given eventFilterManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&efm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
                safuLogErr("The given eventFilterManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                result = elosEventFilterNodeVectorRemoveById(&efm->eventFilterNodeVector, efId);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventFilterNodeVectorRemoveById failed");
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&efm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

static int _removeByEventQueueIdHelper(const void *element, const void *data) {
    elosEventFilterNode_t *eventFilterNode = (elosEventFilterNode_t *)element;
    elosEventQueueId_t const eventQueueId = *(elosEventQueueId_t const *)data;
    elosEventQueue_t const *eventQueue = eventFilterNode->eventQueue;
    int result = 0;

    if ((eventQueue != NULL) && (eventQueue->id == eventQueueId)) {
        safuResultE_t resVal;

        resVal = elosEventFilterNodeDeleteMembers(eventFilterNode);
        if (resVal != SAFU_RESULT_OK) {
            safuLogWarn("elosEventFilterNodeDeleteMembers failed (possible memory leak)");
        }

        result = 1;
    }

    return result;
}

safuResultE_t elosEventFilterManagerNodeRemoveByEventQueueId(elosEventFilterManager_t *efm, elosEventQueueId_t eqId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efm == NULL) || (eqId == ELOS_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
        safuLogErr("The given eventFilterManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&efm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
                safuLogErr("The given eventFilterManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                int retVal;

                retVal = safuVecFindRemove(&efm->eventFilterNodeVector, _removeByEventQueueIdHelper, &eqId);
                if (retVal < 0) {
                    safuLogWarn("safuVecFindRemove failed (possible memory leak)");
                    result = SAFU_RESULT_FAILED;
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&efm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventFilterManagerProcess(elosEventFilterManager_t *efm, elosEvent_t const *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((efm == NULL) || (event == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
        safuLogErr("The given eventFilterManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&efm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&efm->flags) == false) {
                safuLogErr("The given eventFilterManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                result = elosEventFilterNodeVectorProcess(&efm->eventFilterNodeVector, NULL, event);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventFilterNodeVectorProcess failed!");
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&efm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}
