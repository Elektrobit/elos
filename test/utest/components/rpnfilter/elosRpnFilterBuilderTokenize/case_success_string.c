// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderTokenize_utest.h"
#include "safu/common.h"

#define _S1   ""
#define _S2   "f"
#define _S3   "foo"
#define _S4   "foo \"bar\" bas"
#define _S5   "foo 'bar' bas"
#define _L1S1 "'" _S1 "'"
#define _L1S2 "'" _S2 "'"
#define _L1S3 "'" _S3 "'"
#define _L1S4 "'" _S4 "'"
#define _L2S1 "\"" _S1 "\""
#define _L2S2 "\"" _S2 "\""
#define _L2S3 "\"" _S3 "\""
#define _L2S5 "\"" _S5 "\""

#define _testToken(__s) \
    { .type = RPNFILTER_TOKEN_STRING, .length = strlen(__s), .string = (__s) }
#define _testEntry(__i, __s)                                          \
    {                                                                 \
        .input = (__i), .tokenCount = 1, .token = { _testToken(__s) } \
    }

int elos_testElosRpnFilterBuilderTokenizeSuccessStringSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderTokenizeSuccessString(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    int retval;

    const elosTokenTestEntry_t testEntries[] = {
        // clang-format off
        _testEntry(_L1S1, _S1),
        _testEntry(_L1S2, _S2),
        _testEntry(_L1S3, _S3),
        _testEntry(_L1S4, _S4),
        _testEntry(" " _L1S1, _S1),
        _testEntry("  " _L1S2, _S2),
        _testEntry("   " _L1S3, _S3),
        _testEntry(_L1S1 " ", _S1),
        _testEntry(_L1S2 "  ", _S2),
        _testEntry(_L1S3 "   ", _S3),
        _testEntry("   " _L1S1 "   ", _S1),
        _testEntry("  " _L1S2 "  ", _S2),
        _testEntry(" "_L1S3 " ", _S3),
        _testEntry(_L2S1, _S1),
        _testEntry(_L2S2, _S2),
        _testEntry(_L2S3, _S3),
        _testEntry(_L2S5, _S5),
        // clang-format on
    };

    TEST("elosRpnFilterBuilderTokenize");
    SHOULD("%s", "Run successfully for all given filterStrings");

    retval = elosTokenTestEntries(&test->data, testEntries, ARRAY_SIZE(testEntries));
    assert_true(retval == 0);
}

int elos_testElosRpnFilterBuilderTokenizeSuccessStringTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
