// SPDX-License-Identifier: MIT

#include <regex.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>

#include "elos/rpnfilter/execute.h"
#include "elos/rpnfilter/types.h"

#define _STACK_SIZE       32
#define _STACK_ENTRY(__i) ctx->stack->entry[__i]
#define _STACK_LAST       _STACK_ENTRY(ctx->stackIdx)
#define _VSTACK_LAST      ctx->filter->values->entry[ctx->valueStackIdx]
#define _STACK_SUB1       _STACK_ENTRY(ctx->stackIdx - 1)
#define _STACK_SUB2       _STACK_ENTRY(ctx->stackIdx - 2)

static inline void _pushBool(elosRpnFilterExecute_t *ctx, bool value) {
    _STACK_LAST.type = RPNFILTER_VALUE_BOOL;
    _STACK_LAST.bytes = sizeof(bool);
    _STACK_LAST.data.b = value ? true : false;  // bool isn't just one bit, so we make sure its really only true/false
    ctx->stackIdx += 1;
}

static inline void _pushI64(elosRpnFilterExecute_t *ctx, int64_t value) {
    _STACK_LAST.type = RPNFILTER_VALUE_I64;
    _STACK_LAST.bytes = sizeof(int64_t);
    _STACK_LAST.data.i64 = value;
    ctx->stackIdx += 1;
}

static inline void _pushU64(elosRpnFilterExecute_t *ctx, uint64_t value) {
    _STACK_LAST.type = RPNFILTER_VALUE_U64;
    _STACK_LAST.bytes = sizeof(uint64_t);
    _STACK_LAST.data.u64 = value;
    ctx->stackIdx += 1;
}

static inline int64_t _entryToI64(elosRpnFilterStackEntry_t const *restrict entry) {
    int64_t x = 0;

    switch (entry->type) {
        case RPNFILTER_VALUE_BOOL:
            x = entry->data.b;
            break;
        case RPNFILTER_VALUE_I32:
            x = entry->data.i32;
            break;
        case RPNFILTER_VALUE_I64:
            x = entry->data.i64;
            break;
        case RPNFILTER_VALUE_U32:
            x = entry->data.u32;
            break;
        case RPNFILTER_VALUE_U64:
            x = entry->data.u64;
            break;
        default:
            // This part will never be reached due to _verifyNumberTypeTuple always being called first
            safuLogErr("Incompatible value type");
            break;
    }

    return x;
}

static inline uint64_t _entryToU64(elosRpnFilterStackEntry_t const *restrict entry) {
    uint64_t x = 0;

    switch (entry->type) {
        case RPNFILTER_VALUE_BOOL:
            x = entry->data.b;
            break;
        case RPNFILTER_VALUE_I32:
            x = entry->data.i32;
            break;
        case RPNFILTER_VALUE_I64:
            x = entry->data.i64;
            break;
        case RPNFILTER_VALUE_U32:
            x = entry->data.u32;
            break;
        case RPNFILTER_VALUE_U64:
            x = entry->data.u64;
            break;
        default:
            // This part will never be reached due to _verifyNumberTypeTuple always being called first
            safuLogErr("Incompatible value type");
            break;
    }

    return x;
}

static inline elosRpnFilterResultE_t _verifyNumberType(elosRpnFilterExecute_t const *restrict ctx) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    elosRpnFilterValueTypeE_t const val = _STACK_ENTRY(ctx->stackIdx - 1).type;

    switch (val) {
        case RPNFILTER_VALUE_BOOL:
        case RPNFILTER_VALUE_I32:
        case RPNFILTER_VALUE_I64:
        case RPNFILTER_VALUE_U32:
        case RPNFILTER_VALUE_U64:
            break;
        default:
            result = RPNFILTER_RESULT_ERROR_NUMOP_TYPE;
            safuLogErr("Step RPNFILTER_STEP_NOT used with non-numeric type!");
            break;
    }

    return result;
}

