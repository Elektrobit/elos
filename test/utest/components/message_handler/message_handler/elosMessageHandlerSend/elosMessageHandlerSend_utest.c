// SPDX-License-Identifier: MIT
#include "elosMessageHandlerSend_utest.h"

int main(UNUSED int argc, UNUSED char **argv) {
    const struct CMUnitTest unitTests[] = {cmocka_unit_test(elosTestElosMessageHandlerSendSuccess),
                                           cmocka_unit_test(elosTestElosMessageHandlerSendExterrAllocMem),
                                           cmocka_unit_test(elosTestElosMessageHandlerSendErrBytesLtZero),
                                           cmocka_unit_test(elosTestElosMessageHandlerSendErrBytesLtMsgLen)};

    return cmocka_run_group_tests(unitTests, NULL, NULL);
}
