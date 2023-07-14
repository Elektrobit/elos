// SPDX-License-Identifier: MIT

#include <string.h>

#include "elosRpnFilterExecute_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

#define _VALUE0 1
#define _VALUE1 2

static elosTestExecuteSet_t const
    elosTestSet[] =
        {
            {.filter = ".1 .2 AND .3 EQ",
             .entryCount = 3,
             .entry =
                 {
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0xFF), ELOS_STACK_I32(0x0F), ELOS_STACK_I32(0x0F)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0xFF), ELOS_STACK_I32(0x00), ELOS_STACK_I32(0x00)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0x00), ELOS_STACK_I32(0xFF), ELOS_STACK_I32(0x00)},
                     },
                 }},
            {.filter = ".1 .2 OR .3 EQ",
             .entryCount = 3,
             .entry =
                 {
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0xF0), ELOS_STACK_I32(0x0F), ELOS_STACK_I32(0xFF)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0x0F), ELOS_STACK_I32(0xF0), ELOS_STACK_I32(0xFF)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0x00), ELOS_STACK_I32(0x00), ELOS_STACK_I32(0x00)},
                     },
                 }},
            {.filter = ".1 .2 XOR .3 EQ",
             .entryCount = 3,
             .entry =
                 {
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0x7F), ELOS_STACK_I32(0x8F), ELOS_STACK_I32(0xF0)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0x8F), ELOS_STACK_I32(0x7F), ELOS_STACK_I32(0xF0)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 3,
                         .param = {ELOS_STACK_I32(0x00), ELOS_STACK_I32(0x00), ELOS_STACK_I32(0x00)},
                     },
                 }},
            {.filter = _STR(_VALUE0) " .1 EQ " _STR(_VALUE1) " .2 NE AND",
             .entryCount = 4,
             .entry =
                 {
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0), ELOS_STACK_I32(_VALUE1 + 1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0), ELOS_STACK_I32(_VALUE1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0 - 1), ELOS_STACK_I32(_VALUE1 + 1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0 - 1), ELOS_STACK_I32(_VALUE1)},
                     },
                 }},
            {.filter = _STR(_VALUE0) " .1 GE " _STR(_VALUE1) " .2 LE AND",
             .entryCount = 6,
             .entry =
                 {
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0), ELOS_STACK_I32(_VALUE1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0 - 1), ELOS_STACK_I32(_VALUE1 + 1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0 - 1), ELOS_STACK_I32(_VALUE1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0), ELOS_STACK_I32(_VALUE1 + 1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0), ELOS_STACK_I32(_VALUE1 - 1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0 + 1), ELOS_STACK_I32(_VALUE1)},
                     },
                 }},
            {.filter = _STR(_VALUE0) " .1 GT " _STR(_VALUE1) " .2 LT AND",
             .entryCount = 4,
             .entry =
                 {
                     {
                         .result = RPNFILTER_RESULT_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0 - 1), ELOS_STACK_I32(_VALUE1 + 1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0 - 1), ELOS_STACK_I32(_VALUE1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0), ELOS_STACK_I32(_VALUE1 + 1)},
                     },
                     {
                         .result = RPNFILTER_RESULT_NO_MATCH,
                         .paramCount = 2,
                         .param = {ELOS_STACK_I32(_VALUE0), ELOS_STACK_I32(_VALUE1)},
                     },
                 }},
};

static size_t elosTestSetSize = ARRAY_SIZE(elosTestSet);

int elos_testElosRpnFilterExecuteSuccessOperationSetup(void **state) {
    static elosStateTestExecute_t test = {0};

    *state = &test;

    return 0;
}

void elos_testElosRpnFilterExecuteSuccessOperation(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    ELOS_FILTERSTACK_NEW(input, 2)

    TEST("elosRpnFilterExecute");
    SHOULD("%s", "test if number based operations works properly");

    elosTestExecuteSet(test, elosTestSet, elosTestSetSize);
}

int elos_testElosRpnFilterExecuteSuccessOperationTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosRpnFilterDeleteMembers(&test->filter);

    return 0;
}
