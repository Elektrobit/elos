// SPDX-License-Identifier: MIT

#include "elosEventVectorPushDeepCopy_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorPushDeepCopyUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventVectorPushDeepCopyMock),
                                       TEST_CASE(elosTestElosEventVectorPushDeepCopyCopyFailed),
                                       TEST_CASE(elosTestElosEventVectorPushDeepCopySuccess)};

    return RUN_TEST_SUITE(tests, elosEventVectorPushDeepCopyUtest);
}

static int elosEventVectorPushDeepCopyUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosEventVectorPushDeepCopyUtestTeardown(UNUSED void **state) {
    return 0;
}
