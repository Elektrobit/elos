// SPDX-License-Identifier: MIT
#include "elosLogCreateElosEventFromLog_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLogCreateElosEventFromLogUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosLogCreateElosEventFromLogSuccess),
        TEST_CASE(elosLogCreateElosEventFromLogErrParamLogData),
        TEST_CASE(elosLogCreateElosEventFromLogErrParamEvent),
        TEST_CASE(elosLogCreateElosEventFromLogExtErrEvent),
    };
    return RUN_TEST_SUITE(tests, elosLogCreateElosEventFromLogUtest);
}

static int elosLogCreateElosEventFromLogUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLogCreateElosEventFromLogUtestTeardown(UNUSED void **state) {
    return 0;
}
