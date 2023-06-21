// SPDX-License-Identifier: MIT

#include <samconf/uri.h>

#include "cmocka_mocks/mock_libc.h"
#include "mock_samconf.h"
#include "samconfUriDelete_utest.h"

int samconfTestSamconfUriDeleteSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriDeleteSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfUri_t *uri = (void *)0xdeadd474;

    TEST("samconfUriDelete");
    SHOULD("%s", "free the given uri");

    MOCK_FUNC_AFTER_CALL(samconfUriDeleteMembers, 0);
    expect_value(samconfUriDeleteMembers, uri, uri);
    will_return(samconfUriDeleteMembers, SAMCONF_CONFIG_OK);

    MOCK_FUNC_ALWAYS(free);
    expect_value(__wrap_free, ptr, uri);

    status = samconfUriDelete(uri);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
}
