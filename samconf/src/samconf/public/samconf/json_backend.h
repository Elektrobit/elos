// SPDX-License-Identifier: MIT
#ifndef SAMCONF_JSON_BACKEND_H
#define SAMCONF_JSON_BACKEND_H

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

#define DECIMAL_DIGITS_BOUND(t) (241 * sizeof(t) / 100 + 2)

extern const samconfConfigBackendOps_t samconfJsonOps;

samconfConfigStatusE_t samconfJsonBackendOpen(const char *location, samconfConfigBackend_t *backend);

samconfConfigStatusE_t samconfJsonBackendLoad(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config);

samconfConfigStatusE_t samconfJsonBackendSupports(const char *location, bool *isSupported);

samconfConfigStatusE_t samconfJsonBackendClose(samconfConfigBackend_t *backend);

#endif /* SAMCONF_JSON_BACKEND_H */
