// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/ringbuffer.h>
#include <safu/vector_types.h>

#include "dlt_hv/shmem_ring_buffer.h"
#include "dlt_hv/types.h"
#include "elos/libelosplugin/types.h"

#define BUFF_SIZE 20

safuResultE_t elosDeleteEbLogEntry(void *entry);

struct elosTestData {
    safuRingBuffer_t *readQueue;
    elosPlugin_t *plugin;
};

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPublishEntriesFromQueueSuccess)
