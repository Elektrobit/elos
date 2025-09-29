// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <dlt_hv/shmem_ring_buffer.h>
#include <safu/ringbuffer.h>

#define BUFF_SIZE 20

safuResultE_t elosTestDeleteEbLogEntry(void *entry);

struct elosTestData {
    elosEbLogRingBuffer_t *dltRingBuffer;
    uint16_t readIndex;
    safuRingBuffer_t *readQueue;
};

TEST_CASE_FUNC_PROTOTYPES(elosTestElosQueueNextLogEntriesSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosQueueNextLogEntriesSuccessEmpty)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosQueueNextLogEntriesErrIdxToBig)
