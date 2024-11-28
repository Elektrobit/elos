// SPDX-License-Identifier: MIT
#include <ctime>

#include "publish_utest.h"

int elosTestPublishErrSessionSetup(UNUSED void **state) {
    return 0;
}

int elosTestPublishErrSessionTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPublishErrSession(UNUSED void **state) {
    using namespace elos;
    Event testEvent;
    elosResultE result;
    Elos testObject;
    struct timespec testTime;

    TEST("publish");
    SHOULD("%s", "not publish an event, since session is invalid");

    MOCK_FUNC_ALWAYS(elosSessionValid);
    expect_any_always(elosSessionValid, session);
    will_return_always(elosSessionValid, false);

    int ret = clock_gettime(CLOCK_REALTIME, &testTime);
    assert_int_equal(ret, 0);

    PARAM("Default Event");
    result = testObject.publish(testEvent, true);
    assert_int_equal(result, ELOS_RESULT_FAILED);

    PARAM("Event Initializer List");
    result = testObject.publish({elosCTestEvent.date,
                                 {strdup("appNameTest"), strdup("appNameTest"), 0},
                                 elosCTestEvent.severity,
                                 elosCTestEvent.hardwareid,
                                 elosCTestEvent.classification,
                                 elosCTestEvent.messageCode,
                                 elosCTestEvent.payload},
                                true);
    assert_int_equal(result, ELOS_RESULT_FAILED);
    MOCK_FUNC_DISABLE(elosEventPublish);
    MOCK_FUNC_DISABLE(elosSessionValid);
}
