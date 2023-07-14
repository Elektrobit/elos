// SPDX-License-Identifier: MIT

#include "elos/eventdispatcher/eventdispatcher.h"

#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/vector.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "eventdispatcher_worker.h"

safuResultE_t elosEventDispatcherInitialize(elosEventDispatcher_t *eventDispatcher,
                                            elosEventDispatcherParam_t const *const param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventDispatcher == NULL) || (param == NULL) || (param->eventProcessor == NULL)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == true) {
        safuLogErr("The given eventDispatcher is already initialized");
    } else {
        SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(&eventDispatcher->lock, NULL, result);
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT_IF_OK(&eventDispatcher->lock, result);
        if (result == SAFU_RESULT_OK) {
            int eventFdWorkerSync;
            int eventFdSync;

            eventFdWorkerSync = eventfd(0, 0);
            if (eventFdWorkerSync < 0) {
                safuLogErrErrno("eventfd failed");
            } else {
                eventFdSync = eventfd(0, 0);
                if (eventFdSync < 0) {
                    safuLogErrErrno("eventfd failed");
                } else {
                    uint32_t const vectorSize = ELOS_EVENTDISPATCHER_DEFAULT_VECTOR_SIZE;
                    int retVal;

                    retVal = safuVecCreate(&eventDispatcher->eventBufferPtrVector, vectorSize, sizeof(void *));
                    if (retVal < 0) {
                        safuLogErr("safuVecCreate failed");
                        result = SAFU_RESULT_FAILED;
                    } else {
                        eventDispatcher->eventProcessor = param->eventProcessor;
                        eventDispatcher->worker.sync = eventFdWorkerSync;
                        eventDispatcher->sync = eventFdSync;
                        atomic_store(&eventDispatcher->flags, SAFU_FLAG_INITIALIZED_BIT);
                    }
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventDispatcherNew(elosEventDispatcher_t **eventDispatcher,
                                     elosEventDispatcherParam_t const *const param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventDispatcher == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter given");
    } else {
        elosEventDispatcher_t *newEventDispatcher;

        newEventDispatcher = safuAllocMem(NULL, sizeof(elosEventDispatcher_t));
        if (newEventDispatcher == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            memset(newEventDispatcher, 0, sizeof(elosEventDispatcher_t));

            result = elosEventDispatcherInitialize(newEventDispatcher, param);
            if (result != SAFU_RESULT_OK) {
                free(newEventDispatcher);
            } else {
                *eventDispatcher = newEventDispatcher;
            }
        }
    }

    return result;
}

safuResultE_t _eventDispatcherStop(elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    atomic_fetch_and(&eventDispatcher->flags, ~ELOS_EVENTDISPATCHER_FLAG_ACTIVE);
    retVal = eventfd_write(eventDispatcher->worker.sync, 1);
    if (retVal < 0) {
        safuLogErrErrno("eventfd_read failed");
        result = SAFU_RESULT_FAILED;
    } else {
        retVal = pthread_join(eventDispatcher->worker.thread, 0);
        if (retVal < 0) {
            safuLogErr("pthread_join failed");
            result = SAFU_RESULT_FAILED;
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosEventDispatcherDeleteMembers(elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (eventDispatcher != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == true) {
            SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eventDispatcher->lock, result);
            if (result == SAFU_RESULT_OK) {
                int retVal;

                if (ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&eventDispatcher->flags) == true) {
                    result = _eventDispatcherStop(eventDispatcher);
                }

                retVal = close(eventDispatcher->worker.sync);
                if (retVal < 0) {
                    safuLogErrErrno("close failed");
                    result = SAFU_RESULT_FAILED;
                }

                retVal = close(eventDispatcher->sync);
                if (retVal < 0) {
                    safuLogErrErrno("close failed");
                    result = SAFU_RESULT_FAILED;
                }

                retVal = safuVecFree(&eventDispatcher->eventBufferPtrVector);
                if (retVal < 0) {
                    safuLogErr("safuVecFree failed");
                    result = SAFU_RESULT_FAILED;
                }

                atomic_fetch_and(&eventDispatcher->flags, ~SAFU_FLAG_INITIALIZED_BIT);

                SAFU_PTHREAD_MUTEX_DESTROY(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
            }
        }
    }

    return result;
}

safuResultE_t elosEventDispatcherDelete(elosEventDispatcher_t **eventDispatcher) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventDispatcher != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&(*eventDispatcher)->flags) == true) {
            result = elosEventDispatcherDeleteMembers(*eventDispatcher);
            free(*eventDispatcher);
            *eventDispatcher = NULL;
        }
    }

    return result;
}

