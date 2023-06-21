// SPDX-License-Identifier: MIT

#include <samconf/config_backend.h>

#include "mock_samconf.h"
#include "safu/mock_safu.h"
#include "samconfConfigBackendNew_utest.h"

int samconfTestSamconfConfigBackendNewInitErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigBackendNewInitErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigBackendNewInitError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t *backend = (void *)0xc0ffee;
    samconfConfigBackendOps_t *backendOps = (void *)0xdeadb33f;

    TEST("samconfConfigBackendNew");
    SHOULD("%s", "receive error while initializing the configuration backend");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(*backend));
    will_return(__wrap_safuAllocMem, (void *)0xbaaaaaad);

    MOCK_FUNC_AFTER_CALL(samconfConfigBackendInit, 0);
    expect_any(samconfConfigBackendInit, backend);
    expect_value(samconfConfigBackendInit, ops, backendOps);
    will_return(samconfConfigBackendInit, SAMCONF_CONFIG_ERROR);

    status = samconfConfigBackendNew(&backend, backendOps);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(backend, (void *)0xc0ffee);
}
