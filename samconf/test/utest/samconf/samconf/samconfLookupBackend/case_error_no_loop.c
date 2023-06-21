// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "safu/mock_safu.h"
#include "samconfLookupBackend_utest.h"

void samconfTestSamconfLookupBackendErrorNoLoop(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfConfigBackend_t *backendPtr;

    const char *location = "config.json";

    TEST("samconfTestSamconfLookupBackend");
    SHOULD("%s", "fail when no backends are loaded");

    MOCK_FUNC_ALWAYS(samconfGetBackendOps);
    expect_any(samconfGetBackendOps, idx);
    will_return(samconfGetBackendOps, NULL);

    status = samconfLookupBackend(location, &backendPtr);

    MOCK_FUNC_DISABLE(samconfGetBackendOps);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
