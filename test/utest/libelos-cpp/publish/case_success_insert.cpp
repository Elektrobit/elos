// SPDX-License-Identifier: MIT
#include "publish_utest.h"

int elosTestPublishSuccessInsertSetup(UNUSED void **state) {
    return 0;
}

int elosTestPublishSuccessInsertTeardown(UNUSED void **state) {
    return 0;
}

void elosTestPublishSuccessInsert(UNUSED void **state) {
    using namespace elos;
    Event testEvent;
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
    testObject << testEvent;

    PARAM("Event Initializer List");
    testObject << Event{elosCTestEvent.date,           {strdup("appNameTest"), strdup("appNameTest"), 0},
                        elosCTestEvent.severity,       elosCTestEvent.hardwareid,
                        elosCTestEvent.classification, elosCTestEvent.messageCode,
                        elosCTestEvent.payload};
    MOCK_FUNC_DISABLE(elosEventPublish);
    MOCK_FUNC_DISABLE(elosSessionValid);
}
