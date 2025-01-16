// SPDX-License-Identifier: MIT
#pragma once

#include <safu/flags.h>

#define ELOS_EVENTDISPATCHER_FLAG_ACTIVE              SAFU_FLAG_CUSTOM_START_BIT
#define ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(__f) ((atomic_load(__f) & ELOS_EVENTDISPATCHER_FLAG_ACTIVE) != 0)

#ifndef ELOS_EVENTDISPATCHER_DEFAULT_VECTOR_SIZE
#define ELOS_EVENTDISPATCHER_DEFAULT_VECTOR_SIZE 8
#endif

#ifndef ELOS_EVENTDISPATCHER_DEFAULT_POLL_TIMEOUT
#define ELOS_EVENTDISPATCHER_DEFAULT_POLL_TIMEOUT  \
    (struct timespec) {                            \
        .tv_sec = 0, .tv_nsec = 250 * 1000 * 1000, \
    }
#endif

#ifndef ELOS_EVENTDISPATCHER_DEFAULT_HEALTH_INTERVAL
#define ELOS_EVENTDISPATCHER_DEFAULT_HEALTH_INTERVAL \
    (struct timespec) {                              \
        .tv_sec = 1, .tv_nsec = 0,                   \
    }
#endif
