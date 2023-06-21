// SPDX-License-Identifier: MIT

#include <samconf/uri.h>

#include "cmocka_mocks/mock_libc.h"
#include "samconfUriDeleteMembers_utest.h"

int samconfTestSamconfUriDeleteMembersSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriDeleteMembersSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    samconfUri_t uri = {
        .plain = (void *)0xdeadda1a,
        .scheme = (void *)0xdeadda2a,
        .authority = (void *)0xdeadda3a,
        .path = (void *)0xdeadda4a,
        .query = (void *)0xdeadda5a,
        .fragment = (void *)0xdeadda6a,
    };

    TEST("samconfUriDeleteMembers");
    SHOULD("%s", "free the members of the given uri");

    MOCK_FUNC_ALWAYS(free);
    expect_value(__wrap_free, ptr, uri.plain);
    expect_value(__wrap_free, ptr, uri.scheme);
    expect_value(__wrap_free, ptr, uri.authority);
    expect_value(__wrap_free, ptr, uri.path);
    expect_value(__wrap_free, ptr, uri.query);
    expect_value(__wrap_free, ptr, uri.fragment);

    status = samconfUriDeleteMembers(&uri);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
}
