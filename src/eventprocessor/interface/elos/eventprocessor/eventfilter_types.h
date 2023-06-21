// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>
#include <stddef.h>

#include "elos/common/types.h"
#include "elos/eventfilter/vector_types.h"
#include "elos/eventprocessor/eventqueue_types.h"

typedef elosId_t elosEventFilterNodeId_t;
typedef safuVec_t elosEventFilterNodeVector_t;

typedef enum elosEventFilterNodeStateE {
    EVENTFILTERNODE_STATE_INVALID = 0,
    EVENTFILTERNODE_STATE_INITIALIZED,
} elosEventFilterNodeStateE_t;

typedef struct elosEventFilterNode {
    _Atomic elosEventFilterNodeStateE_t state;
    pthread_mutex_t *mutex;
    elosEventFilterNodeId_t id;
    elosEventFilterVector_t eventFilterVector;
    elosEventQueue_t *eventQueue;
} elosEventFilterNode_t;

typedef struct elosEventFilterNodeParam {
    elosEventFilterNodeId_t id;
    elosEventQueue_t *eventQueue;
    char const **filterStrings;
    size_t filterStringCount;
} elosEventFilterNodeParam_t;
