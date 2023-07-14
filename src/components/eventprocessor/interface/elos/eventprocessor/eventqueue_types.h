// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/vector_types.h>
#include <stdint.h>

#include "elos/event/event_types.h"
#include "elos/event/eventqueue_types.h"

typedef safuVec_t elosEventQueueVector_t;

typedef struct elosEventQueueParam {
    elosEventQueueId_t id;
    elosEventQueueLimitEvents_t limitEvents;
    elosEventQueueLimitTime_t limitTime;
    uint32_t elements;
} elosEventQueueParam_t;

typedef struct elosEventQueue {
    elosEventQueueId_t id;
    elosEventVector_t *eventVector;
    elosEventQueueLimitEvents_t limitEvents;
    elosEventQueueLimitTime_t limitTime;
    uint32_t defaultElements;
    pthread_mutex_t *mutex;
} elosEventQueue_t;
