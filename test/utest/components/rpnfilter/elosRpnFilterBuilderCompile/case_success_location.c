// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderCompile_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

#define _VALUE0 1
#define _VALUE1 2
#define _VALUE2 3

int elos_testElosRpnFilterBuilderCompileSuccessLocationSetup(void **state) {
    static elosTestState_t test;

    // These inputs are invalid due to missing values and should fail compiling once the compiler gets
    // more sophisticated in the future; Until then the checks are made during filter execution runtime.
    // For now these tests serve as a very basic set of tests until we improve the compiler.
    static elosStepTestEntry_t const testEntries[] = {
        {
            .input = "." _STR(_VALUE0),
            .stepCount = 1,
            .step = {_STEP(RPNFILTER_STEP_LDP)},
            .stackCount = 1,
            .stack =
                {
                    _STACK_U32(_VALUE0),
                },
        },
        {
            .input = "." _STR(_VALUE0) " ." _STR(_VALUE1),
            .stepCount = 2,
            .step = {_STEP(RPNFILTER_STEP_LDP), _STEP(RPNFILTER_STEP_LDP)},
            .stackCount = 2,
            .stack =
                {
                    _STACK_U32(_VALUE0),
                    _STACK_U32(_VALUE1),
                },
        },
        {
            .input = "." _STR(_VALUE2) " ." _STR(_VALUE0) " ." _STR(_VALUE1),
            .stepCount = 3,
            .step = {_STEP(RPNFILTER_STEP_LDP), _STEP(RPNFILTER_STEP_LDP), _STEP(RPNFILTER_STEP_LDP)},
            .stackCount = 3,
            .stack =
                {
                    _STACK_U32(_VALUE2),
                    _STACK_U32(_VALUE0),
                    _STACK_U32(_VALUE1),
                },
        },
        {
            .input = "." _STR(_VALUE0) " ." _STR(_VALUE0) " ." _STR(_VALUE0) " ." _STR(_VALUE0),
            .stepCount = 4,
            .step =
                {
                    _STEP(RPNFILTER_STEP_LDP),
                    _STEP(RPNFILTER_STEP_LDP),
                    _STEP(RPNFILTER_STEP_LDP),
                    _STEP(RPNFILTER_STEP_LDP),
                },
            .stackCount = 4,
            .stack =
                {
                    _STACK_U32(_VALUE0),
                    _STACK_U32(_VALUE0),
                    _STACK_U32(_VALUE0),
                    _STACK_U32(_VALUE0),
                },
        },
    };

    test.entry = testEntries;
    test.entryCount = ARRAY_SIZE(testEntries);
    *state = &test;

    return 0;
}

void elos_testElosRpnFilterBuilderCompileSuccessLocation(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosRpnFilterBuilderCompile");
    SHOULD("%s", "Properly translate tokens into instructions");

    result = elosVerifySteps(&test->data, test->entry, test->entryCount);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterBuilderCompileSuccessLocationTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    if (test->data != NULL) {
        elosRpnFilterBuilderDelete(test->data);
    }

    return 0;
}
