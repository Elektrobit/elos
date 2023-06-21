// SPDX-License-Identifier: MIT

#include <regex.h>
#include <samconf/uri.h>

#include "cmocka_mocks/mock_libc.h"
#include "mock_samconf.h"
#include "samconfUriPattern_utest.h"

int samconfTestSamconfUriPatternRegCompErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriPatternRegCompErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriPatternRegCompError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    regex_t *pattern = (void *)0xdeadb33f;

    TEST("samconfUriPattern");
    SHOULD("%s", "return error if uri pattern compilation failed");

    MOCK_FUNC_AFTER_CALL(regcomp, 0);
    expect_any(__wrap_regcomp, preg);
    expect_string(__wrap_regcomp, regex, SAMCONF_URI_PATTERN);
    expect_value(__wrap_regcomp, cflags, REG_EXTENDED);
    will_return(__wrap_regcomp, -1);

    MOCK_FUNC_AFTER_CALL(regerror, 0);
    expect_value(__wrap_regerror, errcode, -1);
    expect_any(__wrap_regerror, preg);
    expect_any(__wrap_regerror, errbuf);
    expect_value(__wrap_regerror, errbuf_size, 256);
    will_return(__wrap_regerror, 0);

    status = samconfUriPattern(&pattern);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_ptr_equal(pattern, (void *)0xdeadb33f);
}
