// SPDX-License-Identifier: MIT
#include "elosRpnFilterExecute_utest.h"

void _testEntry(elosStateTestExecute_t *test, elosTestExecuteEntry_t const *entry) {
    elosRpnFilterResultE_t result;
    uint32_t idx;

    // We need to use a fixed value here due to ancient C99
    ELOS_FILTERSTACK_NEW(input, ELOS_MAX_ENTRIES)

    for (idx = 0; idx < entry->paramCount; idx += 1) {
        elosRpnFilterStackEntry_t const *stack = &entry->param[idx];
        ELOS_FILTERSTACK_SET(input, idx, stack->type, stack->bytes, stack->data.ptr)
    }

    result = elosRpnFilterExecute(&test->filter, input);
    assert_int_equal(result, entry->result);
}

void _testSetEntry(elosStateTestExecute_t *test, elosTestExecuteSet_t const *set) {
    uint32_t idx;
    for (idx = 0; idx < set->entryCount; idx += 1) {
        safuLogDebugF("    _testEntry[%u]", idx);
        _testEntry(test, &set->entry[idx]);
    }
}

void elosTestExecuteSet(elosStateTestExecute_t *test, elosTestExecuteSet_t const *set, size_t elements) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    uint32_t idx;

    for (idx = 0; idx < elements; idx += 1) {
        elosRpnFilterParam_t param = {.filterString = (char *)set[idx].filter};
        result = elosRpnFilterCreate(&test->filter, &param);
        assert_true(result == RPNFILTER_RESULT_OK);

        safuLogDebugF("_testSetEntry[%u]: %s", idx, param.filterString);
        _testSetEntry(test, &set[idx]);

        result = elosRpnFilterDeleteMembers(&test->filter);
        assert_true(result == RPNFILTER_RESULT_OK);
    }
}
