// SPDX-License-Identifier: MIT
#include "elosEventDeepCopy_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventDeepCopyNullInput),
        TEST_CASE(elosTestElosEventDeepCopyEventSourceDeepCopyError),
        TEST_CASE(elosTestElosEventStrdupHardwareIdError),
        TEST_CASE(elosTestElosEventStrdupPayloadError),
        TEST_CASE(elosTestElosEventSuccessEventSourceNotNull),
        TEST_CASE(elosTestElosEventSuccessNullEventMemberStrings),
        TEST_CASE(elosTestElosEventSuccessNullEventSource),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
