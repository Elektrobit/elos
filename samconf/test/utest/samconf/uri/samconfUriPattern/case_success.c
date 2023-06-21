// SPDX-License-Identifier: MIT

#include <regex.h>
#include <samconf/uri.h>

#include "cmocka_mocks/mock_libc.h"
#include "mock_samconf.h"
#include "samconfUriPattern_utest.h"

int samconfTestSamconfUriPatternSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriPatternSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriPatternSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    regex_t *pattern = (void *)0xdeadb33f;

    TEST("samconfUriPattern");
    SHOULD("%s", "return pointer to new memory of configuration signature uri");

    MOCK_FUNC_AFTER_CALL(regcomp, 0);
    expect_any(__wrap_regcomp, preg);
    expect_string(__wrap_regcomp, regex, SAMCONF_URI_PATTERN);
    expect_value(__wrap_regcomp, cflags, REG_EXTENDED);
    will_return(__wrap_regcomp, 0);

    status = samconfUriPattern(&pattern);

    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_ptr_not_equal(pattern, (void *)0xdeadb33f);
}
