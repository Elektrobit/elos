// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderTokenize_utest.h"
#include "safu/common.h"

#define _tTok(__t, __s) \
    { .type = (__t), .length = strlen(__s), .string = (__s) }

#define _tTokS(__s)     _tTok(RPNFILTER_TOKEN_STRING, (__s))
#define _tTokC(__s)     _tTok(RPNFILTER_TOKEN_COMMAND, (__s))
#define _tTokL(__s)     _tTok(RPNFILTER_TOKEN_LOCATION, (__s))
#define _tTokN(__s)     _tTok(RPNFILTER_TOKEN_NUMBER, (__s))
#define _tEnt(__i, __c) .input = (__i), .tokenCount = (__c)

// Not a complete list, but should be more than sufficient to weed out relevant errors
#define _S1 "42  .1 EQ"
#define _S2 ".1 'foobar'   STRCMP"
#define _S3 " 4 40 ADD 2 SUB "
#define _S4 ".1 .2 EQ"
#define _S5 "  'foo' 'bar' STRCMP"
#define _S6 "1       2 3  4 SUM"
#define _S7 "'foobar' 'oba'  REGEX "

int elos_testElosRpnFilterBuilderTokenizeSuccessSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderTokenizeSuccess(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    int retval;

    const elosTokenTestEntry_t testEntries[] = {
        // clang-format off
        {_tEnt(_S1, 3), .token = {_tTokN("42"), _tTokL("1"), _tTokC("EQ")}},
        {_tEnt(_S2, 3), .token = {_tTokL("1"), _tTokS("foobar"), _tTokC("STRCMP")}},
        {_tEnt(_S3, 5), .token = {_tTokN("4"), _tTokN("40"), _tTokC("ADD"), _tTokN("2"), _tTokC("SUB")}},
        {_tEnt(_S4, 3), .token = {_tTokL("1"), _tTokL("2"), _tTokC("EQ")}},
        {_tEnt(_S5, 3), .token = {_tTokS("foo"), _tTokS("bar"), _tTokC("STRCMP")}},
        {_tEnt(_S6, 5), .token = {_tTokN("1"), _tTokN("2"), _tTokN("3"), _tTokN("4"), _tTokC("SUM")}},
        {_tEnt(_S7, 3), .token = {_tTokS("foobar"), _tTokS("oba"), _tTokC("REGEX")}},
        // clang-format on
    };

    TEST("elosRpnFilterBuilderTokenize");
    SHOULD("%s", "Run successfully for all given filterStrings");

    retval = elosTokenTestEntries(&test->data, testEntries, ARRAY_SIZE(testEntries));
    assert_true(retval == 0);
}

int elos_testElosRpnFilterBuilderTokenizeSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
