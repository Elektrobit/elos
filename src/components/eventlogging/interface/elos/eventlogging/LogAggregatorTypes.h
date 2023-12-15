// SPDX-License-Identifier: MIT
#pragma once

#include <elos/storagemanager/storagemanager.h>
#include <elos/storagemanager/vector.h>
#include <samconf/samconf_types.h>

typedef struct elosLogAggregatorParam {
    samconfConfig_t *config;
    elosStorageManager_t *storageManager;
} elosLogAggregatorParam_t;

typedef struct elosLogAggregator {
    pthread_mutex_t *lock;
    samconfConfig_t const *config;
    elosStorageManager_t *storageManager;
    elosStorageBackendPtrVector_t *backends;
} elosLogAggregator_t;
