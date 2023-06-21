// SPDX-License-Identifier: MIT
#include <safu/vector.h>

#include "elosRpnFilterBuilderTokenize_utest.h"

#define _NAME_ENTRY(__name) [__name] = #__name

const char *elosTokenTypeToString(elosRpnFilterBuilderTokenType_t type) {
    const char *typeStrings[] = {
        _NAME_ENTRY(RPNFILTER_TOKEN_UNDEFINED), _NAME_ENTRY(RPNFILTER_TOKEN_LOCATION),
        _NAME_ENTRY(RPNFILTER_TOKEN_STRING),    _NAME_ENTRY(RPNFILTER_TOKEN_REGEX),
        _NAME_ENTRY(RPNFILTER_TOKEN_NUMBER),    _NAME_ENTRY(RPNFILTER_TOKEN_COMMAND),
    };
    const char *result = typeStrings[RPNFILTER_TOKEN_UNDEFINED];

    if (type > RPNFILTER_TOKEN_UNDEFINED && type < RPNFILTER_TOKEN_TYPE_COUNT) {
        result = typeStrings[type];
    }

    return result;
}

elosRpnFilterResultE_t elosTokenTestEntries(elosRpnFilterBuilder_t **data, const elosTokenTestEntry_t *entry,
                                            size_t entries) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    size_t i;

    for (i = 0; i < entries; i += 1) {
        const elosRpnFilterParam_t param = {.filterString = entry[i].input};
        elosRpnFilterBuilderTokenEntry_t *tEntry;
        safuVec_t *tVec;
        uint32_t o;

        // Setup
        safuLogDebugF("Input <|%s|> should result in %d tokens.", entry[i].input, entry[i].tokenCount);
        result = elosRpnFilterBuilderNew(&param, data);
        assert_true(result == RPNFILTER_RESULT_OK);
        assert_non_null(data);

        // Builder token
        result = elosRpnFilterBuilderTokenize(*data);
        assert_true(result == RPNFILTER_RESULT_OK);

        // Verify token
        tVec = &(*data)->token;
        assert_true(safuVecElements(tVec) == entry[i].tokenCount);

        for (o = 0; o < entry[i].tokenCount; o += 1) {
            const elosRpnFilterBuilderTokenEntry_t *t = &entry[i].token[o];
            safuLogDebugF("    token[%d] should be: .type=%s, .length=%ld, .str:|%s|", o,
                          elosTokenTypeToString(t->type), t->length, t->string);
            tEntry = safuVecGet(tVec, o);
            assert_non_null(tEntry);
            assert_true(tEntry->type == t->type);
            assert_true(tEntry->length == t->length);
            assert_memory_equal(tEntry->string, t->string, t->length);
        }

        // Cleanup
        result = elosRpnFilterBuilderDelete(*data);
        assert_true(result == RPNFILTER_RESULT_OK);
        *data = NULL;
    }

    return result;
}
