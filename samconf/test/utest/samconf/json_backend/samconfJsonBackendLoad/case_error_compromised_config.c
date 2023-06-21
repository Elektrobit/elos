// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>
#include <samconf/config_backend.h>
#include <stdio.h>

#include "mock_samconf.h"
#include "safu/mock_log.h"
#include "samconfJsonBackendLoad_utest.h"

int samconfTestSamconfJsonBackendLoadErrorCompromisedConfigSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfJsonBackendLoadErrorCompromisedConfigTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfJsonBackendLoadErrorCompromisedConfig(UNUSED void **state) {
    samconfConfig_t *config;
    samconfConfigBackend_t backend = {
        .originalHandle = NULL,
        .backendHandle = NULL,
    };
    samconfConfigStatusE_t result;

    TEST("samconfJsonBackendLoad");
    SHOULD("%s", "return the error code of configNew, when it fails");

    MOCK_FUNC_AFTER_CALL(samconfConfigNew, 0);

    expect_value(samconfConfigNew, config, &config);

    will_return(samconfConfigNew, 400);

    result = samconfJsonBackendLoad(&backend, false, &config);

    assert_int_equal(result, 400);
}
