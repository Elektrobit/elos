// SPDX-License-Identifier: MIT

#include <regex.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/rpnfilter/builder.h"

// Helper types, variables and macros for creating steps

typedef struct _commandListEntry {
    const char *name;
    size_t consumes;
    size_t appends;
} _commandListEntry_t;

#define _ENTRY(__t, __n, __c, __a) [__t] = {.name = (__n), .consumes = (__c), .appends = (__a)}

static const _commandListEntry_t elosCommandList[] = {
    _ENTRY(RPNFILTER_STEP_EQ, "EQ", 2, 1),         _ENTRY(RPNFILTER_STEP_NE, "NE", 2, 1),
    _ENTRY(RPNFILTER_STEP_LE, "LE", 2, 1),         _ENTRY(RPNFILTER_STEP_GE, "GE", 2, 1),
    _ENTRY(RPNFILTER_STEP_LT, "LT", 2, 1),         _ENTRY(RPNFILTER_STEP_GT, "GT", 2, 1),
    _ENTRY(RPNFILTER_STEP_AND, "AND", 2, 1),       _ENTRY(RPNFILTER_STEP_OR, "OR", 2, 1),
    _ENTRY(RPNFILTER_STEP_XOR, "XOR", 2, 1),       _ENTRY(RPNFILTER_STEP_NOT, "NOT", 1, 1),
    _ENTRY(RPNFILTER_STEP_ADD, "ADD", 2, 1),       _ENTRY(RPNFILTER_STEP_SUB, "SUB", 2, 1),
    _ENTRY(RPNFILTER_STEP_DIV, "DIV", 2, 1),       _ENTRY(RPNFILTER_STEP_MUL, "MUL", 2, 1),
    _ENTRY(RPNFILTER_STEP_STRCMP, "STRCMP", 2, 1), _ENTRY(RPNFILTER_STEP_REGEX, "REGEX", 2, 1),
    _ENTRY(RPNFILTER_STEP_LDV, "LDV", 1, 1),       _ENTRY(RPNFILTER_STEP_LDP, "LDP", 1, 1),
};

// Helper types, variables, macros and functions to process tokens

#define _CMD_MAX_LEN 64

static elosRpnFilterResultE_t _processUndefined(elosRpnFilterBuilderTokenEntry_t const *token,
                                                elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    elosRpnFilterBuilderStepEntry_t step = {.type = RPNFILTER_STEP_UNDEFINED, .token = token};
    int retval;

    retval = safuVecPush(&data->step, &step);
    if (retval < 0) {
        safuLogErr("safuVecPush failed");
        result = RPNFILTER_RESULT_ERROR;
    }

    return result;
}

static elosRpnFilterResultE_t _processLocation(elosRpnFilterBuilderTokenEntry_t const *token,
                                               elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    long int location;

    errno = 0;
    location = strtol(token->string, NULL, 10);
    if (errno != 0) {
        safuLogErrErrno("strtol failed");
    } else if ((location < 0) || (location > UINT32_MAX)) {
        safuLogErr("Location must be an UINT32 value");
    } else {
        elosRpnFilterBuilderStepEntry_t step = {.type = RPNFILTER_STEP_LDP, .token = token};
        int retval;

        retval = safuVecPush(&data->step, &step);
        if (retval < 0) {
            safuLogErr("safuVecPush failed");
        } else {
            elosRpnFilterStackEntry_t entry = {
                .type = RPNFILTER_VALUE_U32,
                .bytes = sizeof(uint32_t),
                .data.u32 = (uint32_t)location,
            };

            result = elosRpnFilterBuilderStackAppend(&entry, data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("elosRpnFilterBuilderStackAppend failed");
            }
        }
    }

    return result;
}

static elosRpnFilterResultE_t _processString(elosRpnFilterBuilderTokenEntry_t const *token,
                                             elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    elosRpnFilterBuilderStepEntry_t step = {.type = RPNFILTER_STEP_LDV, .token = token};
    int retval;

    retval = safuVecPush(&data->step, &step);
    if (retval < 0) {
        safuLogErr("safuVecPush failed");
    } else {
        elosRpnFilterStackEntry_t const entry = {
            .type = RPNFILTER_VALUE_STRING,
            .bytes = token->length,
            .data.str = (char *)token->string,  // String is not modified, so the typecast is safe
        };

        result = elosRpnFilterBuilderStackAppend(&entry, data);
        if (result == RPNFILTER_RESULT_ERROR) {
            safuLogErr("_stackPush failed");
        }
    }

    return result;
}

