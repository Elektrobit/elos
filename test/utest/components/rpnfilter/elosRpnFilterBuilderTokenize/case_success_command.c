// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderTokenize_utest.h"
#include "safu/common.h"

#define _S1 "X"
#define _S2 "AND"
#define _S3 "STRCMP"

#define _testToken(__s) \
    { .type = RPNFILTER_TOKEN_COMMAND, .length = strlen(__s), .string = (__s) }
#define _testEntry(__i, __s)                                          \
    {                                                                 \
        .input = (__i), .tokenCount = 1, .token = { _testToken(__s) } \
    }

int elos_testElosRpnFilterBuilderTokenizeSuccessCommandSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderTokenizeSuccessCommand(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    int retval;

    const elosTokenTestEntry_t testEntries[] = {
        // clang-format off
        _testEntry(_S1, _S1),
        _testEntry(_S2, _S2),
        _testEntry(_S3, _S3),
        _testEntry(" " _S1, _S1),
        _testEntry("  " _S2, _S2),
        _testEntry("   " _S3, _S3),
        _testEntry(_S1 " ", _S1),
        _testEntry(_S2 "  ", _S2),
        _testEntry(_S3 "   ", _S3),
        _testEntry("   " _S1 "   ", _S1),
        _testEntry("  " _S2 "  ", _S2),
        _testEntry(" "_S3 " ", _S3),
        // clang-format on
    };

    TEST("elosRpnFilterBuilderTokenize");
    SHOULD("%s", "Run successfully for all given filterStrings");

    retval = elosTokenTestEntries(&test->data, testEntries, ARRAY_SIZE(testEntries));
    assert_true(retval == 0);
}

int elos_testElosRpnFilterBuilderTokenizeSuccessCommandTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
