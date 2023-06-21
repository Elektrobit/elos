// SPDX-License-Identifier: MIT

#include <samconf/uri.h>

#include "mock_samconf.h"
#include "safu/mock_safu.h"
#include "samconfUriNew_utest.h"

int samconfTestSamconfUriNewInitErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriNewInitErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriNewInitError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfUri_t *uri = (void *)0xc0fed00d;
    const char *uriString = (void *)0xdeadb33f;

    TEST("samconfUriNew");
    SHOULD("%s", "receive error while initializing the configuration signature uri");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(*uri));
    will_return(__wrap_safuAllocMem, (void *)0xbaaaaaad);

    MOCK_FUNC_AFTER_CALL(samconfUriInit, 0);
    expect_value(samconfUriInit, uri, (void *)0xbaaaaaad);
    expect_value(samconfUriInit, uriString, uriString);
    will_return(samconfUriInit, SAMCONF_CONFIG_ERROR);

    status = samconfUriNew(&uri, uriString);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(uri, (void *)0xc0fed00d);
}
