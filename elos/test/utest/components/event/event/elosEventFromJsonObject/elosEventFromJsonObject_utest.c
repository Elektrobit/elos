// SPDX-License-Identifier: MIT
#include "elosEventFromJsonObject_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventFromJsonObjectNUllInputJobj),
        TEST_CASE(elosTestElosEventFromJsonObjectNUllInputEvent),
        TEST_CASE(elosTestElosEventFromJsonObjectClassificationError),
        TEST_CASE(elosTestElosEventFromJsonObjectDateError),
        TEST_CASE(elosTestElosEventFromJsonObjectHardwareIdError),
        TEST_CASE(elosTestElosEventFromJsonObjectMessageCodeError),
        TEST_CASE(elosTestElosEventFromJsonObjectPayloadError),
        TEST_CASE(elosTestElosEventFromJsonObjectSeverityError),
        TEST_CASE(elosTestElosEventFromJsonObjectSourceError),
        TEST_CASE(elosTestElosEventFromJsonObjectSuccessEmptyJObject),
        TEST_CASE(elosTestElosEventFromJsonObjectSuccessNullEventSource),
        TEST_CASE(elosTestElosEventFromJsonObjectSuccessSeverityOnly),
        TEST_CASE(elosTestElosEventFromJsonObjectSuccessSourceOnly),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