static inline elosRpnFilterResultE_t _verifyNumberTypeTuple(elosRpnFilterExecute_t const *restrict ctx,
                                                            elosRpnFilterValueTypeE_t *restrict mode) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    elosRpnFilterValueTypeE_t const a = _STACK_ENTRY(ctx->stackIdx - 2).type;
    elosRpnFilterValueTypeE_t const b = _STACK_ENTRY(ctx->stackIdx - 1).type;

    // Note: We do not want the hassle with types in the RpnFilter syntax right now,
    //       so we use either int64_t or uint64_t for operations until we have requirements
    //       that warrant a more strict test or different behaviour. The used type is based on the first parameter
    //       expect when its a bool, in which case the second parameter decides it.
    // Note2: This likely can be further optimized by using a data table (= avoiding conditional jumps)
    switch (a) {
        case RPNFILTER_VALUE_BOOL:
            switch (b) {
                case RPNFILTER_VALUE_U32:
                case RPNFILTER_VALUE_U64:
                    *mode = RPNFILTER_VALUE_U64;
                    break;
                case RPNFILTER_VALUE_BOOL:
                case RPNFILTER_VALUE_I32:
                case RPNFILTER_VALUE_I64:
                    *mode = RPNFILTER_VALUE_I64;
                    break;
                default:
                    result = RPNFILTER_RESULT_ERROR_NUMOP_TYPE;
                    safuLogErr("Incompatible comparison types");
                    break;
            }
            break;
        case RPNFILTER_VALUE_I32:
        case RPNFILTER_VALUE_I64:
            switch (b) {
                case RPNFILTER_VALUE_BOOL:
                case RPNFILTER_VALUE_U32:
                case RPNFILTER_VALUE_U64:
                case RPNFILTER_VALUE_I32:
                case RPNFILTER_VALUE_I64:
                    *mode = RPNFILTER_VALUE_I64;
                    break;
                default:
                    result = RPNFILTER_RESULT_ERROR_NUMOP_TYPE;
                    safuLogErr("Incompatible comparison types");
                    break;
            }
            break;
        case RPNFILTER_VALUE_U32:
        case RPNFILTER_VALUE_U64:
            switch (b) {
                case RPNFILTER_VALUE_BOOL:
                case RPNFILTER_VALUE_I32:
                case RPNFILTER_VALUE_I64:
                case RPNFILTER_VALUE_U32:
                case RPNFILTER_VALUE_U64:
                    *mode = RPNFILTER_VALUE_U64;
                    break;
                default:
                    result = RPNFILTER_RESULT_ERROR_NUMOP_TYPE;
                    safuLogErr("Incompatible comparison types");
                    break;
            }
            break;
        default:
            result = RPNFILTER_RESULT_ERROR_NUMOP_TYPE;
            safuLogErr("Incompatible comparison types");
            break;
    }

    return result;
}

static inline elosRpnFilterResultE_t _stepUnaryNumberOperation(elosRpnFilterExecute_t *restrict ctx,
                                                               elosRpnFilterStepType_t const type) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    int64_t const val = _entryToI64(&_STACK_ENTRY(ctx->stackIdx - 1));
    int64_t x = 0;

    switch (type) {
        case RPNFILTER_STEP_NOT:
            x = !val;
            break;
        default:
            safuLogErr("Unknown comparison type");
            result = RPNFILTER_RESULT_ERROR;
            break;
    }

    if (result == RPNFILTER_RESULT_OK) {
        ctx->stackIdx -= 1;
        switch (type) {
            case RPNFILTER_STEP_NOT:
                _pushBool(ctx, x);

                break;
            default:
                _pushI64(ctx, x);
                break;
        }

        if (x) {
            result = RPNFILTER_RESULT_MATCH;
        } else {
            result = RPNFILTER_RESULT_NO_MATCH;
        }
    }

    return result;
}

