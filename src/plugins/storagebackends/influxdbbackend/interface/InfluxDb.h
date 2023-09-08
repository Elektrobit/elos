// SPDX-License-Identifier: MIT
#pragma once

#include "elos/eventlogging/StorageBackend.h"
#include "safu/common.h"

#define ACCEPT_APP_JSON         "Accept: application/json"
#define CONTENT_TYPE_APP_JSON   "Content-type: application/json"
#define CONTENT_TYPE_TEXT_PLAIN "Content-type: text/plain; charset=utf-8"

typedef struct elosInfluxDbBackend {
    char *host;
    char *user;
    char *pw;
    char *orgId;
    char *db;
    int influixdPid;
} elosInfluxDbBackend_t;

safuResultE_t elosInfluxDbBackendNew(elosStorageBackend_t **backend);

safuResultE_t elosInfluxDbBackendStart(elosStorageBackend_t *backend);

safuResultE_t elosInfluxDbBackendShutdown(elosStorageBackend_t *backend);
