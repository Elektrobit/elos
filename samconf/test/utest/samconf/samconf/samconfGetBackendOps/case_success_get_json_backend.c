// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfGetBackendOps_utest.h"

void samconfTestSamconfGetBackendOpsSuccessGetJsonBackend(UNUSED void **state) {
    const samconfConfigBackendOps_t *opsJson;
    const char *location = "test.json";
    samconfConfigBackend_t testBackend = {
        .backendHandle = NULL,
    };
    bool testBool = true;
    samconfConfig_t *config;

    TEST("samconfGetBackendOps with index of JSON Ops");
    SHOULD("%s", "return JSON Ops");

    MOCK_FUNC_AFTER_CALL(samconfJsonBackendSupports, 0);
    MOCK_FUNC_AFTER_CALL(samconfJsonBackendOpen, 0);
    MOCK_FUNC_AFTER_CALL(samconfJsonBackendLoad, 0);
    MOCK_FUNC_AFTER_CALL(samconfJsonBackendClose, 0);

    expect_string(samconfJsonBackendSupports, location, location);
    expect_string(samconfJsonBackendOpen, location, location);

    expect_value(samconfJsonBackendSupports, isSupported, &testBool);
    expect_value(samconfJsonBackendOpen, backend, &testBackend);
    expect_value(samconfJsonBackendLoad, backend, &testBackend);
    expect_value(samconfJsonBackendLoad, isSigned, false);
    expect_value(samconfJsonBackendLoad, config, &config);
    expect_value(samconfJsonBackendClose, backend, &testBackend);

    will_return(samconfJsonBackendSupports, SAMCONF_CONFIG_OK);
    will_return(samconfJsonBackendOpen, SAMCONF_CONFIG_OK);
    will_return(samconfJsonBackendLoad, SAMCONF_CONFIG_OK);
    will_return(samconfJsonBackendClose, SAMCONF_CONFIG_OK);

    opsJson = samconfGetBackendOps(0);

    assert_int_equal(opsJson->supports(location, &testBool), SAMCONF_CONFIG_OK);
    assert_int_equal(opsJson->open(location, &testBackend), SAMCONF_CONFIG_OK);
    assert_int_equal(opsJson->load(&testBackend, false, &config), SAMCONF_CONFIG_OK);
    assert_int_equal(opsJson->close(&testBackend), SAMCONF_CONFIG_OK);
}
