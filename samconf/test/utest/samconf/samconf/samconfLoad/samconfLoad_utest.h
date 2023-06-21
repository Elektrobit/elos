// SPDX-License-Identifier: MIT
#ifndef SAMCONF_LOAD_UTEST_H
#define SAMCONF_LOAD_UTEST_H

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
                                               samconfConfig_t **config) {
    *config = (void *)0xdeadb33f;
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

void samconfTestSamconfLoadSuccess(void **state);
void samconfTestSamconfLoadOpenError(void **state);
void samconfTestSamconfLoadLoadError(void **state);
void samconfTestSamconfLoadCloseError(void **state);
void samconfTestSamconfLoadLookupError(void **state);
void samconfTestSamconfLoadBackendDeleteError(void **state);
void samconfTestSamconfLoadConfigDeleteError(void **state);

#endif /* SAMCONF_LOAD_UTEST_H */
