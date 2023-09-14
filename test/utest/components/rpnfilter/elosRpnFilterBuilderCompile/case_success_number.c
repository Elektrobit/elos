// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderCompile_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

#define _VALUE0 1
#define _VALUE1 2
#define _VALUE2 3

int elos_testElosRpnFilterBuilderCompileSuccessNumberSetup(void **state) {
    static elosTestState_t test;
    static elosStepTestEntry_t const testEntries[] = {
        {
            .input = _STR(_VALUE0),
            .stepCount = 1,
            .step = {_STEP(RPNFILTER_STEP_LDV)},
            .stackCount = 1,
            .stack =
                {
                    _STACK_I64(_VALUE0),
                },
        },
        {
            .input = _STR(_VALUE1) " " _STR(_VALUE0),
            .stepCount = 2,
            .step = {_STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_LDV)},
            .stackCount = 2,
            .stack =
                {
                    _STACK_I64(_VALUE1),
                    _STACK_I64(_VALUE0),
                },
        },
        {
            .input = _STR(_VALUE2) " " _STR(_VALUE1) " " _STR(_VALUE0),
            .stepCount = 3,
            .step = {_STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_LDV)},
            .stackCount = 3,
            .stack =
                {
                    _STACK_I64(_VALUE2),
                    _STACK_I64(_VALUE1),
                    _STACK_I64(_VALUE0),
                },
        },
        {
            .input = _STR(_VALUE0) " " _STR(_VALUE1) " " _STR(_VALUE2) " " _STR(_VALUE0),
            .stepCount = 4,
            .step =
                {
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
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

void elos_testElosRpnFilterBuilderCompileSuccessNumber(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "Properly translate tokens into instructions");

    result = elosVerifySteps(&test->data, test->entry, test->entryCount);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterBuilderCompileSuccessNumberTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    if (test->data != NULL) {
        elosRpnFilterBuilderDelete(test->data);
    }

    return 0;
}
