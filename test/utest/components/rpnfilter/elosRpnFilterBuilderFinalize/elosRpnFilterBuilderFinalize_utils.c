// SPDX-License-Identifier: MIT
#include "elosRpnFilterBuilderFinalize_utest.h"

#define _NAME_ENTRY(__name) [__name] = #__name

const char *elosStepTypeToString(elosRpnFilterStepType_t type) {
    const char *typeStrings[] = {
        _NAME_ENTRY(RPNFILTER_STEP_UNDEFINED), _NAME_ENTRY(RPNFILTER_STEP_EQ),    _NAME_ENTRY(RPNFILTER_STEP_NE),
        _NAME_ENTRY(RPNFILTER_STEP_LE),        _NAME_ENTRY(RPNFILTER_STEP_GE),    _NAME_ENTRY(RPNFILTER_STEP_LT),
        _NAME_ENTRY(RPNFILTER_STEP_GT),        _NAME_ENTRY(RPNFILTER_STEP_AND),   _NAME_ENTRY(RPNFILTER_STEP_OR),
        _NAME_ENTRY(RPNFILTER_STEP_XOR),       _NAME_ENTRY(RPNFILTER_STEP_NOT),   _NAME_ENTRY(RPNFILTER_STEP_ADD),
        _NAME_ENTRY(RPNFILTER_STEP_SUB),       _NAME_ENTRY(RPNFILTER_STEP_DIV),   _NAME_ENTRY(RPNFILTER_STEP_MUL),
        _NAME_ENTRY(RPNFILTER_STEP_STRCMP),    _NAME_ENTRY(RPNFILTER_STEP_REGEX), _NAME_ENTRY(RPNFILTER_STEP_LDP),
        _NAME_ENTRY(RPNFILTER_STEP_LDV),
    };
    const char *result = typeStrings[RPNFILTER_STEP_UNDEFINED];

    if (type > RPNFILTER_STEP_UNDEFINED && type < RPNFILTER_STEP_TYPE_COUNT) {
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

elosRpnFilterResultE_t elosVerifyFinalize(elosTestFinalize_t *test) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    // elosStepTestEntry_t const *entry = test->entry;
    size_t entries = test->entryCount;

    size_t i;

    for (i = 0; i < entries; i += 1) {
        elosRpnFilterParam_t const param = {.filterString = test->entry[i].input};
        uint32_t o;

        // Setup
        safuLogDebugF("entry[%lu]: Input '%s' should result in %d steps, %d stack.", i, test->entry[i].input,
                      test->entry[i].stepCount, test->entry[i].stackCount);
        result = elosRpnFilterBuilderNew(&param, &test->data);
        assert_true(result == RPNFILTER_RESULT_OK);
        result = elosRpnFilterNew(&test->filter);
        assert_true(result == RPNFILTER_RESULT_OK);

        // Builder steps
        result = elosRpnFilterBuilderTokenize(test->data);
        assert_true(result == RPNFILTER_RESULT_OK);
        result = elosRpnFilterBuilderCompile(test->data);
        assert_true(result == RPNFILTER_RESULT_OK);
        assert_true(test->data->status == RPNFILTER_CREATE_COMPILED);
        result = elosRpnFilterBuilderFinalize(test->data, test->filter);
        assert_true(result == RPNFILTER_RESULT_OK);
        assert_true(test->filter->state == RPNFILTER_FINALIZED);

        // Verify steps
        assert_true(test->filter->steps->count == test->entry[i].stepCount);

        for (o = 0; o < test->entry[i].stepCount; o += 1) {
            elosRpnFilterBuilderStepEntry_t const *s = &test->entry[i].step[o];
            safuLogDebugF("    token[%d] should be: .type=%s", o, elosStepTypeToString(s->type));
            assert_true(test->filter->steps->entry[o].type == s->type);
        }

        // Verify stack
        assert_true(test->filter->values->count == test->entry[i].stackCount);

        for (o = 0; o < test->entry[i].stackCount; o += 1) {
            elosRpnFilterStackEntry_t const *f = &test->filter->values->entry[o];
            elosRpnFilterStackEntry_t const *s = &test->entry[i].stack[o];
            safuLogDebugF("    stack[%d] should be: .type=%s", o, elosStackTypeToString(s->type));
            assert_true(f->type == s->type);
            assert_true(f->bytes == s->bytes);

            switch (f->type) {
                case RPNFILTER_VALUE_BOOL:
                    assert_int_equal(f->data.b, s->data.b);
                    break;
                case RPNFILTER_VALUE_I64:
                    assert_int_equal(f->data.i64, s->data.i64);
                    break;
                case RPNFILTER_VALUE_U64:
                    assert_int_equal(f->data.u64, s->data.u64);
                    break;
                case RPNFILTER_VALUE_I32:
                    assert_int_equal(f->data.i32, s->data.i32);
                    break;
                case RPNFILTER_VALUE_U32:
                    assert_int_equal(f->data.u32, s->data.u32);
                    break;
                default:
                    assert_memory_equal(f->data.ptr, s->data.ptr, s->bytes);
                    break;
            }
        }

        // Cleanup
        result = elosRpnFilterBuilderDelete(test->data);
        assert_true(result == RPNFILTER_RESULT_OK);
        test->data = NULL;
        result = elosRpnFilterDelete(test->filter);
        assert_true(result == RPNFILTER_RESULT_OK);
        test->filter = NULL;
    }

    return result;
}
