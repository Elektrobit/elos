// SPDX-License-Identifier: MIT
#include "elosUnixConfigGetSocketAddress_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetSocketAddressUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestelosUnixConfigGetSocketAddressSuccessPathExist),
        TEST_CASE(elosTestelosUnixConfigGetSocketAddressSuccessCreateDir),
        TEST_CASE(elosTestelosUnixConfigGetSocketAddressErrNoAccess),
    };

    return RUN_TEST_SUITE(tests, elosTestelosUnixConfigGetSocketAddressUtest);
}

static int elosTestelosUnixConfigGetSocketAddressUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestelosUnixConfigGetSocketAddressUtestTeardown(UNUSED void **state) {
    return 0;
}
