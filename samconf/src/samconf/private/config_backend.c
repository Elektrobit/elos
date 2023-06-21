// SPDX-License-Identifier: MIT
#include "samconf/config_backend.h"

#include <safu/common.h>
#include <stdlib.h>

samconfConfigStatusE_t samconfConfigBackendNew(samconfConfigBackend_t **backend, const samconfConfigBackendOps_t *ops) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    samconfConfigBackend_t *tmp;

    tmp = safuAllocMem(NULL, sizeof(*tmp));
    if (tmp != NULL) {
        status = samconfConfigBackendInit(tmp, ops);
        if (status == SAMCONF_CONFIG_OK) {
            *backend = tmp;
        }
    }

    return status;
}

samconfConfigStatusE_t samconfConfigBackendInit(samconfConfigBackend_t *backend, const samconfConfigBackendOps_t *ops) {
    memset(backend, 0, sizeof(*backend));
    backend->ops = ops;
    return SAMCONF_CONFIG_OK;
}

samconfConfigStatusE_t samconfConfigBackendDelete(samconfConfigBackend_t *backend) {
    free(backend);
    return SAMCONF_CONFIG_OK;
}
