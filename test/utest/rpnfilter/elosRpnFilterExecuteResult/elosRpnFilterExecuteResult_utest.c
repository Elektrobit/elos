// SPDX-License-Identifier: MIT
#include "elosRpnFilterExecuteResult_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elos_testElosRpnFilterExecuteResultErrParam),
        TEST_CASE(elos_testElosRpnFilterExecuteResultSuccessMatchFalse),
        TEST_CASE(elos_testElosRpnFilterExecuteResultSuccessMatchTrue),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