safuResultE_t elosEventDispatcherStart(elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventDispatcher == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
        safuLogErr("The given eventDispatcher is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eventDispatcher->lock, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
                safuLogErr("The given eventDispatcher is not initialized");
                result = SAFU_RESULT_FAILED;
            } else if (ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&eventDispatcher->flags) == true) {
                safuLogErr("The given eventDispatcher is active");
                result = SAFU_RESULT_FAILED;
            } else {
                int retVal;

                retVal = pthread_create(&eventDispatcher->worker.thread, 0, elosEventDispatcherWorker,
                                        (void *)eventDispatcher);
                if (retVal < 0) {
                    safuLogErrErrno("pthread_create failed");
                    result = SAFU_RESULT_FAILED;
                } else {
                    eventfd_t syncValue = 0;

                    retVal = eventfd_read(eventDispatcher->sync, &syncValue);
                    if (retVal < 0) {
                        safuLogErrErrno("eventfd_read failed");
                        result = SAFU_RESULT_FAILED;
                    } else if (ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&eventDispatcher->flags) == false) {
                        safuLogErr("eventDispatcher has no ACTIVE bit after starting the worker thread");
                        result = SAFU_RESULT_FAILED;
                    } else {
                        result = SAFU_RESULT_OK;
                    }
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventDispatcherStop(elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventDispatcher == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
        safuLogErr("The given eventDispatcher is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eventDispatcher->lock, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
                safuLogErr("The given eventDispatcher is not initialized");
                result = SAFU_RESULT_FAILED;
            } else if (ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&eventDispatcher->flags) == false) {
                safuLogErr("The given eventDispatcher is not active");
                result = SAFU_RESULT_FAILED;
            } else {
                result = _eventDispatcherStop(eventDispatcher);
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosEventDispatcherBufferAdd(elosEventDispatcher_t *eventDispatcher, elosEventBuffer_t *eventBuffer) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventDispatcher == NULL) || (eventBuffer == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
        safuLogErr("The given eventDispatcher is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eventDispatcher->lock, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
                safuLogErr("The given eventDispatcher is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                int retVal;

                retVal = safuVecPush(&eventDispatcher->eventBufferPtrVector, &eventBuffer);
                if (retVal < 0) {
                    safuLogErr("Adding the EventBuffer failed");
                    result = SAFU_RESULT_FAILED;
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

static int _matchByPointer(const void *element, const void *data) {
    void *elementPtr = *(void **)element;
    int result = 0;

    if (elementPtr == data) {
        result = 1;
    }

    return result;
}

safuResultE_t elosEventDispatcherBufferRemove(elosEventDispatcher_t *eventDispatcher, elosEventBuffer_t *eventBuffer) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventDispatcher == NULL) || (eventBuffer == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
        safuLogErr("The given eventDispatcher is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eventDispatcher->lock, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == false) {
                safuLogErr("The given eventDispatcher is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                int retVal;

                retVal = safuVecFindRemove(&eventDispatcher->eventBufferPtrVector, _matchByPointer, eventBuffer);
                if (retVal < 0) {
                    safuLogErr("Removing the EventBuffer failed");
                    result = SAFU_RESULT_FAILED;
                } else if (retVal == 0) {
                    safuLogErr("Could not find the given EventBuffer");
                    result = SAFU_RESULT_FAILED;
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}
