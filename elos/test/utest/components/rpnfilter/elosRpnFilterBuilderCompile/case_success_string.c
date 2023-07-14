// SPDX-License-Identifier: MIT

#include <string.h>

#include "elosRpnFilterBuilderCompile_utest.h"

#define _VALUE0 "t"
#define _VALUE1 "foo"
#define _VALUE2 "bar"
#define _VALUE3 "BEEF 123"

int elos_testElosRpnFilterBuilderCompileSuccessStringSetup(void **state) {
    static elosTestState_t test;

    static elosStepTestEntry_t const testEntries[] = {
        {
            .input = "'" _VALUE0 "'",
            .stepCount = 1,
            .step = {_STEP(RPNFILTER_STEP_LDV)},
            .stackCount = 1,
            .stack =
                {
                    _STACK(RPNFILTER_VALUE_STRING, 1, _VALUE0),
                },
        },
        {
            .input = "'" _VALUE0 "' '" _VALUE1 "'",
            .stepCount = 2,
            .step = {_STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_LDV)},
            .stackCount = 2,
            .stack =
                {
                    _STACK(RPNFILTER_VALUE_STRING, 1, _VALUE0),
                    _STACK(RPNFILTER_VALUE_STRING, 3, _VALUE1),
                },
        },
        {
            .input = "'" _VALUE1 "' '" _VALUE2 "' '" _VALUE3 "'",
            .stepCount = 3,
            .step = {_STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_LDV)},
            .stackCount = 3,
            .stack =
                {
                    _STACK(RPNFILTER_VALUE_STRING, 3, _VALUE1),
                    _STACK(RPNFILTER_VALUE_STRING, 3, _VALUE2),
                    _STACK(RPNFILTER_VALUE_STRING, 8, _VALUE3),
                },
        },
        {
            .input = "'" _VALUE0 "' '" _VALUE2 "' '" _VALUE1 "' '" _VALUE0 "'",
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
                    _STACK(RPNFILTER_VALUE_STRING, 1, _VALUE0),
                    _STACK(RPNFILTER_VALUE_STRING, 3, _VALUE2),
                    _STACK(RPNFILTER_VALUE_STRING, 3, _VALUE1),
                    _STACK(RPNFILTER_VALUE_STRING, 1, _VALUE0),
                },
        },
    };

    test.entry = testEntries;
    test.entryCount = ARRAY_SIZE(testEntries);
    *state = &test;

    return 0;
}

void elos_testElosRpnFilterBuilderCompileSuccessString(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "Properly translate tokens into instructions");

    result = elosVerifySteps(&test->data, test->entry, test->entryCount);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterBuilderCompileSuccessStringTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    if (test->data != NULL) {
        elosRpnFilterBuilderDelete(test->data);
    }

    return 0;
}
