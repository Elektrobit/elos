// SPDX-License-Identifier: MIT
#include "elosMessageEventPublish_utest.h"

#include <string.h>

TEST_SUITE_FUNC_PROTOTYPES(elosMessageEventPublishUtest)

#define MOCK_STRING_LEN  40
#define MOCK_DISPATCH_ID 42

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosMessageEventPublishSuccess),
        TEST_CASE(elosTestelosMessageEventPublishBlacklistFilterSuccess),
        TEST_CASE(elosTestelosMessageEventPublishBlacklistFilterCreateError),
        TEST_CASE(elosTestElosMessageEventPublishConnectionNullError),
        TEST_CASE(elosTestElosMessageEventPublishMessageNullError),
        TEST_CASE(elosTestElosMessageEventPublishEventEmpty),
        TEST_CASE(elosTestElosMessageEventPublishLoggingFailed),
        TEST_CASE(elosTestElosMessageEventPublishPublishFailed),
    };

    return RUN_TEST_SUITE(tests, elosMessageEventPublishUtest);
}

static int elosMessageEventPublishUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosMessageEventPublishUtestTeardown(UNUSED void **state) {
    return 0;
}
