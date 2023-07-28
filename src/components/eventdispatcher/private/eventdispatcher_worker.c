// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <poll.h>
#include <safu/log.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <time.h>

#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "eventdispatcher_private.h"

#if EAGAIN != EWOULDBLOCK
#define _CASE_EWOULDBLOCK case EWOULDBLOCK:
#else
#define _CASE_EWOULDBLOCK
#endif

#define _is_timespec_greather_than(__a, __b) \
    (((__a).tv_sec > (__b).tv_sec) || (((__a).tv_sec == (__b).tv_sec) && ((__a).tv_nsec >= (__b).tv_nsec)))

static void _handleHealthEvent(elosEventDispatcher_t *eventDispatcher) {
    elosEventDispatcherWorker_t *worker = &eventDispatcher->worker;
    struct timespec treshold = worker->healthTimeTreshold;
    struct timespec current = {0};
    int retVal;

    retVal = clock_gettime(CLOCK_MONOTONIC, &current);
    if (retVal < 0) {
        safuLogErrErrno("Fetching current time for health event failed");
    } else if (_is_timespec_greather_than(current, treshold)) {
        safuResultE_t result;
        elosEvent_t healthEvent = {0};
        char *payload = NULL;

        retVal = asprintf(&payload, "%lu Events published\n", worker->eventsPublished);
        if (retVal < 0) {
            safuLogErrErrno("Creating payload for health event failed");
        } else {
            struct timespec interval = worker->healthTimeInterval;
            struct timespec next = {0};

            healthEvent.date = current;
            healthEvent.classification = ELOS_CLASSIFICATION_ELOS;
            healthEvent.payload = payload;

            result = elosEventProcessorPublish(eventDispatcher->eventProcessor, &healthEvent);
            if (result != SAFU_RESULT_OK) {
                safuLogErrErrno("Publishing health event failed");
            }

            next.tv_nsec = ((current.tv_nsec + interval.tv_nsec) % 1000000000);
            next.tv_sec = ((current.tv_nsec + interval.tv_nsec) / 1000000000);
            next.tv_sec += (current.tv_sec + interval.tv_sec);
            worker->healthTimeTreshold = next;

            free(payload);
        }
    }
}

static void _waitForNextIteration(elosEventDispatcher_t *eventDispatcher) {
    int retVal;
    struct timespec const *timeout = &eventDispatcher->worker.pollTimeout;
    struct pollfd fds[] = {
        {.fd = eventDispatcher->worker.trigger, .events = POLLIN},
    };

    retVal = ppoll(fds, ARRAY_SIZE(fds), timeout, NULL);
    if (retVal < 0) {
        switch (errno) {
            _CASE_EWOULDBLOCK
            case EAGAIN:
            case EINTR:
                break;
            default:
                atomic_fetch_and(&eventDispatcher->flags, ~ELOS_EVENTDISPATCHER_FLAG_ACTIVE);
                safuLogErrErrno("ppoll failed");
                break;
        }
    } else if (retVal > 0) {
        eventfd_t value = 0;

        retVal = eventfd_read(eventDispatcher->worker.trigger, &value);
        if (retVal < 0) {
            atomic_fetch_and(&eventDispatcher->flags, ~ELOS_EVENTDISPATCHER_FLAG_ACTIVE);
            safuLogErrErrno("eventfd_read failed");
        }
    }
}

void *elosEventDispatcherWorker(void *data) {
    elosEventDispatcher_t *eventDispatcher = (elosEventDispatcher_t *)data;
    int retVal;

    atomic_fetch_or(&eventDispatcher->flags, ELOS_EVENTDISPATCHER_FLAG_ACTIVE);

    retVal = eventfd_write(eventDispatcher->sync, 1);
    if (retVal < 0) {
        atomic_fetch_and(&eventDispatcher->flags, ~ELOS_EVENTDISPATCHER_FLAG_ACTIVE);
        safuLogErrErrno("eventfd_write failed");
    } else {
        safuResultE_t result;

        while (ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&eventDispatcher->flags) == true) {
            _waitForNextIteration(eventDispatcher);

            result = elosEventDispatcherDispatch(eventDispatcher);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("Dispatching Events failed!");
            }

            _handleHealthEvent(eventDispatcher);
        }
    }

    return NULL;
}