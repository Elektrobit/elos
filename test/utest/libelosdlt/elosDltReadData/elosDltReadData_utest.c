// SPDX-License-Identifier: MIT

#include "elosDltReadData_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosDltReadDataUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosDltReadDataSuccess),
        TEST_CASE(elosTestElosDltReadDataErrorInvalidMsg),
        TEST_CASE(elosTestElosDltReadDataErrorIncompleteMsg),
        TEST_CASE(elosTestElosDltReadDataErrorIncompleteHdr),
    };

    return RUN_TEST_SUITE(tests, elosDltReadDataUtest);
}

static int elosDltReadDataUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosDltReadDataUtestTeardown(UNUSED void **state) {
    return 0;
}
