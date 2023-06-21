// SPDX-License-Identifier: MIT
#include "elosEventClone_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventCloneErrEventSource), TEST_CASE(elosTestElosEventCloneErrEventDest),
        TEST_CASE(elosTestElosEventCloneExtErrEventNew), TEST_CASE(elosTestElosEventCloneExtErrEventDeepCopy),
        TEST_CASE(elosTestElosEventCloneSuccess)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
