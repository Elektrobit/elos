// SPDX-License-Identifier: MIT

#include "elosEventFilterExecute_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

#define _VALUE0 1
#define _VALUE1 2

#define _ERROR_RESULT_VALUES                 \
    {                                        \
        { .result = RPNFILTER_RESULT_ERROR } \
    }

int elosTestEloEventFilterExecuteErrElementSetup(void **state) {
    static elosStateTestExecute_t test = {0};

    *state = &test;

    return 0;
}

void elosTestEloEventFilterExecuteErrElement(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosTestExecuteSet_t const testSet[] = {
        {
            .filter = ".event.blergh '123' STRCMP",
            .createResult = RPNFILTER_RESULT_ERROR,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
        {
            .filter = ".event.blergh 123 EQ",
            .createResult = RPNFILTER_RESULT_ERROR,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
        {
            .filter = ".event.date 123 EQ",
            .createResult = RPNFILTER_RESULT_ERROR,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
        {
            .filter = ".event.source 123 EQ",
            .createResult = RPNFILTER_RESULT_ERROR,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
    };

    TEST("elosEventFilterExecute");
    SHOULD("%s", "test if event access works properly");

    elosTestExecuteSet(test, testSet, ARRAY_SIZE(testSet));
}

int elosTestEloEventFilterExecuteErrElementTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosEventFilterDeleteMembers(&test->filter);

    return 0;
}
