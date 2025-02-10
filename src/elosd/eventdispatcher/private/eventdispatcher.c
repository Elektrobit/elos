// SPDX-License-Identifier: MIT

// clang-format off
#define _GNU_SOURCE
#include <pthread.h>
// clang-format on

#include "elos/eventdispatcher/eventdispatcher.h"

#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/vector.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/eventbuffer/eventbuffer.h"
#include "eventdispatcher_private.h"

safuResultE_t elosEventDispatcherInitialize(elosEventDispatcher_t *eventDispatcher,
                                            elosEventDispatcherParam_t const *const param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventDispatcher == NULL) || (param == NULL) || (param->eventProcessor == NULL)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == true) {
        safuLogErr("The given eventDispatcher is already initialized");
    } else {
        SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(&eventDispatcher->lock, NULL, result);
        if (result == SAFU_RESULT_OK) {
            SAFU_PTHREAD_COND_INIT_WITH_RESULT(&eventDispatcher->eventBufferRemoveCondition, NULL, result);
            SAFU_PTHREAD_COND_INIT_WITH_RESULT(&eventDispatcher->eventVectorRemoveCondition, NULL, result);
        }
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT_IF_OK(&eventDispatcher->lock, result);
        if (result == SAFU_RESULT_OK) {
            int eventFdWorkerTrigger;
            int eventFdSync;

            eventFdWorkerTrigger = eventfd(0, 0);
            if (eventFdWorkerTrigger < 0) {
                safuLogErrErrno("eventfd failed");
            } else {
                eventFdSync = eventfd(0, 0);
                if (eventFdSync < 0) {
                    safuLogErrErrno("eventfd failed");
                } else {
                    uint32_t const vectorSize = ELOS_EVENTDISPATCHER_DEFAULT_VECTOR_SIZE;
                    int retVal;

                    retVal =
                        safuVecCreate(&eventDispatcher->eventBufferPtrVector, vectorSize, sizeof(elosEventBuffer_t *));
                    if (retVal < 0) {
                        safuLogErr("safuVecCreate failed");
                        result = SAFU_RESULT_FAILED;
                    } else {
                        eventDispatcher->eventProcessor = param->eventProcessor;
                        eventDispatcher->worker.trigger = eventFdWorkerTrigger;
                        eventDispatcher->worker.pollTimeout = ELOS_EVENTDISPATCHER_DEFAULT_POLL_TIMEOUT;
                        eventDispatcher->worker.healthTimeInterval = ELOS_EVENTDISPATCHER_DEFAULT_HEALTH_INTERVAL;
                        eventDispatcher->worker.healthTimeTreshold = (struct timespec){0};
                        eventDispatcher->worker.eventsPublished = 0;
                        eventDispatcher->sync = eventFdSync;

                        if (param->pollTimeout != NULL) {
                            eventDispatcher->worker.pollTimeout = *param->pollTimeout;
                        }

                        if (param->healthTimeInterval != NULL) {
                            eventDispatcher->worker.healthTimeInterval = *param->healthTimeInterval;
                        }

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

safuResultE_t elosEventDispatcherDeleteMembers(elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (eventDispatcher != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&eventDispatcher->flags) == true) {
            if (ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&eventDispatcher->flags) == true) {
                result = elosEventDispatcherStop(eventDispatcher);
                if (result != SAFU_RESULT_OK) {
                    safuLogWarn("elosEventDispatcherStop failed");
                }
            }

            SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&eventDispatcher->lock, result);
            if (result == SAFU_RESULT_OK) {
                struct timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                ts.tv_sec += 1;
                while (safuVecElements(&eventDispatcher->eventBufferPtrVector) != 0) {
                    int rc = pthread_cond_timedwait(&eventDispatcher->eventVectorRemoveCondition,
                                                    &eventDispatcher->lock, &ts);
                    if (rc == ETIMEDOUT) {
                        break;
                    }
                }
                u_int32_t elements = safuVecElements(&eventDispatcher->eventBufferPtrVector);
                if (elements != 0) {
                    safuLogErrF("eventBufferPtrVector still has %d elements before removal", elements);
                }

                int retVal = close(eventDispatcher->worker.trigger);
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
                SAFU_PTHREAD_COND_DESTROY(&eventDispatcher->eventBufferRemoveCondition);
                SAFU_PTHREAD_COND_DESTROY(&eventDispatcher->eventVectorRemoveCondition);
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
                    retVal = pthread_setname_np(eventDispatcher->worker.thread, "EventDispatcher");
                    if (retVal != 0) {
                        safuLogErr("Failed to set thread name for event dispatcher");
                    }

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
        int retVal;

        atomic_fetch_and(&eventDispatcher->flags, ~ELOS_EVENTDISPATCHER_FLAG_ACTIVE);
        retVal = eventfd_write(eventDispatcher->worker.trigger, 1);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_read failed");
            result = SAFU_RESULT_FAILED;
        } else {
            retVal = pthread_join(eventDispatcher->worker.thread, 0);
            if (retVal < 0) {
                safuLogErr("pthread_join failed");
                result = SAFU_RESULT_FAILED;
            } else {
                result = elosEventDispatcherDispatch(eventDispatcher);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("Dispatching Events failed!");
                }
            }
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
            result = elosEventBufferSetWriteTrigger(eventBuffer, eventDispatcher->worker.trigger);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("Setting the EventBuffer trigger failed");
            } else {
                eventBuffer->requestRemoval = false;
                eventBuffer->permitRemoval = false;

                int retVal = safuVecPush(&eventDispatcher->eventBufferPtrVector, &eventBuffer);
                ADDITIONAL_DISPATCHER_DEBUGS("Add eventbuffer %p to vector. Current elements of vector: %d",
                                             (void *)eventBuffer, eventDispatcher->eventBufferPtrVector.elementCount);
                if (retVal < 0) {
                    safuLogErr("Adding the EventBuffer failed");
                    result = elosEventBufferSetWriteTrigger(eventBuffer, ELOS_EVENTBUFFER_NO_TRIGGER);
                    if (result != SAFU_RESULT_OK) {
                        safuLogErr("Disabling the EventBuffer trigger failed");
                    } else {
                        result = SAFU_RESULT_FAILED;
                    }
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
            eventBuffer->requestRemoval = true;
            while (safuVecElements(&eventDispatcher->eventBufferPtrVector) != 0 &&
                   eventBuffer->permitRemoval == false) {
                pthread_cond_wait(&eventDispatcher->eventBufferRemoveCondition, &eventDispatcher->lock);
            }
            ADDITIONAL_DISPATCHER_DEBUGS("Remove eventbuffer %p from vector. Current elements of vector: %d",
                                         (void *)eventBuffer, eventDispatcher->eventBufferPtrVector.elementCount);

            int retVal = safuVecFindRemove(&eventDispatcher->eventBufferPtrVector, _matchByPointer, eventBuffer);

            if (retVal < 0) {
                safuLogErr("Removing the EventBuffer failed");
                result = SAFU_RESULT_FAILED;
            } else if (retVal == 0) {
                safuLogErr("Could not find the given EventBuffer");
                result = SAFU_RESULT_FAILED;
            } else {
                result = elosEventBufferSetWriteTrigger(eventBuffer, ELOS_EVENTBUFFER_NO_TRIGGER);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("Disabling the EventBuffer trigger failed");
                }
            }
            pthread_cond_signal(&eventDispatcher->eventVectorRemoveCondition);
            SAFU_PTHREAD_MUTEX_UNLOCK(&eventDispatcher->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}
