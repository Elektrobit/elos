// SPDX-License-Identifier: MIT
#include "elosEventToJsonObject_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventToJsonObjectEventNullError),
        TEST_CASE(elosTestElosEventToJsonObjectJobjNullError),
        TEST_CASE(elosTestElosEventToJsonObjectClassificationError),
        TEST_CASE(elosTestElosEventToJsonObjectDateError),
        TEST_CASE(elosTestElosEventToJsonObjectHardwareIdError),
        TEST_CASE(elosTestElosEventToJsonObjectMessageCodeError),
        TEST_CASE(elosTestElosEventToJsonObjectPayloadError),
        TEST_CASE(elosTestElosEventToJsonObjectSeverityError),
        TEST_CASE(elosTestElosEventToJsonObjectSourceAddObjectError),
        TEST_CASE(elosTestElosEventToJsonObjectSourceError),
        TEST_CASE(elosTestElosEventToJsonObjectJsonObjectError),
        TEST_CASE(elosTestElosEventToJsonObjectSuccessNullEventSource),
        TEST_CASE(elosTestElosEventToJsonObjectSuccessSeverityOnly),
        TEST_CASE(elosTestElosEventToJsonObjectSuccessValidTimeValues),
        TEST_CASE(elosTestElosEventToJsonObjectSuccessZeroInitialized),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
