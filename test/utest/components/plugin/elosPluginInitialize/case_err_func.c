// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <errno.h>

#include "elosPluginInitialize_utest.h"

int elosTestElosPluginInitializeErrFuncSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginInitializeErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosPluginDeleteMembers(&test->plugin);
    return 0;
}

void elosTestElosPluginInitializeErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosPluginParam_t param = {0};
    safuResultE_t result;

    TEST("elosPluginInitialize");
    SHOULD("%s", "test correct behaviour when function calls fail");

    for (int i = 0; i < 3; i += 1) {
        PARAM("eventfd_create call %d fails", (i + 1));
        MOCK_FUNC_AFTER_CALL(eventfd, i);
        expect_value(__wrap_eventfd, __count, 0);
        expect_value(__wrap_eventfd, __flags, 0);
        will_return(__wrap_eventfd, -1);

        result = elosPluginInitialize(&test->plugin, &param);
        assert_int_equal(result, SAFU_RESULT_FAILED);
    }

    for (int i = 0; i < 2; i += 1) {
        param.file = "/dev/null";

        PARAM("strdup call %d fails", (i + 1));
        MOCK_FUNC_AFTER_CALL(strdup, i);
        expect_any(__wrap_strdup, string);
        will_return(__wrap_strdup, NULL);

        result = elosPluginInitialize(&test->plugin, &param);
        assert_int_equal(result, SAFU_RESULT_FAILED);
    }
}
