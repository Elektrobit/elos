// SPDX-License-Identifier: MIT
#ifndef __ELOS_EVENTLOGGING_PLUGIN_SQLBACKEND_H__
#define __ELOS_EVENTLOGGING_PLUGIN_SQLBACKEND_H__

#include <sqlite3.h>

#include "elos/libelosplugin/StorageBackend_types.h"
#include "safu/common.h"
#include "safu/vector.h"

typedef struct elosSqlBackend {
    const char *connectionString;
    sqlite3 *db;
    sqlite3_stmt *insertEvent;
} elosSqlBackend_t;

safuResultE_t elosSqlBackendNew(elosStorageBackend_t **backend);

safuResultE_t elosSqlBackendStart(elosStorageBackend_t *backend);
safuResultE_t elosSqlBackendShutdown(elosStorageBackend_t *backend);

#endif /* __ELOS_EVENTLOGGING_PLUGIN_SQLBACKEND_H__ */
