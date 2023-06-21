// SPDX-License-Identifier: MIT

#include <samconf/config_backend.h>

#include "safu/mock_safu.h"
#include "samconfConfigBackendNew_utest.h"

int samconfTestSamconfConfigBackendNewMemErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigBackendNewMemErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigBackendNewMemError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t *backend = (void *)0xc0ffee;
    samconfConfigBackendOps_t *backendOps = (void *)0xdeadb33f;

    TEST("samconfConfigBackendNew");
    SHOULD("%s", "receive error while allocating memory for the configuration backend");

    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(*backend));
    will_return(__wrap_safuAllocMem, NULL);
    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);

    status = samconfConfigBackendNew(&backend, backendOps);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(backend, (void *)0xc0ffee);
}
