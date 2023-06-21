// SPDX-License-Identifier: MIT
#include "elosEventDelete_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventDeleteWrongParameter),
        TEST_CASE(elosTestElosEventDeleteSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
