// SPDX-License-Identifier: MIT

#include <samconf/config_backend.h>

#include "cmocka_mocks/mock_libc.h"
#include "samconfConfigBackendDelete_utest.h"

int samconfTestSamconfConfigBackendDeleteSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigBackendDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigBackendDeleteSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t *backend = (void *)0xde4dda1a;

    TEST("samconfConfigBackendDelete");
    SHOULD("%s", "free the given backend");

    MOCK_FUNC_AFTER_CALL(free, 0);
    expect_value(__wrap_free, ptr, backend);

    status = samconfConfigBackendDelete(backend);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
}
