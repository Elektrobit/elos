// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>
#include <samconf/config_backend.h>
#include <stdio.h>

#include "mock_samconf.h"
#include "samconfJsonBackendLoad_utest.h"

int samconfTestSamconfJsonBackendLoadErrorNullBackendSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfJsonBackendLoadErrorNullBackendTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfJsonBackendLoadErrorNullBackend(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *config;

    TEST("samconfJsonBackendLoad");
    SHOULD("%s", "returns error due to null backend");

    result = samconfJsonBackendLoad(NULL, false, &config);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
