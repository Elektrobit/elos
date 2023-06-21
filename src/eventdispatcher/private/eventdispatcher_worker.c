// SPDX-License-Identifier: MIT

#include "eventdispatcher_worker.h"

#include <safu/log.h>
#include <sched.h>
#include <sys/eventfd.h>

#include "elos/eventdispatcher/eventdispatcher.h"

#if EAGAIN != EWOULDBLOCK
#define _CASE_EWOULDBLOCK case EWOULDBLOCK:
#else
#define _CASE_EWOULDBLOCK
#endif

void *elosEventDispatcherWorker(void *data) {
    elosEventDispatcher_t *eventDispatcher = (elosEventDispatcher_t *)data;
    int retVal;

    atomic_fetch_or(&eventDispatcher->flags, ELOS_EVENTDISPATCHER_FLAG_ACTIVE);

    retVal = eventfd_write(eventDispatcher->sync, 1);
    if (retVal < 0) {
        atomic_fetch_and(&eventDispatcher->flags, ~ELOS_EVENTDISPATCHER_FLAG_ACTIVE);
        safuLogErrErrno("eventfd_write failed");
    } else {
        eventfd_t syncValue = 0;

        while (ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&eventDispatcher->flags) == true) {
            retVal = eventfd_read(eventDispatcher->worker.sync, &syncValue);
            if (retVal < 0) {
                switch (errno) {
                    _CASE_EWOULDBLOCK
                    case EAGAIN:
                    case EINTR:
                        break;
                    default:
                        atomic_fetch_and(&eventDispatcher->flags, ~ELOS_EVENTDISPATCHER_FLAG_ACTIVE);
                        safuLogErrErrno("eventfd_read failed");
                        break;
                }
            }
        }
    }

    return NULL;
}