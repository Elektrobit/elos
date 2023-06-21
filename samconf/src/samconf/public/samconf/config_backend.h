// SPDX-License-Identifier: MIT
#ifndef CONFIG_BACKEND_H
#define CONFIG_BACKEND_H

#include "samconf/samconf.h"

typedef struct samconfConfigBackend samconfConfigBackend_t;

typedef struct samconfConfigBackendOps {
    samconfConfigStatusE_t (*supports)(const char *location, bool *isSupported);
    samconfConfigStatusE_t (*open)(const char *location, samconfConfigBackend_t *backend);
    samconfConfigStatusE_t (*load)(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config);
    samconfConfigStatusE_t (*close)(samconfConfigBackend_t *backend);
} samconfConfigBackendOps_t;

struct samconfConfigBackend {
    void *originalHandle;                  // eg. File descriptor
    void *backendHandle;                   // internal json object
    const samconfConfigBackendOps_t *ops;  // Config operations
};

samconfConfigStatusE_t samconfConfigBackendNew(samconfConfigBackend_t **backend, const samconfConfigBackendOps_t *ops);
samconfConfigStatusE_t samconfConfigBackendInit(samconfConfigBackend_t *backend, const samconfConfigBackendOps_t *ops);
samconfConfigStatusE_t samconfConfigBackendDelete(samconfConfigBackend_t *backend);

#endif /* CONFIG_BACKEND_H */
