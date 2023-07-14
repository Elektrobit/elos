// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/vector.h>

#include "elosRpnFilterBuilderTokenize_utest.h"

#define _testEntry(__i, __c) \
    { .input = (__i), .tokenCount = (__c) }

int elos_testElosRpnFilterBuilderTokenizeErrStringSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    *state = test;
    return 0;
}

void elos_testElosRpnFilterBuilderTokenizeErrString(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterResultE_t result;
    size_t i;

    const elosTokenTestEntry_t testEntry[] = {
        // clang-format off
        _testEntry(".1 cmd 'str'x", 2),
        _testEntry(".1 cmd \"str\"x", 2),
        _testEntry("cmd .x", 1),
        _testEntry(".1ab", 0),
        _testEntry("#'str'c.1", 0),
        _testEntry("#\"str\"c.1", 0),
        _testEntry("~", 0),
        _testEntry("_", 0),
        _testEntry("f.", 0),
        _testEntry("f1", 0),
        _testEntry("f''", 0),
        _testEntry("f\"\"", 0),
        _testEntry("f.", 0),
        _testEntry(".", 0),
        _testEntry("c.", 0),
        _testEntry("1.", 0),
        _testEntry("..", 0),
        _testEntry("'s'.", 0),
        _testEntry("\"s\".", 0),
        _testEntry("'", 0),
        _testEntry("'str", 0),
        _testEntry("\"str", 0),
        _testEntry("str'", 0),
        _testEntry("str\"", 0),
        _testEntry("'str\"", 0),
        _testEntry("\"str'", 0),
        _testEntry("''c", 0),
        _testEntry("\"\"c", 0),
        _testEntry("''1", 0),
        _testEntry("\"\"1", 0),
        _testEntry("''.", 0),
        _testEntry("\"\".", 0),
        _testEntry("'''", 0),
        _testEntry("\"\"\"", 0),
        // clang-format on
    };

    TEST("elosRpnFilterBuilderTokenize");
    SHOULD("%s", "Fail with FILTER_RESULT_ERROR and elements in data->token");

    for (i = 0; i < ARRAY_SIZE(testEntry); i += 1) {
        const elosRpnFilterParam_t param = {.filterString = testEntry[i].input};

        safuLogDebugF("entry[%lu] .input:\"%s\" .tokenCount:%d", i, testEntry[i].input, testEntry[i].tokenCount);

        result = elosRpnFilterBuilderNew(&param, &test->data);
        assert_true(result == RPNFILTER_RESULT_OK);
        assert_non_null(test->data);

        result = elosRpnFilterBuilderTokenize(test->data);
        assert_true(result == RPNFILTER_RESULT_ERROR);
        assert_true(safuVecElements(&test->data->token) == testEntry[i].tokenCount);

        result = elosRpnFilterBuilderDelete(test->data);
        assert_true(result == RPNFILTER_RESULT_OK);
        test->data = NULL;
    }
}

int elos_testElosRpnFilterBuilderTokenizeErrStringTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosRpnFilterBuilderDelete(test->data);
    free(test);
    return 0;
}
