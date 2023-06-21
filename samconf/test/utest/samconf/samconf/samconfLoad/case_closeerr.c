// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "safu/mock_safu.h"
#include "samconfLoad_utest.h"

static samconfConfigStatusE_t samconfCloseDummyFail(UNUSED samconfConfigBackend_t *backend) {
    return SAMCONF_CONFIG_ERROR;
}

static const samconfConfigBackendOps_t samconfDummyOps = {
    .open = samconfOpenDummy,
    .load = samconfLoadDummy,
    .close = samconfCloseDummyFail,
};

void samconfTestSamconfLoadCloseError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t backend = {.ops = &samconfDummyOps};
    samconfConfig_t *config = NULL;
    const char *location = "config.json";

    TEST("samconfTestSamconfLoad");
    SHOULD("%s", "fail on unsuccesful close");

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
    will_return(samconfConfigBackendDelete, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigDelete, 0);
    expect_value(samconfConfigDelete, config, (void *)0xdeadb33f);
    will_return(samconfConfigDelete, SAMCONF_CONFIG_OK);

    status = samconfLoad(location, false, &config);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
