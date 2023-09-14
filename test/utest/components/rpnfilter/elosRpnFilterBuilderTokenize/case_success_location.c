// SPDX-License-Identifier: MIT

#include "elosRpnFilterBuilderTokenize_utest.h"
#include "safu/common.h"

#define _S1  "1"
#define _S2  "234"
#define _S3  "456"
#define _LS1 "." _S1
#define _LS2 "." _S2
#define _LS3 "." _S3

#define _testToken(__s) \
    { .type = RPNFILTER_TOKEN_LOCATION, .length = strlen(__s), .string = (__s) }
#define _testEntry(__i, __s)                                          \
    {                                                                 \
        .input = (__i), .tokenCount = 1, .token = { _testToken(__s) } \
    }

int elos_testElosRpnFilterBuilderTokenizeSuccessLocationSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderTokenizeSuccessLocation(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    int retval;

    const elosTokenTestEntry_t testEntries[] = {
        // clang-format off
        _testEntry(_LS1, _S1),
        _testEntry(_LS2, _S2),
        _testEntry(_LS3, _S3),
        _testEntry(" " _LS1, _S1),
        _testEntry("  " _LS2, _S2),
        _testEntry("   " _LS3, _S3),
        _testEntry(_LS1 " ", _S1),
        _testEntry(_LS2 "  ", _S2),
        _testEntry(_LS3 "   ", _S3),
        _testEntry("   " _LS1 "   ", _S1),
        _testEntry("  " _LS2 "  ", _S2),
        _testEntry(" "_LS3 " ", _S3),
        // clang-format on
    };

    TEST("elosRpnFilterBuilderTokenize");
    SHOULD("%s", "Run successfully for all given filterStrings");

    retval = elosTokenTestEntries(&test->data, testEntries, ARRAY_SIZE(testEntries));
    assert_true(retval == 0);
}

int elos_testElosRpnFilterBuilderTokenizeSuccessLocationTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
