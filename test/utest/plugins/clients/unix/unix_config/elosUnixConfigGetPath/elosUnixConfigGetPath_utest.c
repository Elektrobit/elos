// SPDX-License-Identifier: MIT
#include "elosUnixConfigGetPath_utest.h"

#include <samconf/samconf_types.h>

TEST_SUITE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetPathUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestelosUnixConfigGetPathSuccessFromEnv),
        TEST_CASE(elosTestelosUnixConfigGetPathSuccessFromDefault),
    };

    return RUN_TEST_SUITE(tests, elosTestelosUnixConfigGetPathUtest);
}

static int elosTestelosUnixConfigGetPathUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestelosUnixConfigGetPathUtestTeardown(UNUSED void **state) {
    return 0;
}