static elosRpnFilterResultE_t _processRegex(elosRpnFilterBuilderTokenEntry_t const *token,
                                            elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    elosRpnFilterBuilderStepEntry_t step = {.type = RPNFILTER_STEP_LDV, .token = token};
    int retval;
    regex_t regex;
    char errbuf[200];

    retval = safuVecPush(&data->step, &step);
    if (retval < 0) {
        safuLogErr("safuVecPush failed");
    } else {
        char *pattern = strndup((char *)token->string, token->length);
        if (pattern != NULL) {
            retval = regcomp(&regex, pattern, REG_EXTENDED);
            if (retval == 0) {
                elosRpnFilterStackEntry_t const entry = {
                    .type = RPNFILTER_VALUE_REGEX,
                    .bytes = sizeof(regex_t),
                    .data.ptr = &regex,
                };

                result = elosRpnFilterBuilderStackAppend(&entry, data);
                if (result == RPNFILTER_RESULT_ERROR) {
                    safuLogErr("_stackPush failed");
                }
            } else {
                regerror(retval, &regex, errbuf, sizeof(errbuf));
                safuLogErrF("Processing regex failed: %s", errbuf);
                result = RPNFILTER_RESULT_ERROR;
            }

            free(pattern);
        } else {
            safuLogErr("Copying of pattern string failed.");
            result = RPNFILTER_RESULT_ERROR;
        }
    }

    return result;
}

static elosRpnFilterResultE_t _processNumber(elosRpnFilterBuilderTokenEntry_t const *token,
                                             elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    long long number;  // Note: We should add a compile time check that long long == int64_t for platform independency

    errno = 0;
    number = strtoll(token->string, NULL, 10);
    if (errno != 0) {
        safuLogErrErrno("strtol failed");
    } else {
        elosRpnFilterBuilderStepEntry_t step = {.type = RPNFILTER_STEP_LDV, .token = token};
        int retval;

        retval = safuVecPush(&data->step, &step);
        if (retval < 0) {
            safuLogErr("safuVecPush failed");
        } else {
            elosRpnFilterStackEntry_t entry = {
                .type = RPNFILTER_VALUE_I64,
                .bytes = sizeof(int64_t),
                .data.i64 = (int64_t)number,
            };

            result = elosRpnFilterBuilderStackAppend(&entry, data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("_stackPush failed");
            }
        }
    }

    return result;
}

