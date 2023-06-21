// SPDX-License-Identifier: MIT
#ifndef SAMCONF_LOOKUP_BACKEND_UTEST_H
#define SAMCONF_LOOKUP_BACKEND_UTEST_H

#include "mock_samconf.h"
#include "samconf_utest.h"

static samconfConfigStatusE_t samconfSupportsDummy(UNUSED const char *location, bool *isSupported) {
    *isSupported = true;
    return SAMCONF_CONFIG_OK;
}

static samconfConfigStatusE_t samconfOpenDummy(UNUSED const char *location, UNUSED samconfConfigBackend_t *backend) {
    return SAMCONF_CONFIG_OK;
}

static samconfConfigStatusE_t samconfLoadDummy(UNUSED samconfConfigBackend_t *backend, UNUSED bool isSigned,
                                               UNUSED samconfConfig_t **config) {
    return SAMCONF_CONFIG_OK;
}

static samconfConfigStatusE_t samconfCloseDummy(UNUSED samconfConfigBackend_t *backend) {
    return SAMCONF_CONFIG_OK;
}

static const samconfConfigBackendOps_t samconfDefaultOps = {
    .supports = samconfSupportsDummy,
    .open = samconfOpenDummy,
    .load = samconfLoadDummy,
    .close = samconfCloseDummy,
};

void samconfTestSamconfLookupBackendSuccess(void **state);
void samconfTestSamconfLookupBackendErrorNoLoop(void **state);
void samconfTestSamconfLookupBackendErrorSupportFail(void **state);
void samconfTestSamconfLookupBackendErrorAlloc(void **state);

#endif /* SAMCONF_LOOKUP_BACKEND_UTEST_H */
