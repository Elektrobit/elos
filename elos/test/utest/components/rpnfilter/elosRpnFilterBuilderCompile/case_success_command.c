// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderCompile_utest.h"

#define _VALUE0 1
#define _VALUE1 2
#define _VALUE2 3

int elos_testElosRpnFilterBuilderCompileSuccessCommandSetup(void **state) {
    static elosTestState_t test;
    static elosStepTestEntry_t const testEntries[] = {
        {
            .input = "1 2 AND",
            .stepCount = 3,
            .step =
                {
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_AND),
                },
            .stackCount = 2,
            .stack =
                {
                    _STACK_I64(_VALUE0),
                    _STACK_I64(_VALUE1),
                },
        },
        {
            .input = "1 2 AND 3 EQ",
            .stepCount = 5,
            .step =
                {
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_AND),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_EQ),
                },
            .stackCount = 3,
            .stack =
                {
                    _STACK_I64(_VALUE0),
                    _STACK_I64(_VALUE1),
                    _STACK_I64(_VALUE2),
                },
        },
        {
            .input = "'foo' 'bar' STRCMP 1 OR",
            .stepCount = 5,
            .step =
                {
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_STRCMP),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_OR),
                },
            .stackCount = 3,
            .stack =
                {
                    _STACK(RPNFILTER_VALUE_STRING, 3, "foo"),
                    _STACK(RPNFILTER_VALUE_STRING, 3, "bar"),
                    _STACK_I64(_VALUE0),
                },
        },
        {
            .input = "1 2 ADD 3 SUB NOT 1 LT",
            .stepCount = 8,
            .step =
                {
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_ADD),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_SUB),
                    _STEP(RPNFILTER_STEP_NOT),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LT),
                },
            .stackCount = 4,
            .stack =
                {
                    _STACK_I64(_VALUE0),
                    _STACK_I64(_VALUE1),
                    _STACK_I64(_VALUE2),
                    _STACK_I64(_VALUE0),
                },
        },
    };

    test.entry = testEntries;
    test.entryCount = ARRAY_SIZE(testEntries);
    *state = &test;

    return 0;
}

void elos_testElosRpnFilterBuilderCompileSuccessCommand(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "Properly translate tokens into instructions");

    result = elosVerifySteps(&test->data, test->entry, test->entryCount);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterBuilderCompileSuccessCommandTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    if (test->data != NULL) {
        elosRpnFilterBuilderDelete(test->data);
    }

    return 0;
}
