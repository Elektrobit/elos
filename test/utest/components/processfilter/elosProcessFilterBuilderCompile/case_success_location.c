// SPDX-License-Identifier: MIT

#include "elosProcessFilterBuilderCompile_utest.h"

int elosTestEloProcessFilterBuilderCompileSuccessLocationSetup(void **state) {
    static elosTestState_t test;

    static elosStepTestEntry_t const testEntries[] = {
        {
            .input = ".process.uid",
            .stepCount = 1,
            .step = {_STEP(PROCESSFILTER_STEP_LDI_EV)},
            .stackCount = 1,
            .stack = {_STACK_U32(PROCESSFILTER_VALUE_PROCESS_UID)},
        },
        {
            .input = ".proc.uid",
            .stepCount = 1,
            .step = {_STEP(PROCESSFILTER_STEP_LDI_EV)},
            .stackCount = 1,
            .stack = {_STACK_U32(PROCESSFILTER_VALUE_PROCESS_UID)},
        },
        {
            .input = ".p.uid",
            .stepCount = 1,
            .step = {_STEP(PROCESSFILTER_STEP_LDI_EV)},
            .stackCount = 1,
            .stack = {_STACK_U32(PROCESSFILTER_VALUE_PROCESS_UID)},
        },
        {
            .input = ".proc.uid 1 GE",
            .stepCount = 3,
            .step = {_STEP(PROCESSFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_GE)},
            .stackCount = 2,
            .stack = {_STACK_U32(PROCESSFILTER_VALUE_PROCESS_UID), _STACK_I64(1)},
        },
        {
            .input = ".process.gid 1 EQ",
            .stepCount = 3,
            .step = {_STEP(PROCESSFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_EQ)},
            .stackCount = 2,
            .stack = {_STACK_U32(PROCESSFILTER_VALUE_PROCESS_GID), _STACK_I64(1)},
        },
        {
            .input = ".process.pid 1 EQ",
            .stepCount = 3,
            .step = {_STEP(PROCESSFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_EQ)},
            .stackCount = 2,
            .stack = {_STACK_U32(PROCESSFILTER_VALUE_PROCESS_PID), _STACK_I64(1)},
        },
        {
            .input = ".process.exec '/bin/elosc' STRCMP",
            .stepCount = 3,
            .step = {_STEP(PROCESSFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_STRCMP)},
            .stackCount = 2,
            .stack = {_STACK_U32(PROCESSFILTER_VALUE_PROCESS_EXEC), _STACK_STRING("/bin/elosc")},
        },
    };

    test.entry = testEntries;
    test.entryCount = ARRAY_SIZE(testEntries);
    *state = &test;

    return 0;
}

void elosTestEloProcessFilterBuilderCompileSuccessLocation(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    TEST("elosProcessFilterBuilderCompile");
    SHOULD("%s", "Properly translate tokens into instructions");

    result = elosVerifySteps(&test->data, test->entry, test->entryCount);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elosTestEloProcessFilterBuilderCompileSuccessLocationTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    if (test->data != NULL) {
        elosProcessFilterBuilderDelete(test->data);
    }

    return 0;
}
