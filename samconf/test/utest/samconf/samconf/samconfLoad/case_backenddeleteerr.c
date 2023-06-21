// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "safu/mock_safu.h"
#include "samconfLoad_utest.h"

void samconfTestSamconfLoadBackendDeleteError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t backend = {.ops = &samconfDefaultOps};
    samconfConfig_t *config = NULL;
    const char *location = "config.json";

    TEST("samconfTestSamconfLoad");
    SHOULD("%s", "fail on unsuccessful backend delete");

    MOCK_FUNC_AFTER_CALL(samconfVerifySignature, 0);
    expect_string(samconfVerifySignature, location, location);
    will_return(samconfVerifySignature, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfLookupBackend, 0);
    expect_value(samconfLookupBackend, location, location);
    expect_any(samconfLookupBackend, backend);
    will_set_parameter(samconfLookupBackend, backend, &backend);
    will_return(samconfLookupBackend, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigBackendDelete, 0);
    expect_value(samconfConfigBackendDelete, backend, &backend);
    will_return(samconfConfigBackendDelete, SAMCONF_CONFIG_ERROR);

    MOCK_FUNC_AFTER_CALL(samconfConfigDelete, 0);
    expect_value(samconfConfigDelete, config, (void *)0xdeadb33f);
    will_return(samconfConfigDelete, SAMCONF_CONFIG_OK);

    status = samconfLoad(location, false, &config);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
