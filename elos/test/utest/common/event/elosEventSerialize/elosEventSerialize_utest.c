// SPDX-License-Identifier: MIT
#include "elosEventSerialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventSerializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventSerializeErrorNullEvent),
        TEST_CASE(elosTestElosEventSerializeErrorNullStorageString),
        TEST_CASE(elosTestElosEventSerializeNewJsonObjectError),
        TEST_CASE(elosTestElosEventSerializeEventConversionError),
        TEST_CASE(elosTestElosEventSerializeJsonObjStringError),
        TEST_CASE(elosTestElosEventSerializeStrdupError),
        TEST_CASE(elosTestElosEventSerializeSuccessEmptyEvent),
        TEST_CASE(elosTestElosEventSerializeSuccessNullEventSource),
        TEST_CASE(elosTestElosEventSerializeSuccessOnlySeverity),
        TEST_CASE(elosTestElosEventSerializeSuccessOnlySource),
        TEST_CASE(elosTestElosEventSerializeSuccessOnlyTime),
    };

    return RUN_TEST_SUITE(tests, elosEventSerializeUtest);
}

static int elosEventSerializeUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosEventSerializeUtestTeardown(UNUSED void **state) {
    return 0;
}
