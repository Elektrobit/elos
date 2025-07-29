// SPDX-License-Identifier: MIT
#ifndef __ELOS_EVENTLOGGING_PLUGIN_JSONBACKEND_H__
#define __ELOS_EVENTLOGGING_PLUGIN_JSONBACKEND_H__

#include "elos/libelosplugin/StorageBackend_types.h"
#include "safu/result.h"

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

/**
 * Initialize the file backed storage of JSON serialized event objects. The
 * ``elosStorageBackend_t.backendData`` shall be initialized with a pointer
 * to a ``elosJsonBackend_t`` instance.
 * ``elosJsonBackend_t.storageFileWrite`` and
 * ``elosJsonBackend_t.storageFileRead`` shall contain valid file
 * descriptors to the target file for writing/reading purposes. In write
 * mode, the file shall be opened for appending and synchronous writing, to
 * ensure a write is carried out through the kernel caches.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosJsonBackendStart(elosStorageBackend_t *backend);

/**
 * Shutdown the logging system and free all related resources.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosJsonBackendShutdown(elosStorageBackend_t *backend);

/**
 * Assembles the complete storage path replacing "%date%", "%host%" and "%count%"
 *
 * Parameters:
 *     path (const char* const): The path template to assemble
 *     sizelimit (size_t): The maximum length for the assembled path
 *     countStr (const char*): The string that should replace "%count%" in the assembled path
 *     date (const char* const): The string that should replace "%date%" in the assembled path
 *     hasCount (bool*): Return parameter to tell if the path template
 *                       or count was appendend to the end of the assembled path
 *
 * Returns:
 *     char*: The fully assembled path string
 *            defaulting to STORAGE_LOCATION when allocation for the path fails,
 *            or when NULL was passed as path template
 */
char *elosCompleteStoragePath(const char *const path, size_t sizelimit, const char *countStr, const char *const date,
                              bool *hasCount);

#endif /* __ELOS_EVENTLOGGING_PLUGIN_JSONBACKEND_H__ */
