// SPDX-License-Identifier: MIT
#include "elosRpnFilterClone_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testelosRpnFilterCloneErrAlloc),
        TEST_CASE(elos_testelosRpnFilterCloneErrDeepcopy),
        TEST_CASE(elos_testelosRpnFilterCloneErrParamNull),
        TEST_CASE(elos_testelosRpnFilterCloneSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