static elosRpnFilterResultE_t _processCommand(elosRpnFilterBuilderTokenEntry_t const *token,
                                              elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    elosRpnFilterStepType_t idx = RPNFILTER_STEP_UNDEFINED + 1;
    elosRpnFilterBuilderStepEntry_t step = {.type = RPNFILTER_STEP_UNDEFINED, .token = token};
    int retval;

    if (token->length > _CMD_MAX_LEN) {
        safuLogErr("Token too long for command");
    } else {
        char cmd[_CMD_MAX_LEN + 1] = {0};

        strncpy(cmd, token->string, token->length);

        if (token->length > 0) {
            while (idx < RPNFILTER_STEP_TYPE_COUNT) {
                _commandListEntry_t const *command = &elosCommandList[idx];

                retval = strcmp(cmd, command->name);
                if (retval == 0) {
                    step.type = idx;
                    step.consumes = command->consumes;
                    step.appends = command->appends;
                    break;
                }

                idx += 1;
            }
        }

        result = elosRpnFilterBuilderStepAppend(&step, data);
        if (result != RPNFILTER_RESULT_OK) {
            safuLogErr("Appending step failed");
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderStackAppend(elosRpnFilterStackEntry_t const *entry,
                                                       elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    elosRpnFilterStackEntry_t newEntry;
    int retval;

    memcpy(&newEntry, entry, sizeof(elosRpnFilterStackEntry_t));

    switch (entry->type) {
        case RPNFILTER_VALUE_UNDEFINED:
        case RPNFILTER_VALUE_BINARY:
        case RPNFILTER_VALUE_STRING:
        case RPNFILTER_VALUE_REGEX:
            if (newEntry.bytes > 0) {
                void *ptr;
                ptr = safuAllocMem(NULL, entry->bytes);
                if (ptr == NULL) {
                    safuLogErr("safuAllocMem failed");
                } else {
                    memcpy(ptr, entry->data.ptr, entry->bytes);
                    newEntry.data.ptr = ptr;
                }
            }
            break;
        default:
            break;
    }

    retval = safuVecPush(&data->stack, &newEntry);
    if (retval < 0) {
        safuLogErr("safuVecPush failed");
        result = RPNFILTER_RESULT_ERROR;
        free(newEntry.data.ptr);
    } else {
        data->stackSizeTracker += 1;
        result = RPNFILTER_RESULT_OK;
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderStepAppend(elosRpnFilterBuilderStepEntry_t const *step,
                                                      elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    int64_t stackSize = data->stackSizeTracker;
    int retVal;

    stackSize -= step->consumes;
    if (stackSize < 0) {
        safuLogErrF("Too few values on stack for step '%.*s' (consumes:%u, on stack:%ld)", (int)step->token->length,
                    step->token->string, step->consumes, data->stackSizeTracker);
    } else {
        retVal = safuVecPush(&data->step, step);
        if (retVal < 0) {
            safuLogErr("safuVecPush failed");
        } else {
            stackSize += step->appends;
            data->stackSizeTracker = stackSize;
            result = RPNFILTER_RESULT_OK;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderCompileToken(elosRpnFilterBuilderTokenEntry_t const *token,
                                                        elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((token == NULL) || (data == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosRpnFilterBuilderTokenType_t const type = token->type;
        elosRpnFilterBuilderCompileFunc_t const funcMap[] = {
            [RPNFILTER_TOKEN_UNDEFINED] = _processUndefined, [RPNFILTER_TOKEN_LOCATION] = _processLocation,
            [RPNFILTER_TOKEN_STRING] = _processString,       [RPNFILTER_TOKEN_NUMBER] = _processNumber,
            [RPNFILTER_TOKEN_COMMAND] = _processCommand,     [RPNFILTER_TOKEN_REGEX] = _processRegex,
        };

        if ((type <= (int)ARRAY_SIZE(funcMap)) && (funcMap[type] != NULL)) {
            result = funcMap[type](token, data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErrF("Compile token failed (type:'%d', str:'%*.s')", type, (int)token->length, token->string);
            }
        } else {
            result = RPNFILTER_RESULT_NOT_FOUND;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderCompileWith(elosRpnFilterBuilder_t *data,
                                                       elosRpnFilterBuilderCompileFunc_t func[], size_t funcCount) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((data == NULL) || (func == NULL) || (funcCount == 0)) {
        safuLogErr("Invalid parameter");
    } else if (data->status != RPNFILTER_CREATE_TOKENIZED) {
        safuLogErr("The given data struct is not in state 'RPNFILTER_CREATE_TOKENIZED'");
    } else {
        uint32_t elements = safuVecElements(&data->token);
        uint32_t i;

        for (i = 0; i < elements; i += 1) {
            elosRpnFilterBuilderTokenEntry_t *token;
            uint32_t o;

            token = safuVecGet(&data->token, i);
            if (token == NULL) {
                safuLogErrF("token[%d]: safuVecGet failed", i);
                break;
            }

            for (o = 0; o < funcCount; o += 1) {
                result = func[o](token, data);
                if (result != RPNFILTER_RESULT_NOT_FOUND) {
                    break;
                }
            }

            if (result == RPNFILTER_RESULT_NOT_FOUND) {
                safuLogErrF("Invalid token (type:'%d', str:'%.*s')", token->type, (int)token->length, token->string);
                result = RPNFILTER_RESULT_ERROR;
                break;
            } else if (result == RPNFILTER_RESULT_ERROR) {
                break;
            }
        }
    }

    if (result != RPNFILTER_RESULT_ERROR) {
        data->status = RPNFILTER_CREATE_COMPILED;
        result = RPNFILTER_RESULT_OK;
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderCompile(elosRpnFilterBuilder_t *data) {
    elosRpnFilterBuilderCompileFunc_t func[] = {elosRpnFilterBuilderCompileToken};
    return elosRpnFilterBuilderCompileWith(data, func, ARRAY_SIZE(func));
}
