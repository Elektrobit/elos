// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderFinalize_utest.h"

#define _VALUE0 1
#define _VALUE1 2
#define _VALUE2 3

int elos_testElosRpnFilterBuilderFinalizeSuccessSetup(void **state) {
    static elosTestFinalize_t test;

    static elosTestFinalizeEntry_t const testEntries[] = {
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
        {
            .input = "'hello123' '[0-9]' REGEX",
            .stepCount = 3,
            .step =
                {
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_LDV),
                    _STEP(RPNFILTER_STEP_REGEX),
                },
            .stackCount = 2,
            .stack =
                {
                    _STACK(RPNFILTER_VALUE_STRING, 8, "hello123"),
                    _STACK(RPNFILTER_VALUE_STRING, 5, "[0-9]"),
                },
        },
    };

    test.entry = testEntries;
    test.entryCount = ARRAY_SIZE(testEntries);

    *state = &test;

    return 0;
}

void elos_testElosRpnFilterBuilderFinalizeSuccess(void **state) {
    elosTestFinalize_t *test = *(elosTestFinalize_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "Properly construct a useable filter from compile data");

    result = elosVerifyFinalize(test);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterBuilderFinalizeSuccessTeardown(void **state) {
    elosTestFinalize_t *test = *(elosTestFinalize_t **)state;

    elosRpnFilterBuilderDelete(test->data);
    elosRpnFilterDelete(test->filter);
    return 0;
}
