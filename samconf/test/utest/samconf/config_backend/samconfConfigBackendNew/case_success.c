// SPDX-License-Identifier: MIT

#include <samconf/config_backend.h>

#include "mock_samconf.h"
#include "safu/mock_safu.h"
#include "samconfConfigBackendNew_utest.h"

int samconfTestSamconfConfigBackendNewSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigBackendNewSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigBackendNewSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t *backend;
    samconfConfigBackend_t stackInst;
    samconfConfigBackendOps_t *backendOps = (void *)0xdeadb33f;

    TEST("samconfConfigBackendNew");
    SHOULD("%s", "return pointer to new memory of configuration backend");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(*backend));
    will_return(__wrap_safuAllocMem, &stackInst);

    MOCK_FUNC_AFTER_CALL(samconfConfigBackendInit, 0);
    expect_value(samconfConfigBackendInit, backend, &stackInst);
    expect_value(samconfConfigBackendInit, ops, backendOps);
    will_return(samconfConfigBackendInit, SAMCONF_CONFIG_OK);

    status = samconfConfigBackendNew(&backend, backendOps);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_ptr_equal(backend, &stackInst);
}