static inline elosRpnFilterResultE_t _stepNumberOperationSigned(elosRpnFilterExecute_t *restrict ctx,
                                                                elosRpnFilterStepType_t const type) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    int64_t const a = _entryToI64(&_STACK_ENTRY(ctx->stackIdx - 2));
    int64_t const b = _entryToI64(&_STACK_ENTRY(ctx->stackIdx - 1));
    int64_t x = 0;

    switch (type) {
        case RPNFILTER_STEP_AND:
            x = a & b;
            break;
        case RPNFILTER_STEP_OR:
            x = a | b;
            break;
        case RPNFILTER_STEP_XOR:
            x = a ^ b;
            break;
        case RPNFILTER_STEP_ADD:
            x = a + b;
            break;
        case RPNFILTER_STEP_SUB:
            x = a - b;
            break;
        case RPNFILTER_STEP_DIV:
            x = a / b;
            break;
        case RPNFILTER_STEP_MUL:
            x = a * b;
            break;
        case RPNFILTER_STEP_EQ:
            x = (a == b);
            break;
        case RPNFILTER_STEP_NE:
            x = (a != b);
            break;
        case RPNFILTER_STEP_LE:
            x = (a <= b);
            break;
        case RPNFILTER_STEP_GE:
            x = (a >= b);
            break;
        case RPNFILTER_STEP_LT:
            x = (a < b);
            break;
        case RPNFILTER_STEP_GT:
            x = (a > b);
            break;
        default:
            safuLogErr("Unknown comparison type");
            result = RPNFILTER_RESULT_ERROR;
            break;
    }

    if (result == RPNFILTER_RESULT_OK) {
        ctx->stackIdx -= 2;
        switch (type) {
            case RPNFILTER_STEP_EQ:
            case RPNFILTER_STEP_NE:
            case RPNFILTER_STEP_LE:
            case RPNFILTER_STEP_GE:
            case RPNFILTER_STEP_LT:
            case RPNFILTER_STEP_GT:
                _pushBool(ctx, x);
                break;
            default:
                _pushI64(ctx, x);
                break;
        }

        if (x) {
            result = RPNFILTER_RESULT_MATCH;
        } else {
            result = RPNFILTER_RESULT_NO_MATCH;
        }
    }

    return result;
}

static inline elosRpnFilterResultE_t _stepNumberOperationUnsigned(elosRpnFilterExecute_t *restrict ctx,
                                                                  elosRpnFilterStepType_t const type) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    uint64_t const a = _entryToU64(&_STACK_ENTRY(ctx->stackIdx - 2));
    uint64_t const b = _entryToU64(&_STACK_ENTRY(ctx->stackIdx - 1));
    uint64_t x = 0;

    switch (type) {
        case RPNFILTER_STEP_AND:
            x = a & b;
            break;
        case RPNFILTER_STEP_OR:
            x = a | b;
            break;
        case RPNFILTER_STEP_XOR:
            x = a ^ b;
            break;
        case RPNFILTER_STEP_ADD:
            x = a + b;
            break;
        case RPNFILTER_STEP_SUB:
            x = a - b;
            break;
        case RPNFILTER_STEP_DIV:
            x = a / b;
            break;
        case RPNFILTER_STEP_MUL:
            x = a * b;
            break;
        case RPNFILTER_STEP_EQ:
            x = (a == b);
            break;
        case RPNFILTER_STEP_NE:
            x = (a != b);
            break;
        case RPNFILTER_STEP_LE:
            x = (a <= b);
            break;
        case RPNFILTER_STEP_GE:
            x = (a >= b);
            break;
        case RPNFILTER_STEP_LT:
            x = (a < b);
            break;
        case RPNFILTER_STEP_GT:
            x = (a > b);
            break;
        default:
            safuLogErr("Unknown comparison type");
            result = RPNFILTER_RESULT_ERROR;
            break;
    }

    if (result == RPNFILTER_RESULT_OK) {
        ctx->stackIdx -= 2;
        switch (type) {
            case RPNFILTER_STEP_EQ:
            case RPNFILTER_STEP_NE:
            case RPNFILTER_STEP_LE:
            case RPNFILTER_STEP_GE:
            case RPNFILTER_STEP_LT:
            case RPNFILTER_STEP_GT:
                _pushBool(ctx, x);
                break;
            default:
                _pushU64(ctx, x);
                break;
        }

        if (x) {
            result = RPNFILTER_RESULT_MATCH;
        } else {
            result = RPNFILTER_RESULT_NO_MATCH;
        }
    }

    return result;
}

