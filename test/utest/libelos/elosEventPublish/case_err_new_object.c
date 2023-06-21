// SPDX-License-Identifier: MIT

#include "elosEventPublish_utest.h"
#include "mock_event.h"

int elosTestElosEventPublishErrNewObjectSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventPublishErrNewObjectTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventPublishErrNewObject(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosUnitTestSet_t *tSet;
    safuResultE_t result;

    TEST("elosEventPublish");
    SHOULD("%s", "return SAFU_RESULT_FAILED when json_object_new_object failed");

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);
    will_return(__wrap_json_object_new_object, NULL);

    tSet = &test->normal;
    result = elosEventPublish(&test->session, &tSet->event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
