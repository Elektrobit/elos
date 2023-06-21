// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>
#include <safu/result.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

#define SAFU_RINGBUFFER_ELEMENT_SIZE sizeof(void *)

#define SAFU_RINGBUFFER_DELETE_ENTRIES_BIT       (SAFU_FLAG_CUSTOM_START_BIT << 0)
#define SAFU_FLAG_HAS_DELETE_ENTRIES_BIT(__flag) ((atomic_load(__flag) & SAFU_RINGBUFFER_DELETE_ENTRIES_BIT) != 0)

typedef safuResultE_t(safuRingBufferEntryDeleteFunc_t)(void *);

typedef struct safuRingBufferParam {
    size_t elements;
    bool deleteEntries;
    safuRingBufferEntryDeleteFunc_t *deleteFunc;
} safuRingBufferParam_t;

typedef struct safuRingBufferCallback {
    safuRingBufferEntryDeleteFunc_t *delete;
} safuRingBufferCallback_t;

/**
 * Represents a ring buffer.
 *
 * Members:
 *  flags: Indicate the current status of the ringbuffer.
 *          SAFU_FLAG_INITIALIZED_BIT – successfully initialized and ready to use
 *          SAFU_FLAG_NONE – uninitilized or deleted ready for removal
 *          SAFU_RINGBUFFER_DELETE_ENTRIES_BIT – TODO
 *  memorySize: actual size acquired in bytes
 *  elements: number of elements can be stored
 *  elementsWritten: actual number of elemnts available to read
 *  nextIdx: index of first element of next read
 *  lock: mutex to read & write the buffer
 *  callback: TODO
 *  data: TODO
 *
 */
typedef struct safuRingBuffer {
    safuFlags_t flags;
    size_t memorySize;
    size_t elements;
    size_t elementsWritten;
    size_t nextIdx;
    pthread_mutex_t lock;
    safuRingBufferCallback_t callback;
    void **data;
} safuRingBuffer_t;
