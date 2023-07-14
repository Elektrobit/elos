// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>

#include "elos/rpnfilter/builder.h"

#define _NUMBER_MATCH_CONTINUE(__var)   ((__var) >= '0' && (__var) <= '9')
#define _NUMBER_MATCH_START(__var)      (_NUMBER_MATCH_CONTINUE(__var) || ((__var) == '-') || ((__var) == '+'))
#define _COMMAND_MATCH_START(__var)     (((__var) >= 'a' && (__var) <= 'z') || ((__var) >= 'A' && (__var) <= 'Z'))
#define _COMMAND_MATCH_CONTINUE(__var)  _COMMAND_MATCH_START(__var)
#define _LOCATION_MATCH_START(__var)    ((__var) == '.')
#define _LOCATION_MATCH_CONTINUE(__var) _NUMBER_MATCH_CONTINUE(__var)
#define _SEPARATOR_MATCH(__var)         RPNFILTER_MATCH_SEPARATOR(__var)
#define _TERMINATOR_MATCH(__var)        RPNFILTER_MATCH_TERMINATOR(__var)

#define _STRING_MATCH_START(__var)         (((__var) == '\'') || ((__var) == '"'))
#define _STRING_MATCH_CONTINUE(__var, __m) (((__var) != '\0') && ((__var) != (__m)))
#define _STRING_MATCH_STOP(__var, __m)     ((__var) == (__m))

#define _REGEX_MATCH_START(__var1, __var2) ((((__var1) == 'r') || ((__var1) == 'R')) && _STRING_MATCH_START(__var2))
#define _REGEX_MATCH_CONTINUE(__var, __m)  _STRING_MATCH_CONTINUE(__var, __m)
#define _REGEX_MATCH_STOP(__var, __m)      _STRING_MATCH_STOP(__var, __m)

static elosRpnFilterResultE_t _tokenParseNumber(const char *fStr, size_t *idxStart,
                                                elosRpnFilterBuilderTokenEntry_t *entry) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    int idx = *idxStart;
    char c = fStr[idx];

    if (_NUMBER_MATCH_START(c)) {
        c = fStr[++idx];

        while (_NUMBER_MATCH_CONTINUE(c)) {
            c = fStr[++idx];
        }

        if (_TERMINATOR_MATCH(c)) {
            entry->type = RPNFILTER_TOKEN_NUMBER;
            entry->string = &fStr[*idxStart];
            entry->length = (idx - *idxStart);
            *idxStart = idx;
            result = RPNFILTER_RESULT_OK;
        }
    }

    return result;
}

static elosRpnFilterResultE_t _tokenParseString(const char *fStr, size_t *idxStart,
                                                elosRpnFilterBuilderTokenEntry_t *entry) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    size_t const idxS = *idxStart;
    size_t idx = idxS;
    char c = fStr[idx];

    if (_STRING_MATCH_START(c)) {
        char const m = c;
        c = fStr[++idx];

        while (_STRING_MATCH_CONTINUE(c, m)) {
            c = fStr[++idx];
        }

        if (_STRING_MATCH_STOP(c, m)) {
            c = fStr[idx + 1];
            if (_TERMINATOR_MATCH(c)) {
                entry->type = RPNFILTER_TOKEN_STRING;
                entry->string = &fStr[idxS + 1];
                entry->length = idx - idxS - 1;
                *idxStart = idx + 1;
                result = RPNFILTER_RESULT_OK;
            }
        }
    }

    return result;
}

static elosRpnFilterResultE_t _tokenParseRegex(const char *fStr, size_t *idxStart,
                                               elosRpnFilterBuilderTokenEntry_t *entry) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    size_t const idxS = *idxStart;
    size_t idx = idxS;
    char c = fStr[++idx];

    if (_REGEX_MATCH_START(fStr[idxS], c)) {
        char const m = c;
        c = fStr[++idx];

        while (_REGEX_MATCH_CONTINUE(c, m)) {
            c = fStr[++idx];
        }

        if (_REGEX_MATCH_STOP(c, m)) {
            c = fStr[idx + 1];
            if (_TERMINATOR_MATCH(c)) {
                entry->type = RPNFILTER_TOKEN_REGEX;
                entry->string = &fStr[idxS + 2];
                entry->length = idx - idxS - 2;
                *idxStart = idx + 1;
                result = RPNFILTER_RESULT_OK;
            }
        }
    }

    return result;
}

