// SPDX-License-Identifier: MIT

#include "elosDltMapperMapDataToEvent_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosDltMapperMapDataToEventUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosDltMapperMapDataToEventSuccess),
        TEST_CASE(elosTestElosDltMapperMapDataToEventError),
        TEST_CASE(elosTestElosDltMapperMapDataToEventErrorDefaultValues),
    };

    return RUN_TEST_SUITE(tests, elosDltMapperMapDataToEventUtest);
}

static int elosDltMapperMapDataToEventUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosDltMapperMapDataToEventUtestTeardown(UNUSED void **state) {
    return 0;
}
