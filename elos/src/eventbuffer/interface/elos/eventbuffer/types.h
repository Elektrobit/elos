// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>
#include <safu/ringbuffer_types.h>
#include <safu/types.h>

#include "elos/common/types.h"
#include "elos/event/event_types.h"

typedef elosId_t elosEventBufferId_t;
typedef safuVec_t elosEventPtrVector_t;
typedef safuVec_t elosEventBufferPtrVector_t;

typedef struct elosEventBufferRing {
    safuRingBuffer_t ringBuffer;
    uint32_t limitEventCount;
} elosEventBufferRing_t;

typedef struct elosEventBuffer {
    safuFlags_t flags;
    elosEventBufferRing_t *ring;
    size_t ringCount;
} elosEventBuffer_t;

typedef struct elosEventBufferParam {
    uint32_t limitEventCount;
} elosEventBufferParam_t;