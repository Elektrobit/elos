// SPDX-License-Identifier: MIT
#include <ctime>

#include "publish_utest.h"

int elosTestPublishSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestPublishSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPublishSuccess(UNUSED void **state) {
    using namespace elos;
    Event testEvent;
    elosResultE result;
    Elos testObject;

    TEST("publish");
    SHOULD("%s", "successfully publish an event");

    MOCK_FUNC_ALWAYS(elosEventPublish);
    expect_any_always(elosEventPublish, session);
    expect_any_always(elosEventPublish, event);
    will_return_always(elosEventPublish, ELOS_RESULT_OK);

    MOCK_FUNC_ALWAYS(elosSessionValid);
    expect_any_always(elosSessionValid, session);
    will_return_always(elosSessionValid, true);

    PARAM("Default Event");
    result = testObject.publish(testEvent, true);
    assert_int_equal(result, ELOS_RESULT_OK);

    PARAM("Event Initializer List");
    result = testObject.publish({elosCTestEvent.date,
                                 {strdup("appNameTest"), strdup("appNameTest"), 0},
                                 elosCTestEvent.severity,
                                 elosCTestEvent.hardwareid,
                                 elosCTestEvent.classification,
                                 elosCTestEvent.messageCode,
                                 elosCTestEvent.payload},
                                true);
    assert_int_equal(result, ELOS_RESULT_OK);
    MOCK_FUNC_DISABLE(elosEventPublish);
    MOCK_FUNC_DISABLE(elosSessionValid);
}
