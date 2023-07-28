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

/*******************************************************************
 * Parameter for EventBuffer initialization
 *
 * Members:
 *   limitEventCount: Number of Events for each priority that an EventBuffer is required to keep available
 ******************************************************************/
typedef struct elosEventBufferParam {
    uint32_t limitEventCount;
} elosEventBufferParam_t;

/*******************************************************************
 * Ring data structure of an EventBuffer.
 * Stores the Events written to the EventBuffer, each priority (e.g. HIGH, NORMAL) has its own indivual ring.
 *
 * Members:
 *   ringBuffer: safuRingBuffer containing the Events
 *   limitEventCount: size of the RingBuffer used for storing the Events
 ******************************************************************/
typedef struct elosEventBufferRing {
    safuRingBuffer_t ringBuffer;
    uint32_t limitEventCount;
} elosEventBufferRing_t;

/*******************************************************************
 * Data structure of an EventBuffer
 *
 * Members:
 *   flags: contains the component status bits, e.g. initialized
 *   writeTrigger: optional eventfd that is used for signalizing that an event was written
 *   ring: pointer to an array of elosEventBufferRing_t's
 *   ringCount: size of the array of elosEventBufferRing_t's
 ******************************************************************/
typedef struct elosEventBuffer {
    safuFlags_t flags;
    atomic_int writeTrigger;
    elosEventBufferRing_t *ring;
    size_t ringCount;
} elosEventBuffer_t;
