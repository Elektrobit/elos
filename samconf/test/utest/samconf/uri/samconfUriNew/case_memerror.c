// SPDX-License-Identifier: MIT

#include <samconf/uri.h>

#include "safu/mock_safu.h"
#include "samconfUriNew_utest.h"

int samconfTestSamconfUriNewMemErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriNewMemErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriNewMemError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfUri_t *uri = (void *)0xc0fed00d;
    const char *uriString = (void *)0xdeadb33f;

    TEST("samconfUriNew");
    SHOULD("%s", "receive error while allocating memory for the configuration signature uri");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(*uri));
    will_return(__wrap_safuAllocMem, NULL);

    status = samconfUriNew(&uri, uriString);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(uri, (void *)0xc0fed00d);
}
