// SPDX-License-Identifier: MIT
#include "elosUnixConfigGetElosdConnectionLimit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetElosdConnectionLimitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromEnv),
        TEST_CASE(elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromDefault),
        TEST_CASE(elosTestelosUnixConfigGetElosdConnectionLimitErrorLimitNotIntInEnv),
    };

    return RUN_TEST_SUITE(tests, elosTestelosUnixConfigGetElosdConnectionLimitUtest);
}

static int elosTestelosUnixConfigGetElosdConnectionLimitUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestelosUnixConfigGetElosdConnectionLimitUtestTeardown(UNUSED void **state) {
    return 0;
}