static elosRpnFilterResultE_t _stepNumberOperation(elosRpnFilterExecute_t *ctx, elosRpnFilterStepType_t type) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NO_MATCH;
    elosRpnFilterValueTypeE_t mode = RPNFILTER_VALUE_UNDEFINED;

    result = _verifyNumberTypeTuple(ctx, &mode);
    if (result == RPNFILTER_RESULT_OK) {
        switch (mode) {
            case RPNFILTER_VALUE_I64:
                result = _stepNumberOperationSigned(ctx, type);
                break;
            case RPNFILTER_VALUE_U64:
                result = _stepNumberOperationUnsigned(ctx, type);
                break;
            default:
                result = RPNFILTER_RESULT_ERROR;
                break;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterExecuteStackNew(elosRpnFilterStack_t **stack, size_t stackSize) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((stack == NULL) || (stackSize < 1)) {
        safuLogErr("Invalid parameter");
    } else {
        size_t const stackMemLen = sizeof(elosRpnFilterStack_t) + ((sizeof(elosRpnFilterStackEntry_t) * stackSize));
        void *ptr;

        // We should find a way to avoid this allocMem, as this function will get called VERY often
        ptr = safuAllocMem(NULL, stackMemLen);
        if (ptr == NULL) {
            safuLogErr("safuAllocMem failed");
        } else {
            memset(ptr, 0, stackMemLen);
            *stack = ptr;
            result = RPNFILTER_RESULT_OK;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterExecuteStackDelete(elosRpnFilterStack_t *stack) {
    free(stack);
    return RPNFILTER_RESULT_OK;
}

elosRpnFilterResultE_t elosRpnFilterExecuteStep(elosRpnFilterExecute_t *ctx, uint32_t idx) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    elosRpnFilterStepType_t const type = ctx->filter->steps->entry[idx].type;
    uint32_t ui32;

    switch (type) {
        case RPNFILTER_STEP_LDP:
            if (ctx->param == NULL) {
                safuLogErr("Step RPNFILTER_STEP_LDP used without parameters!");
                result = RPNFILTER_RESULT_ERROR;
            } else {
                ui32 = _VSTACK_LAST.data.u32 - 1;
                memcpy(&_STACK_LAST, &ctx->param->entry[ui32], sizeof(elosRpnFilterStackEntry_t));
                ctx->valueStackIdx += 1;
                ctx->stackIdx += 1;
            }
            break;
        case RPNFILTER_STEP_LDV:
            memcpy(&_STACK_LAST, &_VSTACK_LAST, sizeof(elosRpnFilterStackEntry_t));
            ctx->valueStackIdx += 1;
            ctx->stackIdx += 1;
            break;
        case RPNFILTER_STEP_EQ:
        case RPNFILTER_STEP_NE:
        case RPNFILTER_STEP_LE:
        case RPNFILTER_STEP_GE:
        case RPNFILTER_STEP_LT:
        case RPNFILTER_STEP_GT:
        case RPNFILTER_STEP_AND:
        case RPNFILTER_STEP_OR:
        case RPNFILTER_STEP_XOR:
        case RPNFILTER_STEP_ADD:
        case RPNFILTER_STEP_SUB:
        case RPNFILTER_STEP_DIV:
        case RPNFILTER_STEP_MUL:
            result = _stepNumberOperation(ctx, type);
            break;
        case RPNFILTER_STEP_NOT:
            result = _verifyNumberType(ctx);
            if (result == RPNFILTER_RESULT_OK) {
                result = _stepUnaryNumberOperation(ctx, type);
            }
            break;
        case RPNFILTER_STEP_STRCMP:
            if ((_STACK_SUB1.type != RPNFILTER_VALUE_STRING) || (_STACK_SUB2.type != RPNFILTER_VALUE_STRING)) {
                safuLogErr("Step RPNFILTER_STEP_STRCMP used with non-string values!");
                result = RPNFILTER_RESULT_ERROR;
            } else {
                result = RPNFILTER_RESULT_NO_MATCH;
                if (_STACK_SUB1.bytes == _STACK_SUB2.bytes) {
                    if (strncmp(_STACK_SUB1.data.str, _STACK_SUB2.data.str, _STACK_SUB1.bytes) == 0) {
                        result = RPNFILTER_RESULT_MATCH;
                    }
                }
            }
            ctx->stackIdx -= 2;
            _pushBool(ctx, result == RPNFILTER_RESULT_MATCH ? 1 : 0); /* 2nd param expects true (1) or false (0) */
            break;
        case RPNFILTER_STEP_REGEX: {
            int res;
            regex_t *regex;
            char errbuf[200];

            if ((_STACK_SUB1.type != RPNFILTER_VALUE_REGEX) || (_STACK_SUB2.type != RPNFILTER_VALUE_STRING)) {
                safuLogErr("Step RPNFILTER_STEP_REGEX expects a regex string as first and a string as second value!");
                result = RPNFILTER_RESULT_ERROR;
            } else {
                result = RPNFILTER_RESULT_NO_MATCH;
                regex = _STACK_SUB1.data.ptr;

                if (_STACK_SUB1.bytes > 0 && _STACK_SUB2.bytes > 0) {
                    res = regexec(regex, _STACK_SUB2.data.str, 0, NULL, 0);
                    if (res == 0) {
                        result = RPNFILTER_RESULT_MATCH;
                    } else if (res == REG_NOMATCH) {
                        result = RPNFILTER_RESULT_NO_MATCH;
                    } else {
                        regerror(res, regex, errbuf, sizeof(errbuf));
                        safuLogErrF("Step RPNFILTER_STEP_REGEX failed to match regex: %s", errbuf);
                        result = RPNFILTER_RESULT_ERROR;
                    }
                }
            }

            ctx->stackIdx -= 2;
            _pushBool(ctx, result == RPNFILTER_RESULT_MATCH ? 1 : 0); /* 2nd param expects true (1) or false (0) */
            break;
        }
        default:
            result = RPNFILTER_RESULT_NOT_FOUND;
            break;
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterExecuteWith(elosRpnFilter_t const *filter, elosRpnFilterStack_t const *param,
                                                void *input, elosRpnFilterExecuteFunc_t func[], size_t funcCount) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((filter == NULL) || (func == NULL) || (funcCount == 0)) {
        safuLogErr("Invalid parameter");
    } else if ((filter->steps == NULL) || (filter->values == NULL)) {
        safuLogErr("Invalid filter");
    } else {
        elosRpnFilterExecute_t ctx = {.filter = filter, .param = param, .input = input};

        result = elosRpnFilterExecuteStackNew(&ctx.stack, _STACK_SIZE);
        if (result == RPNFILTER_RESULT_ERROR) {
            safuLogErr("elosRpnFilterExecuteStackNew failed");
        } else {
            for (uint32_t stepIdx = 0; stepIdx < filter->steps->count; stepIdx += 1) {
                for (size_t funcIdx = 0; funcIdx < funcCount; funcIdx += 1) {
                    result = func[funcIdx](&ctx, stepIdx);
                    if (result == RPNFILTER_RESULT_ERROR) {
                        safuLogErrF("executeStep (stepIdx:%u, funcIdx:%lu) failed", stepIdx, funcIdx);
                    } else if (result == RPNFILTER_RESULT_NOT_FOUND) {
                        continue;
                    }
                    break;
                }

                switch (result) {
                    case RPNFILTER_RESULT_ERROR:
                        stepIdx = filter->steps->count;
                        break;
                    case RPNFILTER_RESULT_NOT_FOUND:
                        safuLogErr("Invalid step");
                        result = RPNFILTER_RESULT_ERROR;
                        break;
                    case RPNFILTER_RESULT_OK:
                    case RPNFILTER_RESULT_MATCH:
                    case RPNFILTER_RESULT_NO_MATCH:
                        break;
                    default:
                        safuLogErrF("Invalid step response '%d'", result);
                        result = RPNFILTER_RESULT_ERROR;
                        break;
                }
            }

            elosRpnFilterExecuteStackDelete(ctx.stack);
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterExecute(elosRpnFilter_t const *filter, elosRpnFilterStack_t const *param) {
    elosRpnFilterExecuteFunc_t func[] = {elosRpnFilterExecuteStep};
    return elosRpnFilterExecuteWith(filter, param, NULL, func, ARRAY_SIZE(func));
}

elosRpnFilterResultE_t elosRpnFilterExecuteResult(UNUSED const elosRpnFilter_t *filter,
                                                  UNUSED const elosRpnFilterStack_t *param,
                                                  UNUSED elosRpnFilterStack_t *output) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;

    safuLogWarnF("%s not implemented yet!", __func__);

    return result;
}
