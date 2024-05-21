// SPDX-License-Identifier: MIT
#ifndef __ELOS_NOSQLBACKEND_H__
#define __ELOS_NOSQLBACKEND_H__

#include <mongoc/mongoc.h>

#include "elos/eventfilter/eventfilter.h"
#include "elos/storagemanager/StorageBackend.h"
#include "safu/common.h"
#include "safu/vector.h"

#ifndef CONNECTION_STRING
#define CONNECTION_STRING "mongodb://localhost:27017/?appname=elosd"
#endif

typedef struct elosNoSqlBackend {
    char *connectionString;
    mongoc_client_t *client;
    mongoc_collection_t *collection;
} elosNoSqlBackend_t;

safuResultE_t elosNoSqlBackendNew(elosStorageBackend_t **backend);

safuResultE_t elosNoSqlBackendStart(elosStorageBackend_t *backend);

safuResultE_t elosNoSqlBackendShutdown(elosStorageBackend_t *backend);

#endif /* __ELOS_NOSQLBACKEND_H__ */
