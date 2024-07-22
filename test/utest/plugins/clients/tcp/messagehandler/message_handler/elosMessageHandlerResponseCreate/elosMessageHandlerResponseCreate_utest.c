// SPDX-License-Identifier: MIT
#include "elosMessageHandlerResponseCreate_utest.h"

int main(UNUSED int argc, UNUSED char **argv) {
    const struct CMUnitTest unitTests[] = {
        cmocka_unit_test(elosTestElosMessageHandlerResponseCreateSuccessNullString),
        cmocka_unit_test(elosTestElosMessageHandlerResponseCreateSuccessErrString),
        cmocka_unit_test(elosTestElosMessageHandlerResponseCreateExterrJsonObject),
        cmocka_unit_test(elosTestElosMessageHandlerResponseCreateExterrAddObject),
        cmocka_unit_test(elosTestElosMessageHandlerResponseCreateExterrAddNewString)};

    return cmocka_run_group_tests(unitTests, NULL, NULL);
}
