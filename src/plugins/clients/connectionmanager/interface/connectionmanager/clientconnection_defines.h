// SPDX-License-Identifier: MIT
#pragma once

#include <stdatomic.h>
#include <time.h>

#define ELOS_CLIENTCONNECTION_TAKEN_BIT  (SAFU_FLAG_CUSTOM_START_BIT << 0)
#define ELOS_CLIENTCONNECTION_ACTIVE_BIT (SAFU_FLAG_CUSTOM_START_BIT << 1)

#define ELOS_CLIENTCONNECTION_HAS_TAKEN_BIT(__flag)  ((atomic_load(__flag) & ELOS_CLIENTCONNECTION_TAKEN_BIT) != 0)
#define ELOS_CLIENTCONNECTION_HAS_ACTIVE_BIT(__flag) ((atomic_load(__flag) & ELOS_CLIENTCONNECTION_ACTIVE_BIT) != 0)

#ifndef ELOS_CLIENTCONNECTION_WORKER_START_TIMEOUT
#define ELOS_CLIENTCONNECTION_WORKER_START_TIMEOUT \
    (struct timespec) {                            \
        .tv_sec = 0, .tv_nsec = 500 * 1000 * 1000  \
    }
#endif

#ifndef ELOS_CLIENTCONNECTION_WORKER_STOP_TIMEOUT
#define ELOS_CLIENTCONNECTION_WORKER_STOP_TIMEOUT \
    (struct timespec) {                           \
        .tv_sec = 0, .tv_nsec = 500 * 1000 * 1000 \
    }
#endif

#ifndef ELOS_CLIENTCONNECTION_WORKER_POLL_TIMEOUT
#define ELOS_CLIENTCONNECTION_WORKER_POLL_TIMEOUT \
    (struct timespec) {                           \
        .tv_sec = 0, .tv_nsec = 250 * 1000 * 1000 \
    }
#endif
