// SPDX-License-Identifier: MIT

#include "elosEventFilterBuilderCompile_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

int elosTestEloEventFilterBuilderCompileSuccessLocationSetup(void **state) {
    static elosTestState_t test;

    // These inputs are invalid due to missing values and should fail compiling once the compiler gets
    // more sophisticated in the future; Until then the checks are made during filter execution runtime.
    // For now these tests serve as a very basic set of tests until we improve the compiler.
    static elosStepTestEntry_t const testEntries[] = {
        {
            .input = ".event.severity",
            .stepCount = 1,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV)},
            .stackCount = 1,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_SEVERITY)},
        },
        {
            .input = ".ev.severity",
            .stepCount = 1,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV)},
            .stackCount = 1,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_SEVERITY)},
        },
        {
            .input = ".e.severity",
            .stepCount = 1,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV)},
            .stackCount = 1,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_SEVERITY)},
        },
        {
            .input = ".ev.severity 1 GE",
            .stepCount = 3,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_GE)},
            .stackCount = 2,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_SEVERITY), _STACK_I64(1)},
        },
        {
            .input = ".event.classification 1 EQ",
            .stepCount = 3,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_EQ)},
            .stackCount = 2,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_CLASSIFICATION), _STACK_I64(1)},
        },
        {
            .input = ".event.date.tv_sec 1 EQ",
            .stepCount = 3,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_EQ)},
            .stackCount = 2,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_DATE_SEC), _STACK_I64(1)},
        },
        {
            .input = ".event.date.tv_nsec 1 EQ",
            .stepCount = 3,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_EQ)},
            .stackCount = 2,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_DATE_NSEC), _STACK_I64(1)},
        },
        {
            .input = ".event.date.sec 1 EQ",
            .stepCount = 3,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_EQ)},
            .stackCount = 2,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_DATE_SEC), _STACK_I64(1)},
        },
        {
            .input = ".event.date.nsec 1 EQ",
            .stepCount = 3,
            .step = {_STEP(EVENTFILTER_STEP_LDI_EV), _STEP(RPNFILTER_STEP_LDV), _STEP(RPNFILTER_STEP_EQ)},
            .stackCount = 2,
            .stack = {_STACK_U32(EVENTFILTER_VALUE_EVENT_DATE_NSEC), _STACK_I64(1)},
        },
    };

    test.entry = testEntries;
    test.entryCount = ARRAY_SIZE(testEntries);
    *state = &test;

    return 0;
}

void elosTestEloEventFilterBuilderCompileSuccessLocation(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterResultE_t result;

    TEST("elosEventFilterBuilderCompile");
    SHOULD("%s", "Properly translate tokens into instructions");

    result = elosVerifySteps(&test->data, test->entry, test->entryCount);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elosTestEloEventFilterBuilderCompileSuccessLocationTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    if (test->data != NULL) {
        elosEventFilterBuilderDelete(test->data);
    }

    return 0;
}
