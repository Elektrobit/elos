// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "safu/mock_safu.h"
#include "samconfLookupBackend_utest.h"

static int samconfSupportsDummyCounter;
static samconfConfigStatusE_t samconfSupportsDummyError(UNUSED const char *location, UNUSED bool *isSupported) {
    samconfSupportsDummyCounter++;
    return SAMCONF_CONFIG_INVALID_TYPE;  // something besides OK and ERROR
}

static samconfConfigStatusE_t samconfSupportsDummyFalse(UNUSED const char *location, bool *isSupported) {
    samconfSupportsDummyCounter++;
    *isSupported = false;
    return SAMCONF_CONFIG_OK;
}

static samconfConfigBackendOps_t samconfDummyOps = {
    .supports = samconfSupportsDummyError,
    .open = samconfOpenDummy,
    .load = samconfLoadDummy,
    .close = samconfCloseDummy,
};

void samconfTestSamconfLookupBackendErrorSupportFail(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t *backendPtr;
    const char *location = "config.json";

    TEST("samconfTestSamconfLookupBackend");
    SHOULD("%s", "fail on unsuccesful supports callback call");

    MOCK_FUNC_ALWAYS(samconfGetBackendOps);

    // Test fail on error return
    // HINT: Some optimization seems to reduce the calls to 1
    expect_value(samconfGetBackendOps, idx, 0);
    expect_value(samconfGetBackendOps, idx, 1);
    expect_value(samconfGetBackendOps, idx, 2);

    will_return_count(samconfGetBackendOps, &samconfDummyOps, 2);
    will_return(samconfGetBackendOps, NULL);

    status = samconfLookupBackend(location, &backendPtr);
    assert_int_equal(samconfSupportsDummyCounter, 2);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);

    // Reset callback counter
    samconfSupportsDummyCounter = 0;

    // Test fail on supports false
    samconfDummyOps.supports = samconfSupportsDummyFalse;

    expect_value(samconfGetBackendOps, idx, 0);
    expect_value(samconfGetBackendOps, idx, 1);
    expect_value(samconfGetBackendOps, idx, 2);

    will_return_count(samconfGetBackendOps, &samconfDummyOps, 2);
    will_return(samconfGetBackendOps, NULL);

    status = samconfLookupBackend(location, &backendPtr);
    assert_int_equal(samconfSupportsDummyCounter, 2);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);

    MOCK_FUNC_DISABLE(samconfGetBackendOps);
}
