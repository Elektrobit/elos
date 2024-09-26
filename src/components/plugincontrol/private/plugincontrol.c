// SPDX-License-Identifier: MIT

#include "elos/plugincontrol/plugincontrol.h"

#include <dlfcn.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <threads.h>
#include <unistd.h>

#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventlogging/LogAggregator.h"
#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/eventprocessor/eventqueuemanager.h"
#include "elos/libelosplugin/types.h"
#include "elos/plugincontrol/types.h"

#define _LOG_ERR_CONTROL_WRONG_STATE(__control, __state)                                          \
    safuLogErrF("Plugin id=%d is not in state '%s' (state=%d)", (__control)->context.id, __state, \
                (__control)->context.state);

#define _LOG_ERR_CONTROL_UNKNOWN_STATE(__control) \
    safuLogErrF("Plugin id=%d is in unknown state=%d", (__control)->context.id, (__control)->context.state);

static inline void _setupPluginSynchronization(elosPluginControl_t *control, int eventFdSync, int eventFdStop,
                                               int eventFdWorker) {
    control->context.stop = eventFdStop;
    control->context.sync = eventFdWorker;
    control->sync = eventFdSync;
}

static inline void _setupPluginApi(elosPluginControl_t *control) {
    control->context.createPublisher = elosPluginControlCreatePublisher;
    control->context.deletePublisher = elosPluginControlDeletePublisher;
    control->context.createSubscriber = elosPluginControlCreateSubscriber;
    control->context.deleteSubscriber = elosPluginControlDeleteSubscriber;
    control->context.publish = elosPluginControlPublish;
    control->context.store = elosPluginControlStore;
    control->context.findEvents = elosPluginControlFindEvents;
    control->context.subscribe = elosPluginControlSubscribe;
    control->context.readQueue = elosPluginControlEventQueueRead;
    control->context.unsubscribe = elosPluginControlUnsubscribe;
    control->context.unsubscribeAll = elosPluginControlUnsubscribeAll;
}

