// SPDX-License-Identifier: MIT

#include "elosProcessFilterExecute_utest.h"

#define _ERROR_RESULT_VALUES                 \
    {                                        \
        { .result = RPNFILTER_RESULT_ERROR } \
    }

int elosTestEloProcessFilterExecuteErrElementSetup(void **state) {
    static elosStateTestExecute_t test = {0};

    *state = &test;

    return 0;
}

void elosTestEloProcessFilterExecuteErrElement(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosTestExecuteSet_t const testSet[] = {
        {
            .filter = ".process.blergh '123' STRCMP",
            .createResult = RPNFILTER_RESULT_ERROR,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
        {
            .filter = ".process.blergh 123 EQ",
            .createResult = RPNFILTER_RESULT_ERROR,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
        {
            .filter = ".process.uid 'test' STRCMP",
            .createResult = RPNFILTER_RESULT_OK,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
        {
            .filter = ".process.exec 123 EQ",
            .createResult = RPNFILTER_RESULT_OK,
            .entryCount = 1,
            .entry = _ERROR_RESULT_VALUES,
        },
    };

    TEST("elosProcessFilterExecute");
    SHOULD("%s", "test if process access works properly");

    elosTestExecuteSet(test, testSet, ARRAY_SIZE(testSet));
}

int elosTestEloProcessFilterExecuteErrElementTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosProcessFilterDeleteMembers(&test->filter);

    return 0;
}
