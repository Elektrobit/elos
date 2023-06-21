// SPDX-License-Identifier: MIT

#include <samconf/config_backend.h>

#include "samconfConfigBackendInit_utest.h"

int samconfTestSamconfConfigBackendInitSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigBackendInitSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigBackendInitSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t backend = {
        .originalHandle = (void *)0xdeadb33f,
        .backendHandle = (void *)0xb105f00d,
        .ops = (void *)0xdeadb33f,
    };
    samconfConfigBackendOps_t backendOps = {0};

    TEST("samconfConfigBackendInit");
    SHOULD("%s", "initialize configuration backend");

    status = samconfConfigBackendInit(&backend, &backendOps);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_ptr_equal(backend.originalHandle, NULL);
    assert_ptr_equal(backend.backendHandle, NULL);
    assert_ptr_equal(backend.ops, &backendOps);
}
