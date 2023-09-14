// SPDX-License-Identifier: MIT

#include <elos/common/process.h>

#include "elosProcessFilterExecute_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

#define _VALUE0 1
#define _VALUE1 2

int elosTestEloProcessFilterExecuteSuccessElementSetup(void **state) {
    static elosStateTestExecute_t test = {0};

    *state = &test;

    return 0;
}

void elosTestEloProcessFilterExecuteSuccessElement(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;
    elosProcessIdentity_t process = {.uid = 123, .gid = 456, .pid = 789, .exec = "/bin/crinit"};

    elosTestExecuteSet_t const testSet[] = {
        {
            .filter = ".process.uid .1 EQ",
            .createResult = RPNFILTER_RESULT_OK,
            .entryCount = 2,
            .entry =
                {
                    {
                        .result = RPNFILTER_RESULT_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_I32(process.uid)},
                    },
                    {
                        .result = RPNFILTER_RESULT_NO_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_I32(0)},
                    },
                },
        },
        {
            .filter = ".process.gid .1 EQ",
            .createResult = RPNFILTER_RESULT_OK,
            .entryCount = 2,
            .entry =
                {
                    {
                        .result = RPNFILTER_RESULT_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_I32(process.gid)},
                    },
                    {
                        .result = RPNFILTER_RESULT_NO_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_I32(0)},
                    },
                },
        },
        {
            .filter = ".process.pid .1 EQ",
            .createResult = RPNFILTER_RESULT_OK,
            .entryCount = 2,
            .entry =
                {
                    {
                        .result = RPNFILTER_RESULT_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_I32(process.pid)},
                    },
                    {
                        .result = RPNFILTER_RESULT_NO_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_I32(0)},
                    },
                },
        },
        {
            .filter = ".process.exec .1 STRCMP",
            .createResult = RPNFILTER_RESULT_OK,
            .entryCount = 2,
            .entry =
                {
                    {
                        .result = RPNFILTER_RESULT_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_STRING(process.exec)},
                    },
                    {
                        .result = RPNFILTER_RESULT_NO_MATCH,
                        .process = process,
                        .paramCount = 1,
                        .param = {ELOS_STACK_STRING("this will not match")},
                    },
                },
        },
    };

    TEST("elosProcessFilterExecute");
    SHOULD("%s", "test if process access works properly");

    elosTestExecuteSet(test, testSet, ARRAY_SIZE(testSet));
}

int elosTestEloProcessFilterExecuteSuccessElementTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosProcessFilterDeleteMembers(&test->filter);

    return 0;
}
