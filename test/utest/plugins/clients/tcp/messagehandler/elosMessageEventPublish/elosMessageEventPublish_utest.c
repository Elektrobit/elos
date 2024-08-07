// SPDX-License-Identifier: MIT
#include "elosMessageEventPublish_utest.h"

#include <elos/event/event.h>
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

int elosMessageEventPublishCheckJsonObject(const long unsigned int parameter, const long unsigned int compare) {
    json_object *jobjParam = (json_object *)parameter;
    json_object *jobjCmp = (json_object *)compare;
    if (json_object_equal(jobjParam, jobjCmp)) {
        return 1;
    }
    print_error("%s != %s\n", json_object_to_json_string(jobjParam), json_object_to_json_string(jobjCmp));
    return 0;
}

int elosMessageEventPublishCheckEvent(const elosEvent_t *expected, const elosEvent_t *actual) {
    int result = 1;
    json_object *eventParam = json_object_new_object();
    json_object *eventCmp = json_object_new_object();
    safuResultE_t res = elosEventToJsonObject(eventParam, expected);
    if (res != SAFU_RESULT_OK) {
        result = 0;
    }
    res = elosEventToJsonObject(eventCmp, actual);
    if (res != SAFU_RESULT_OK) {
        result = 0;
    }
    if (result == 1) {
        result = elosMessageEventPublishCheckJsonObject((const long unsigned int)eventParam,
                                                        (const long unsigned int)eventCmp);
    }

    json_object_put(eventParam);
    json_object_put(eventCmp);

    return result;
}
