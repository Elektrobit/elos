// SPDX-License-Identifier: MIT

#include "elosRpnFilterExecute_utest.h"

#define TEST_VALUE "foo bar baz"

typedef struct _testEntry {
    elosRpnFilterResultE_t result;
    char *value;
} _testEntry_t;

int elos_testElosRpnFilterExecuteSuccessStringSetup(void **state) {
    const elosRpnFilterParam_t param = {.filterString = "'" TEST_VALUE "' .1 STRCMP"};
    static elosStateTestExecute_t test = {0};

    elosRpnFilterCreate(&test.filter, &param);
    *state = &test;

    return 0;
}

void elos_testElosRpnFilterExecuteSuccessString(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;
    elosRpnFilterResultE_t result;
    _testEntry_t entry[] = {
        {.result = RPNFILTER_RESULT_MATCH, .value = TEST_VALUE},
        {.result = RPNFILTER_RESULT_NO_MATCH, .value = "foo bar bla"},
        {.result = RPNFILTER_RESULT_NO_MATCH, .value = "foo bar baz "},
        {.result = RPNFILTER_RESULT_NO_MATCH, .value = " foo bar baz"},
    };
    uint32_t i;

    ELOS_FILTERSTACK_NEW(input, 1)

    TEST("elosRpnFilterExecute");
    SHOULD("%s", "test if string comparison works properly");

    for (i = 0; i < ARRAY_SIZE(entry); i += 1) {
        safuLogDebugF("entry[%u]: STRCMP '" TEST_VALUE "' with '%s'", i, entry[i].value);
        ELOS_FILTERSTACK_SET(input, 0, RPNFILTER_VALUE_STRING, strlen(entry[i].value), entry[i].value)

        result = elosRpnFilterExecute(&test->filter, input);
        assert_true(result == entry[i].result);
    }
}

int elos_testElosRpnFilterExecuteSuccessStringTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosRpnFilterDeleteMembers(&test->filter);

    return 0;
}
