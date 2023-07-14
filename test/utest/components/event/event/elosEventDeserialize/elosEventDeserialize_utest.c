// SPDX-License-Identifier: MIT
#include "elosEventDeserialize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventDeserializeEmptyInputString),
        TEST_CASE(elosTestElosEventDeserializeEmptyEvent),
        TEST_CASE(elosTestElosEventDeserializeEventConversionError),
        TEST_CASE(elosTestElosEventDeserializeJsonParseError),
        TEST_CASE(elosTestElosEventDeserializeSuccessNullEventSource),
        TEST_CASE(elosTestElosEventDeserializeSuccessNullEvent),
        TEST_CASE(elosTestElosEventDeserializeSuccessSeverityOnly),
        TEST_CASE(elosTestElosEventDeserializeSuccessSourceOnly),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