safuResultE_t elosPluginControlInitialize(elosPluginControl_t *control, elosPluginControlParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((control == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if ((param->path != NULL) && ((param->config == NULL) || (param->file != NULL))) {
        safuLogErr("Invalid value combination in parameter struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == true) {
        safuLogErr("Component is already initialized");
    } else {
        int eventFdWorker;
        int eventFdSync;
        int eventFdStop;

        memset(control, 0, sizeof(elosPluginControl_t));
        control->context.stop = -1;
        control->context.sync = -1;
        control->sync = -1;
        control->eventDispatcher = param->eventDispatcher;
        control->logAggregator = param->logAggregator;
        control->eventProcessor = param->eventProcessor;

        eventFdStop = eventfd(0, 0);
        if (eventFdStop == -1) {
            safuLogErrErrnoValue("eventfd failed", eventFdStop);
        } else {
            eventFdSync = eventfd(0, 0);
            if (eventFdSync == -1) {
                safuLogErrErrnoValue("eventfd (sync) failed", eventFdSync);
            } else {
                eventFdWorker = eventfd(0, 0);
                if (eventFdWorker == -1) {
                    safuLogErrErrnoValue("eventfd (worker) failed", eventFdWorker);
                } else {
                    result = SAFU_RESULT_OK;

                    if (param->file != NULL) {
                        control->file = strdup(param->file);
                        if (control->file == NULL) {
                            safuLogErr("String duplication failed");
                            result = SAFU_RESULT_FAILED;
                        }
                    }

                    control->context.instanceRef = control;
                    control->context.config = param->config;
                    control->context.data = param->data;
                    control->context.id = param->id;
                    control->context.useEnv = param->useEnv;
                    control->pluginType = param->pluginType;
                    control->path = param->path;
                    _setupPluginSynchronization(control, eventFdSync, eventFdStop, eventFdWorker);
                    _setupPluginApi(control);
                    atomic_store(&control->flags, SAFU_FLAG_INITIALIZED_BIT);
                }
            }
        }

        if (result != SAFU_RESULT_OK) {
            elosPluginControlDeleteMembers(control);
        }
    }

    return result;
}

safuResultE_t elosPluginControlNew(elosPluginControl_t **control, elosPluginControlParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((control == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        void *newPlugin;
        newPlugin = safuAllocMem(NULL, sizeof(elosPluginControl_t));
        if (newPlugin == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            memset(newPlugin, 0, sizeof(elosPluginControl_t));

            result = elosPluginControlInitialize(newPlugin, param);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlInitialize failed");
            } else {
                *control = newPlugin;
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlStart(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("Component is not initialized");
    } else if (ELOS_PLUGINCONTROL_FLAG_HAS_LOADED_BIT(&control->flags) == false) {
        safuLogErr("Plugin is not loaded");
    } else if (ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(&control->flags) == true) {
        safuLogErr("PluginControl is in an invalid state due to Plugin errors");
    } else {
        result = SAFU_RESULT_OK;

        if (ELOS_PLUGINCONTROL_FLAG_PUSH_STARTED_BIT(&control->flags) == true) {
            int retVal;

            retVal = eventfd_write(control->sync, 1);
            if (retVal < 0) {
                safuLogErrErrno("eventfd_write (sync) failed");
                result = SAFU_RESULT_FAILED;
            } else {
                eventfd_t efdVal = 0;

                retVal = eventfd_read(control->context.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read (worker.sync) failed");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlStop(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control == NULL) {
        safuLogErr("NULL parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("Component is not initialized");
    } else if (ELOS_PLUGINCONTROL_FLAG_HAS_LOADED_BIT(&control->flags) == false) {
        safuLogErr("Plugin is not loaded");
    } else {
        result = SAFU_RESULT_OK;

        if (ELOS_PLUGINCONTROL_FLAG_PULL_STARTED_BIT(&control->flags) == true) {
            if ((control->pluginConfig != NULL) && (control->pluginConfig->stop != NULL)) {
                if (ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(&control->flags) == true) {
                    safuLogWarn("PluginControl had errors during Plugin operation; Trying to stop Plugin.");
                }

                result = control->pluginConfig->stop(&control->context);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrF("PluginWorker stop call failed for %s", control->context.config->key);
                } else {
                    eventfd_t efdVal = 0;
                    int retVal;

                    retVal = eventfd_read(control->context.sync, &efdVal);
                    if (retVal < 0) {
                        safuLogErrErrno("eventfd_read (worker.sync) failed");
                        result = SAFU_RESULT_FAILED;
                    }
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlUnload(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("The given PluginControl is not initialized");
    } else {
        result = SAFU_RESULT_OK;

        if (ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(&control->flags) == true) {
            safuLogWarn("PluginControl had errors during Plugin operation; Trying to unload Plugin.");
        }

        if (ELOS_PLUGINCONTROL_FLAG_HAS_STARTED_BIT(&control->flags) == true) {
            result = elosPluginControlStop(control);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlStop failed");
            }
        }

        if (ELOS_PLUGINCONTROL_FLAG_PULL_LOADED_BIT(&control->flags) == true) {
            if ((ELOS_PLUGINCONTROL_FLAG_PULL_WORKER_BIT(&control->flags) == true)) {
                int retVal;

                retVal = pthread_join(control->workerThread, NULL);
                if (retVal < 0) {
                    safuLogErr("Plugin: pthread_join failed!");
                    result = SAFU_RESULT_FAILED;
                }
            }

            if ((control->pluginConfig != NULL) && (control->pluginConfig->unload != NULL)) {
                safuResultE_t subResult;

                subResult = control->pluginConfig->unload(&control->context);
                if (subResult != SAFU_RESULT_OK) {
                    safuLogWarn("PluginWorker: Unload function failed (might result in memory leaks)");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlDeleteMembers(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (control != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == true) {
            int retVal;

            if (ELOS_PLUGINCONTROL_FLAG_HAS_LOADED_BIT(&control->flags) == true) {
                result = elosPluginControlUnload(control);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosPluginControlStop failed");
                }
            }

            if (control->dlHandle != NULL) {
                retVal = dlclose(control->dlHandle);
                if (retVal < 0) {
                    safuLogWarnF("dlclose for plugin id:%d failed with: %s", control->context.id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            free(control->file);

            if (control->context.stop != -1) {
                retVal = close(control->context.stop);
                if (retVal < 0) {
                    safuLogWarnF("close (stop) for plugin id:%d failed with: %s", control->context.id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (control->sync != -1) {
                retVal = close(control->sync);
                if (retVal < 0) {
                    safuLogWarnF("close (sync) for plugin id:%d failed with: %s", control->context.id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (control->context.sync != -1) {
                retVal = close(control->context.sync);
                if (retVal < 0) {
                    safuLogWarnF("close (worker.sync) for plugin id:%d failed with: %s", control->context.id,
                                 strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            memset(control, 0, sizeof(elosPluginControl_t));
        }
    }

    return result;
}

safuResultE_t elosPluginControlDelete(elosPluginControl_t **control) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((control != NULL) && (*control != NULL)) {
        result = elosPluginControlDeleteMembers(*control);
        free(*control);
        *control = NULL;
    }

    return result;
}

safuResultE_t elosPluginControlGetName(const elosPluginControl_t *control, const char **name) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((control == NULL) || (name == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("Component is not initialized");
    } else if (control->context.config == NULL) {
        safuLogErr("Plugin has no configuration");
    } else {
        *name = control->context.config->key;
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosPluginControlCreatePublisher(struct elosPluginControl *pluginControl,
                                               struct elosPublisher **publisher) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosPublisher_t *newPublisher = safuAllocMem(NULL, sizeof(elosPublisher_t));
    if (newPublisher == NULL) {
        safuLogErr("safuAllocMem failed");
    } else {
        memset(newPublisher, 0, sizeof(elosPublisher_t));
        newPublisher->logAggregator = pluginControl->logAggregator;
        elosEventBufferParam_t eventBufferParam = {
            .limitEventCount = ELOS_EVENTBUFFER_DEFAULT_LIMIT,
        };
        result = elosEventBufferInitialize(&newPublisher->eventBuffer, &eventBufferParam);
        if (result != SAFU_RESULT_OK) {
            safuLogErrValue("Creating EventBuffer failed", result);
        } else {
            result = elosEventDispatcherBufferAdd(pluginControl->eventDispatcher, &newPublisher->eventBuffer);
            if (result != SAFU_RESULT_OK) {
                safuLogErrValue("Adding EventBuffer to EventDispatcher failed", result);
            }
        }
    }

    if (result == SAFU_RESULT_FAILED) {
        elosEventBufferDeleteMembers(&newPublisher->eventBuffer);
        free(newPublisher);
    } else {
        *publisher = newPublisher;
    }

    return result;
}

safuResultE_t elosPluginControlDeletePublisher(struct elosPluginControl *pluginControl,
                                               struct elosPublisher *publisher) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (pluginControl == NULL || publisher == NULL) {
        safuLogErr("Invalid paramter");
    } else {
        result = elosEventDispatcherBufferRemove(pluginControl->eventDispatcher, &publisher->eventBuffer);
        if (result != SAFU_RESULT_OK) {
            safuLogErrValue("Removing EventBuffer from EventDispatcher failed", result);
        } else {
            elosEventBufferDeleteMembers(&publisher->eventBuffer);
            free(publisher);
        }
    }

    return result;
}

safuResultE_t elosPluginControlCreateSubscriber(struct elosPluginControl *pluginControl,
                                                struct elosSubscriber **subscriber) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosSubscriber_t *newSubscriber = safuAllocMem(NULL, sizeof(elosSubscriber_t));
    if (newSubscriber == NULL) {
        safuLogErr("safuAllocMem failed");
    } else {
        newSubscriber->eventProcessor = pluginControl->eventProcessor;
        result = safuVecCreate(&newSubscriber->subscriptions, 1, sizeof(elosSubscription_t *));
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Creating subscription vector failed");
        }
    }

    if (result == SAFU_RESULT_FAILED) {
        free(newSubscriber);
    } else {
        *subscriber = newSubscriber;
    }

    return result;
}

safuResultE_t elosPluginControlDeleteSubscriber(UNUSED struct elosPluginControl *pluginControl,
                                                struct elosSubscriber *subscriber) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (subscriber == NULL) {
        safuLogErr("Invalid paramter");
    } else {
        safuVecFree(&subscriber->subscriptions);
        free(subscriber);
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosPluginControlPublish(elosPublisher_t *publisher, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    safuResultE_t bufferResult = elosEventBufferWrite(&publisher->eventBuffer, event);
    if (bufferResult != SAFU_RESULT_OK) {
        safuLogErr("Writing into the EventBuffer failed");
        result = SAFU_RESULT_FAILED;
    }

    safuResultE_t loggerResult = elosLogAggregatorAdd(publisher->logAggregator, event);
    if (loggerResult != SAFU_RESULT_OK) {
        safuLogErr("elosLogAggregatorAdd failed");
        result = SAFU_RESULT_FAILED;
    }

    if (bufferResult == SAFU_RESULT_OK && loggerResult == SAFU_RESULT_OK) {
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosPluginControlFindEvents(elosPluginControl_t *pluginControl, const char *rule,
                                          struct timespec const *newest, struct timespec const *oldest,
                                          safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosLogAggregatorFindEvents(pluginControl->logAggregator, rule, newest, oldest, events);
    return result;
}

safuResultE_t elosPluginControlEventQueueRead(elosSubscriber_t *subscriber, const elosSubscription_t *subscription,
                                              safuVec_t **const eventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosEventProcessorQueueRead(subscriber->eventProcessor, subscription->eventQueueId, eventVector);

    return result;
}

safuResultE_t elosPluginControlStore(elosPluginControl_t *pluginControl, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosLogAggregatorAdd(pluginControl->logAggregator, event);

    return result;
}

safuResultE_t elosPluginControlSubscribe(elosSubscriber_t *subscriber, char const *const *filterStrings,
                                         size_t filterCount, const elosSubscription_t **const subscription) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosSubscription_t *newSubscription = NULL;

    newSubscription = safuAllocMem(NULL, sizeof(elosSubscription_t));
    if (newSubscription == NULL) {
        safuLogErr("safuAllocMem failed");
    } else {
        newSubscription->eventQueueId = ELOS_ID_INVALID;
        newSubscription->eventFilterNodeId = ELOS_ID_INVALID;
        result = elosEventProcessorFilterNodeCreateWithQueue(subscriber->eventProcessor, (const char **)filterStrings,
                                                             filterCount, &newSubscription->eventQueueId,
                                                             &newSubscription->eventFilterNodeId);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("%s", "elosEventProcessorFilterNodeCreateWithQueue failed");
            safuLogDebugF("%s", "failed to create eventQueue and/or eventFilterNode");
        } else {
            int retval = safuVecPush(&subscriber->subscriptions, &newSubscription);
            if (retval < 0) {
                safuLogErrF("safuVecPush with subscription '%u/%u' failed", newSubscription->eventFilterNodeId,
                            newSubscription->eventQueueId);
                result = SAFU_RESULT_FAILED;
            }
        }

        if (result == SAFU_RESULT_FAILED) {
            if (newSubscription->eventFilterNodeId != ELOS_ID_INVALID) {
                elosEventFilterManagerNodeRemove(&subscriber->eventProcessor->eventFilterManager,
                                                 newSubscription->eventFilterNodeId);
            }
            if (newSubscription->eventQueueId != ELOS_ID_INVALID) {
                elosEventQueueManagerEntryDelete(&subscriber->eventProcessor->eventQueueManager,
                                                 newSubscription->eventQueueId);
            }
            free(newSubscription);
        } else {
            *subscription = newSubscription;
        }
    }

    return result;
}

static int _findSubscriptionPtr(const void *sub, const void *search) {
    elosSubscription_t *subscription = *(elosSubscription_t **)sub;
    elosSubscription_t *searchTarget = (elosSubscription_t *)search;
    return subscription == searchTarget;
}

safuResultE_t elosPluginControlUnsubscribe(elosSubscriber_t *subscriber, elosSubscription_t const *const subscription) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (subscriber == NULL || subscription == NULL) {
        safuLogErr("Invalid paramter");
    } else {
        result = elosEventProcessorFilterNodeRemove(subscriber->eventProcessor, subscription->eventFilterNodeId);
        if (result != SAFU_RESULT_OK) {
            safuLogWarnF("elosEventProcessorFilterNodeRemove with 'id:%u' failed", subscription->eventFilterNodeId);
        }

        result = elosEventProcessorQueueRemove(subscriber->eventProcessor, subscription->eventQueueId);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("elosEventProcessorQueueRemove with 'id:%u' failed", subscription->eventQueueId);
        }
        int res = safuVecFindRemove(&subscriber->subscriptions, &_findSubscriptionPtr, subscription);
        if (res != 1) {
            safuLogWarnF("subscription %p not found in subscriber for removal ", (void *)subscription);
            result = SAFU_RESULT_FAILED;
        }
        free((void *)subscription);
    }
    return result;
}

static safuResultE_t _pluginControlUnsubscribeIdx(elosSubscriber_t *subscriber, size_t idx) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosSubscription_t **subscription = safuVecGet(&subscriber->subscriptions, idx);
    if (subscription == NULL) {
        safuLogErrF("safuVecGet with 'idx:%lu' failed", idx);
    } else {
        result = elosPluginControlUnsubscribe(subscriber, *subscription);
    }
    return result;
}

safuResultE_t elosPluginControlUnsubscribeAll(elosSubscriber_t *subscriber) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    size_t count = safuVecElements(&subscriber->subscriptions);
    for (size_t idx = 0; idx < safuVecElements(&subscriber->subscriptions);) {
        result = _pluginControlUnsubscribeIdx(subscriber, idx);
        if (result != SAFU_RESULT_OK) {
            idx += 1;
        }
    }
    size_t failed = safuVecElements(&subscriber->subscriptions);
    if (failed != 0) {
        safuLogErrF("Not all subscriptions could successfully be revoked (%lu/%lu failed)", failed, count);
        result = SAFU_RESULT_FAILED;
    } else {
        result = SAFU_RESULT_OK;
    }

    return result;
}
