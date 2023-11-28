// SPDX-License-Identifier: MIT
#ifndef __ELOS_EVENTLOGGING_PLUGIN_JSONBACKEND_H__
#define __ELOS_EVENTLOGGING_PLUGIN_JSONBACKEND_H__

#include "elos/storagemanager/StorageBackend.h"
#include "safu/common.h"

#ifndef STORAGE_LOCATION
#define STORAGE_LOCATION "/var/log/elosd_event_%count%.log"
#endif

#ifndef ELOS_JSON_LOGGING_PATH_LIMIT_DEFAULT
#define ELOS_JSON_LOGGING_PATH_LIMIT_DEFAULT 1024
#endif

#ifndef ELOS_JSON_LOGGER_MAX_FILE_SIZE
#define ELOS_JSON_LOGGER_MAX_FILE_SIZE 100000
#endif

typedef struct elosJsonBackend {
    char *storageFilePattern;
    char *filePath;
    char *storageDate;
    char *dateFormat;
    size_t pathSizeLimit;
    bool pathContainsCount;
    int additionalFlags;
    int storageFileWrite;
    int storageFileRead;
    size_t count;
    size_t maxFileSize;
} elosJsonBackend_t;

safuResultE_t elosJsonBackendNew(elosStorageBackend_t **backend);

safuResultE_t elosJsonBackendStart(elosStorageBackend_t *backend);

safuResultE_t elosJsonBackendShutdown(elosStorageBackend_t *backend);

char *elosCompleteStoragePath(const char *const path, size_t sizelimit, const char *countStr, const char *const date,
                              bool *hasCount);

#endif /* __ELOS_EVENTLOGGING_PLUGIN_JSONBACKEND_H__ */
