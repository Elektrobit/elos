// SPDX-License-Identifier: MIT

#include "elos/eventprocessor/eventqueuemanager.h"

#include <safu/log.h>
#include <stddef.h>

#include "elos/eventprocessor/defines.h"
#include "eventprocessor/eventqueuevector.h"
#include "safu/mutex.h"

safuResultE_t elosEventQueueManagerInitialize(elosEventQueueManager_t *eqm, elosEventQueueManagerParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eqm == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags)) {
        safuLogErr("The given eventQueueManager is already initialized");
    } else {
        SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(&eqm->mutex, NULL, result);
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT_IF_OK(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            eqm->idManager = NULL;
            eqm->idCount = 0;
            eqm->defaultEventQueueSize = ELOS_EVENTQUEUE_DEFAULT_SIZE;
            eqm->defaultLimitEvents = ELOS_EVENTQUEUE_DEFAULT_LIMITEVENTS;
            eqm->defaultLimitTime.tv_sec = ELOS_EVENTQUEUE_DEFAULT_LIMITTIME_SEC;
            eqm->defaultLimitTime.tv_nsec = ELOS_EVENTQUEUE_DEFAULT_LIMITTIME_NSEC;

            atomic_store(&eqm->flags, SAFU_FLAG_INITIALIZED_BIT);

            if (param != NULL) {
                eqm->defaultLimitEvents = param->defaultLimitEvents;
                eqm->defaultLimitTime = param->defaultLimitTime;
                if (param->defaultEventQueueSize != 0) {
                    eqm->defaultEventQueueSize = param->defaultEventQueueSize;
                }
            }

            result = elosEventQueueVectorInitialize(&eqm->eventQueueVector, ELOS_EVENTQUEUEMANAGER_DEFAULT_SIZE);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosEventQueueVectorInitialize failed");
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventQueueManagerDeleteMembers(elosEventQueueManager_t *eqm) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((eqm != NULL) && SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags)) {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            elosEventQueueVectorDeleteMembers(&eqm->eventQueueVector);
            atomic_store(&eqm->flags, SAFU_FLAG_NONE);
            eqm->defaultEventQueueSize = 0;
            eqm->defaultLimitEvents = 0;
            eqm->defaultLimitTime.tv_nsec = 0;
            eqm->defaultLimitTime.tv_sec = 0;
            eqm->idCount = 0;
            eqm->idManager = NULL;

            SAFU_PTHREAD_MUTEX_DESTROY(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventQueueManagerEntryNew(elosEventQueueManager_t *eqm, elosEventQueueId_t *eqId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eqm == NULL) || (eqId == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
        safuLogErr("The given eventQueueManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
                safuLogErr("The given eventQueueManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                elosEventQueueId_t const newId = eqm->idCount + 1;
                elosEventQueueParam_t param = {
                    .id = newId,
                    .limitEvents = eqm->defaultLimitEvents,
                    .limitTime = eqm->defaultLimitTime,
                    .elements = eqm->defaultEventQueueSize,
                };

                result = elosEventQueueVectorPushByParam(&eqm->eventQueueVector, &param);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventQueueVectorPushByParam failed");
                } else {
                    *eqId = newId;
                    eqm->idCount += 1;
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventQueueManagerEntryDelete(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((eqm == NULL) || (eqId == ELOS_EVENTQUEUE_ID_INVALID)) {
        safuLogErr("Invalid parameter");
        result = SAFU_RESULT_FAILED;
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
        safuLogErr("The given eventQueueManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
                safuLogErr("The given eventQueueManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                result = elosEventQueueVectorRemoveById(&eqm->eventQueueVector, eqId);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventQueueVectorRemoveById failed");
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventQueueManagerEntrySetLimit(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                                 UNUSED elosEventQueueLimitEvents_t limitEvents,
                                                 UNUSED elosEventQueueLimitTime_t limitTime) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eqm == NULL) || (eqId == ELOS_EVENTQUEUE_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
        safuLogErr("The given eventQueueManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
                safuLogErr("The given eventQueueManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                safuLogWarn("Function is not implemented yet");
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }
    return result;
}

safuResultE_t elosEventQueueManagerEntryGetLimit(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                                 UNUSED elosEventQueueLimitEvents_t *limitEvents,
                                                 UNUSED elosEventQueueLimitTime_t *limitTime) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eqm == NULL) || (eqId == ELOS_EVENTQUEUE_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
        safuLogErr("The given eventQueueManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
                safuLogErr("The given eventQueueManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                safuLogWarn("Function is not implemented yet");
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }
    return result;
}

safuResultE_t elosEventQueueManagerEntryGet(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                            elosEventQueue_t **eventQueue) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eqm == NULL) || (eqId == ELOS_EVENTQUEUE_ID_INVALID) || (eventQueue == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
        safuLogErr("The given eventQueueManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
                safuLogErr("The given eventQueueManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                result = elosEventQueueVectorGetById(&eqm->eventQueueVector, eqId, eventQueue);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventQueueVectorGetById failed");
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventQueueManagerEntryRead(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                             elosEventVector_t **eventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eqm == NULL) || (eqId == ELOS_EVENTQUEUE_ID_INVALID) || (eventVector == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
        safuLogErr("The given eventQueueManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
                safuLogErr("The given eventQueueManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                safuLogWarn("Function is not implemented yet");
            }
            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventQueueManagerEntryPush(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                             elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eqm == NULL) || (eqId == ELOS_EVENTQUEUE_ID_INVALID) || (event == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
        safuLogErr("The given eventQueueManager is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eqm->mutex, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eqm->flags) == false) {
                safuLogErr("The given eventQueueManager is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                safuLogWarn("Function is not implemented yet");
            }
            SAFU_PTHREAD_MUTEX_UNLOCK(&eqm->mutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}
