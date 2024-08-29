// SPDX-License-Identifier: MIT
#pragma once

#include <elos/storagemanager/vector.h>
#include <samconf/samconf_types.h>

typedef struct elosLogAggregatorParam {
    samconfConfig_t *config;
    elosStorageBackendPtrVector_t *backends;
} elosLogAggregatorParam_t;

typedef struct elosLogAggregator {
    pthread_mutex_t *lock;
    samconfConfig_t const *config;
    elosStorageBackendPtrVector_t *backends;
    uint32_t fetchapiBackendPluginIndex;
} elosLogAggregator_t;
