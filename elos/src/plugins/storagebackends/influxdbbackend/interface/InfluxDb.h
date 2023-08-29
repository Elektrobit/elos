// SPDX-License-Identifier: MIT
#pragma once

#include "elos/eventlogging/StorageBackend.h"
#include "safu/common.h"

#define CURL_HEADER_1 "Accept: application/json"
#define CURL_HEADER_2 "Content-type: application/json"
#define CURL_HEADER_3 "Content-type: text/plain; charset=utf-8"

typedef struct elosInfluxDbBackend {
    char *host;
    char *token;
    char *orgId;
    char *db;
    int influixdPid;
} elosInfluxDbBackend_t;

safuResultE_t elosInfluxDbBackendNew(elosStorageBackend_t **backend);

safuResultE_t elosInfluxDbBackendStart(elosStorageBackend_t *backend);

safuResultE_t elosInfluxDbBackendShutdown(elosStorageBackend_t *backend);
