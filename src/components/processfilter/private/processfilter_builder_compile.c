// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/processfilter/builder.h"
#include "elos/processfilter/processfilter.h"

typedef struct _memberMap {
    char const *name;
    elosRpnFilterValueType_t type;
} _memberMap_t;

static elosRpnFilterResultE_t _searchMemberMap(_memberMap_t const *memberMap, size_t memberMapSize,
                                               elosRpnFilterBuilderTokenEntry_t const *token, size_t *idx) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;

    for (size_t i = 0; i < memberMapSize; i += 1) {
        size_t const len = strlen(memberMap[i].name);
        int retVal;

        if (len == token->length) {
            retVal = strncmp(memberMap[i].name, token->string, len);
            if (retVal == 0) {
                result = RPNFILTER_RESULT_OK;
                *idx = i;
                break;
            }
        }
    }

    return result;
}

static elosRpnFilterResultE_t _compileProcessLocation(elosRpnFilterBuilderTokenEntry_t const *token,
                                                      elosRpnFilterBuilder_t *data) {
    static _memberMap_t const memberMap[] = {
        {.name = "uid", .type = PROCESSFILTER_VALUE_PROCESS_UID},
        {.name = "gid", .type = PROCESSFILTER_VALUE_PROCESS_GID},
        {.name = "pid", .type = PROCESSFILTER_VALUE_PROCESS_PID},
        {.name = "exec", .type = PROCESSFILTER_VALUE_PROCESS_EXEC},
    };
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    size_t idx = 0;

    result = _searchMemberMap(memberMap, ARRAY_SIZE(memberMap), token, &idx);
    if (result == RPNFILTER_RESULT_OK) {
        elosRpnFilterBuilderStepEntry_t const step = {
            .type = PROCESSFILTER_STEP_LDI_EV,
            .token = token,
        };
        int retVal;

        retVal = safuVecPush(&data->step, &step);
        if (retVal < 0) {
            safuLogErr("safuVecPush failed");
            result = RPNFILTER_RESULT_ERROR;
        } else {
            elosRpnFilterStackEntry_t const entry = {
                .type = RPNFILTER_VALUE_U32,
                .bytes = sizeof(uint32_t),
                .data.u32 = memberMap[idx].type,
            };

            result = elosRpnFilterBuilderStackAppend(&entry, data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("elosRpnFilterBuilderStackAppend failed");
            } else {
                result = RPNFILTER_RESULT_OK;
            }
        }
    }

    return result;
}

elosRpnFilterResultE_t elosProcessFilterBuilderCompileToken(elosRpnFilterBuilderTokenEntry_t const *token,
                                                            elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((token == NULL) || (data == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosRpnFilterBuilderTokenType_t const type = token->type;

        if (type == PROCESSFILTER_TOKEN_PROCESS) {
            result = _compileProcessLocation(token, data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErrF("Token compile failed (type:%d, str:%.*s)", type, (int)token->length, token->string);
            }
        } else {
            result = RPNFILTER_RESULT_NOT_FOUND;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosProcessFilterBuilderCompile(elosRpnFilterBuilder_t *data) {
    elosRpnFilterBuilderCompileFunc_t func[] = {elosProcessFilterBuilderCompileToken, elosRpnFilterBuilderCompileToken};
    return elosRpnFilterBuilderCompileWith(data, func, ARRAY_SIZE(func));
}