static elosRpnFilterResultE_t _tokenParseCommand(const char *fStr, size_t *idxStart,
                                                 elosRpnFilterBuilderTokenEntry_t *entry) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    int idx = *idxStart;
    char c = fStr[idx];

    if (_COMMAND_MATCH_START(c)) {
        c = fStr[++idx];

        while (c && _COMMAND_MATCH_CONTINUE(c)) {
            c = fStr[++idx];
        }

        if (_TERMINATOR_MATCH(c)) {
            entry->type = RPNFILTER_TOKEN_COMMAND;
            entry->string = &fStr[*idxStart];
            entry->length = (idx - *idxStart);
            *idxStart = idx;
            result = RPNFILTER_RESULT_OK;
        }
    }

    return result;
}

static elosRpnFilterResultE_t _tokenParseLocation(const char *fStr, size_t *idxStart,
                                                  elosRpnFilterBuilderTokenEntry_t *entry) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    size_t idx = *idxStart;
    char c = fStr[idx];

    if (_LOCATION_MATCH_START(c)) {
        c = fStr[++idx];

        while (c && _LOCATION_MATCH_CONTINUE(c)) {
            c = fStr[++idx];
        }

        if (idx > (*idxStart + 1)) {
            if (_TERMINATOR_MATCH(c)) {
                entry->type = RPNFILTER_TOKEN_LOCATION;
                entry->string = &fStr[*idxStart + 1];
                entry->length = (idx - *idxStart - 1);
                *idxStart = idx;
                result = RPNFILTER_RESULT_OK;
            }
        }
    }

    return result;
}

static elosRpnFilterResultE_t _tokenParseSeparator(const char *fStr, size_t *idxStart,
                                                   UNUSED elosRpnFilterBuilderTokenEntry_t *entry) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    size_t idx = *idxStart;
    char c = fStr[idx];

    if (_SEPARATOR_MATCH(c)) {
        c = fStr[++idx];

        while (c && _SEPARATOR_MATCH(c)) {
            c = fStr[++idx];
        }

        entry->type = RPNFILTER_TOKEN_SEPARATOR;
        result = RPNFILTER_RESULT_OK;
        *idxStart = idx;
    }

    return result;
}

typedef elosRpnFilterResultE_t (*elosTokenParseFunc_t)(const char *fStr, size_t *idxStart,
                                                       elosRpnFilterBuilderTokenEntry_t *entry);

elosRpnFilterResultE_t elosRpnFilterBuilderTokenizeParse(size_t *idx, elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((idx == NULL) || (data == NULL) || (data->param.filterString == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (*idx > strlen(data->param.filterString)) {
        safuLogErr("Invalid index");
    } else if (data->status != RPNFILTER_CREATE_INITIALIZED) {
        safuLogErr("The given data struct is not in state 'FILTER_CREATE_INITIALIZED'");
    } else {
        char const *ptr = data->param.filterString;
        elosTokenParseFunc_t func[] = {_tokenParseNumber,  _tokenParseString,   _tokenParseRegex,
                                       _tokenParseCommand, _tokenParseLocation, _tokenParseSeparator};

        for (size_t i = 0; i < ARRAY_SIZE(func); i += 1) {
            elosRpnFilterBuilderTokenEntry_t tokenEntry = {0};
            result = func[i](ptr, idx, &tokenEntry);
            if (result == RPNFILTER_RESULT_ERROR) {
                break;
            } else if (result == RPNFILTER_RESULT_OK) {
                if (tokenEntry.type != RPNFILTER_TOKEN_SEPARATOR) {
                    int retval;
                    retval = safuVecPush(&data->token, &tokenEntry);
                    if (retval < 0) {
                        safuLogErr("safuVecPush failed");
                        result = RPNFILTER_RESULT_ERROR;
                    }
                }
                break;
            }
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderTokenize(elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((data == NULL) || (data->param.filterString == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        char const *str = data->param.filterString;
        size_t len = strlen(str);
        size_t idx = 0;

        while (idx < len) {
            result = elosRpnFilterBuilderTokenizeParse(&idx, data);
            if (result == RPNFILTER_RESULT_NOT_FOUND) {
                safuLogErrF("Invalid filterString '%s'", &str[idx]);
                result = RPNFILTER_RESULT_ERROR;
                break;
            } else if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("elosRpnFilterBuilderTokenizeParse failed");
                break;
            }
        }

        if (result == RPNFILTER_RESULT_OK) {
            data->status = RPNFILTER_CREATE_TOKENIZED;
        }
    }

    return result;
}
