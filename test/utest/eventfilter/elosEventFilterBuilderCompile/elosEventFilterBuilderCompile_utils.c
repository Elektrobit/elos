// SPDX-License-Identifier: MIT
#include "elosEventFilterBuilderCompile_utest.h"

#define _NAME_ENTRY(__name) [__name] = #__name

const char *elosStepTypeToString(elosRpnFilterStepType_t type) {
    const char *typeStrings[] = {
        _NAME_ENTRY(RPNFILTER_STEP_UNDEFINED), _NAME_ENTRY(RPNFILTER_STEP_EQ),       _NAME_ENTRY(RPNFILTER_STEP_NE),
        _NAME_ENTRY(RPNFILTER_STEP_LE),        _NAME_ENTRY(RPNFILTER_STEP_GE),       _NAME_ENTRY(RPNFILTER_STEP_LT),
        _NAME_ENTRY(RPNFILTER_STEP_GT),        _NAME_ENTRY(RPNFILTER_STEP_AND),      _NAME_ENTRY(RPNFILTER_STEP_OR),
        _NAME_ENTRY(RPNFILTER_STEP_XOR),       _NAME_ENTRY(RPNFILTER_STEP_NOT),      _NAME_ENTRY(RPNFILTER_STEP_ADD),
        _NAME_ENTRY(RPNFILTER_STEP_SUB),       _NAME_ENTRY(RPNFILTER_STEP_DIV),      _NAME_ENTRY(RPNFILTER_STEP_MUL),
        _NAME_ENTRY(RPNFILTER_STEP_STRCMP),    _NAME_ENTRY(RPNFILTER_STEP_REGEX),    _NAME_ENTRY(RPNFILTER_STEP_LDP),
        _NAME_ENTRY(RPNFILTER_STEP_LDV),       _NAME_ENTRY(EVENTFILTER_STEP_LDI_EV),
    };
    const char *result = typeStrings[RPNFILTER_STEP_UNDEFINED];

    if (type > RPNFILTER_STEP_UNDEFINED && type < EVENTFILTER_STEP_TYPE_COUNT) {
        result = typeStrings[type];
    }

    return result;
}

const char *elosStackTypeToString(elosRpnFilterValueType_t type) {
    const char *typeStrings[] = {
        _NAME_ENTRY(RPNFILTER_VALUE_UNDEFINED), _NAME_ENTRY(RPNFILTER_VALUE_BINARY), _NAME_ENTRY(RPNFILTER_VALUE_BOOL),
        _NAME_ENTRY(RPNFILTER_VALUE_I64),       _NAME_ENTRY(RPNFILTER_VALUE_U64),    _NAME_ENTRY(RPNFILTER_VALUE_I32),
        _NAME_ENTRY(RPNFILTER_VALUE_U32),       _NAME_ENTRY(RPNFILTER_VALUE_STRING), _NAME_ENTRY(RPNFILTER_VALUE_REGEX),
    };
    const char *result = typeStrings[RPNFILTER_VALUE_UNDEFINED];

    if (type > RPNFILTER_VALUE_UNDEFINED && type < RPNFILTER_VALUE_TYPE_COUNT) {
        result = typeStrings[type];
    }

    return result;
}

elosRpnFilterResultE_t elosVerifySteps(elosRpnFilterBuilder_t **data, elosStepTestEntry_t const *entry,
                                       size_t entries) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    size_t i;

    for (i = 0; i < entries; i += 1) {
        elosRpnFilterParam_t const param = {.filterString = entry[i].input};
        elosRpnFilterBuilderStepEntry_t *stepEntry;
        elosRpnFilterStackEntry_t *stackEntry;
        safuVec_t *stepVec;
        safuVec_t *stackVec;
        uint32_t o;

        // Setup
        safuLogDebugF("entry[%lu]: Input '%s' should result in %d steps, %d stack.", i, entry[i].input,
                      entry[i].stepCount, entry[i].stackCount);
        result = elosEventFilterBuilderNew(&param, data);
        assert_true(result == RPNFILTER_RESULT_OK);

        // Builder steps
        result = elosEventFilterBuilderTokenize(*data);
        assert_true(result == RPNFILTER_RESULT_OK);
        result = elosEventFilterBuilderCompile(*data);
        assert_true(result == RPNFILTER_RESULT_OK);
        assert_true((*data)->status == RPNFILTER_CREATE_COMPILED);

        // Verify steps
        stepVec = &(*data)->step;
        assert_true(safuVecElements(stepVec) == entry[i].stepCount);

        for (o = 0; o < entry[i].stepCount; o += 1) {
            elosRpnFilterBuilderStepEntry_t const *s = &entry[i].step[o];
            safuLogDebugF("    step[%d] should be: .type=%s", o, elosStepTypeToString(s->type));
            stepEntry = safuVecGet(stepVec, o);
            assert_non_null(stepEntry);
            safuLogDebugF("    step[%d].type: %d == %d", o, stepEntry->type, s->type);
            assert_true(stepEntry->type == s->type);
        }

        // Verify stack
        stackVec = &(*data)->stack;
        assert_true(safuVecElements(stackVec) == entry[i].stackCount);

        for (o = 0; o < entry[i].stackCount; o += 1) {
            elosRpnFilterStackEntry_t const *s = &entry[i].stack[o];
            safuLogDebugF("    stack[%d] should be: .type=%s", o, elosStackTypeToString(s->type));
            stackEntry = safuVecGet(stackVec, o);
            assert_non_null(stackEntry);
            safuLogDebugF("    stack[%d].type: %d == %d", o, stackEntry->type, s->type);
            assert_true(stackEntry->type == s->type);
            safuLogDebugF("    stack[%d].bytes: %lu == %lu", o, stackEntry->bytes, s->bytes);
            assert_true(stackEntry->bytes == s->bytes);

            switch (stackEntry->type) {
                case RPNFILTER_VALUE_BOOL:
                    safuLogDebugF("    stack[%d].data: %d == %d", o, stackEntry->data.b, s->data.b);
                    assert_true(stackEntry->data.b == s->data.b);
                    break;
                case RPNFILTER_VALUE_I64:
                    safuLogDebugF("    stack[%d].data: %ld == %ld", o, stackEntry->data.i64, s->data.i64);
                    assert_true(stackEntry->data.i64 == s->data.i64);
                    break;
                case RPNFILTER_VALUE_U64:
                    safuLogDebugF("    stack[%d].data: %lu == %lu", o, stackEntry->data.u64, s->data.u64);
                    assert_true(stackEntry->data.u64 == s->data.u64);
                    break;
                case RPNFILTER_VALUE_I32:
                    safuLogDebugF("    stack[%d].data: %d == %d", o, stackEntry->data.i32, s->data.i32);
                    assert_true(stackEntry->data.i32 == s->data.i32);
                    break;
                case RPNFILTER_VALUE_U32:
                    safuLogDebugF("    stack[%d].data: %u == %u", o, stackEntry->data.u32, s->data.u32);
                    assert_true(stackEntry->data.u32 == s->data.u32);
                    break;
                default:
                    assert_memory_equal(stackEntry->data.ptr, s->data.ptr, s->bytes);
                    break;
            }
        }

        // Cleanup
        result = elosEventFilterBuilderDelete(*data);
        assert_true(result == RPNFILTER_RESULT_OK);
        *data = NULL;
    }

    return result;
}
