// SPDX-License-Identifier: MIT

#include <samconf/uri.h>

#include "mock_samconf.h"
#include "safu/mock_safu.h"
#include "samconfUriNew_utest.h"

int samconfTestSamconfUriNewSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriNewSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriNewSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfUri_t *uri, stackInst;
    const char *uriString = (void *)0xdeadb33f;

    TEST("samconfUriNew");
    SHOULD("%s", "return pointer to new memory of configuration signature uri");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(*uri));
    will_return(__wrap_safuAllocMem, &stackInst);

    MOCK_FUNC_AFTER_CALL(samconfUriInit, 0);
    expect_value(samconfUriInit, uri, &stackInst);
    expect_value(samconfUriInit, uriString, uriString);
    will_return(samconfUriInit, SAMCONF_CONFIG_OK);

    status = samconfUriNew(&uri, uriString);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_ptr_equal(uri, &stackInst);
}
