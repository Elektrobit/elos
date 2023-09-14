// SPDX-License-Identifier: MIT

#include "elosRpnFilterExecute_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

#define _VALUE0 1
#define _VALUE1 2

static elosTestExecuteSet_t const elosTestSet[] = {
    {.filter = ".1 .2 ADD .3 EQ",
     .entryCount = 2,
     .entry =
         {
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 3,
                 .param = {ELOS_STACK_I32(1), ELOS_STACK_I32(2), ELOS_STACK_I32(3)},
             },
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 3,
                 .param = {ELOS_STACK_I32(2), ELOS_STACK_I32(1), ELOS_STACK_I32(3)},
             },
         }},
    {.filter = ".1 .2 SUB .3 EQ",
     .entryCount = 2,
     .entry =
         {
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 3,
                 .param = {ELOS_STACK_I32(3), ELOS_STACK_I32(2), ELOS_STACK_I32(1)},
             },
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 3,
                 .param = {ELOS_STACK_I32(2), ELOS_STACK_I32(3), ELOS_STACK_I32(-1)},
             },
         }},
    {.filter = ".1 .2 DIV .3 EQ",
     .entryCount = 1,
     .entry =
         {
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 3,
                 .param = {ELOS_STACK_I32(16), ELOS_STACK_I32(2), ELOS_STACK_I32(8)},
             },
         }},
    {.filter = ".1 .2 MUL .3 EQ",
     .entryCount = 2,
     .entry =
         {
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 3,
                 .param = {ELOS_STACK_I32(8), ELOS_STACK_I32(2), ELOS_STACK_I32(16)},
             },
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 3,
                 .param = {ELOS_STACK_I32(2), ELOS_STACK_I32(8), ELOS_STACK_I32(16)},
             },
         }},
    {.filter = "10 20 ADD 4 SUB 2 ADD 2 DIV 3 MUL .1 EQ",
     .entryCount = 2,
     .entry =
         {
             {
                 .result = RPNFILTER_RESULT_MATCH,
                 .paramCount = 1,
                 .param = {ELOS_STACK_I64(42)},
             },
             {
                 .result = RPNFILTER_RESULT_NO_MATCH,
                 .paramCount = 1,
                 .param = {ELOS_STACK_I64(43)},
             },
         }},
};

static size_t elosTestSetSize = ARRAY_SIZE(elosTestSet);

int elos_testElosRpnFilterExecuteSuccessArithmeticSetup(void **state) {
    static elosStateTestExecute_t test = {0};

    *state = &test;

    return 0;
}

void elos_testElosRpnFilterExecuteSuccessArithmetic(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    ELOS_FILTERSTACK_NEW(input, 2)

    TEST("elosRpnFilterExecute");
    SHOULD("%s", "test if number arithmetic works properly");

    elosTestExecuteSet(test, elosTestSet, elosTestSetSize);
}

int elos_testElosRpnFilterExecuteSuccessArithmeticTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosRpnFilterDeleteMembers(&test->filter);

    return 0;
}
