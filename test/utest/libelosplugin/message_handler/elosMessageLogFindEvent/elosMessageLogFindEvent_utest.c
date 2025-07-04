// SPDX-License-Identifier: MIT
#include "elosMessageLogFindEvent_utest.h"

#include <elos/event/event.h>
#include <string.h>

TEST_SUITE_FUNC_PROTOTYPES(elosMessageLogFindEventUtest)

#define MOCK_STRING_LEN  40
#define MOCK_DISPATCH_ID 42

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosMessageLogFindEventUntruncatedSuccess),
        TEST_CASE(elosTestElosMessageLogFindEventTruncatedSuccess),
        TEST_CASE(elosTestElosMessageLogFindEventInvalidRequestFilterError),
        TEST_CASE(elosTestElosMessageLogFindEventNoRequestTimeSuccess),
        TEST_CASE(elosTestElosMessageLogFindEventError),
        TEST_CASE(elosTestElosMessageLogFindEventNullConnectionError),
        TEST_CASE(elosTestElosMessageLogFindEventNullMessageError),
        TEST_CASE(elosTestElosMessageLogFindEventNullMessageLengthError),
    };

    return RUN_TEST_SUITE(tests, elosMessageLogFindEventUtest);
}

static int elosMessageLogFindEventUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosMessageLogFindEventUtestTeardown(UNUSED void **state) {
    return 0;
}

int elosMessageLogFindEventVerifyResponse(const LargestIntegralType actual, const LargestIntegralType expected) {
    json_object *actualJsonResponse = (json_object *)actual;
    json_object *expectedJsonResponse = (json_object *)expected;
    int isEqual = json_object_equal(actualJsonResponse, expectedJsonResponse);
    if (isEqual == 0) {
        print_error("Request is not as expected: actual '%s' !=  expected '%s'\n",
                    json_object_to_json_string(actualJsonResponse), json_object_to_json_string(expectedJsonResponse));
    }
    return isEqual;
}

int elosMessageLogFindEventInjectEventList(const LargestIntegralType eventVector, const LargestIntegralType expected) {
    elosEventVector_t *events = (elosEventVector_t *)eventVector;
    json_object *expectedJsonResponse = (json_object *)expected;

    elosEventVector_t *injectedEvents = NULL;
    size_t eventCount = 0;
    json_object *jEventArray = safuJsonGetArray(expectedJsonResponse, "eventArray", 0, &eventCount);
    elosEventVectorFromJsonArray(jEventArray, &injectedEvents);

    for (size_t i = 0; i < eventCount; i++) {
        safuVecPush(events, safuVecGet(injectedEvents, i));
    }

    safuVecFree(injectedEvents);
    free(injectedEvents);

    return 1;
}
