// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/eventfilter/builder.h"
#include "elos/eventfilter/eventfilter.h"

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

static elosRpnFilterResultE_t _compileEventLocation(elosRpnFilterBuilderTokenEntry_t const *token,
                                                    elosRpnFilterBuilder_t *data) {
    _memberMap_t const memberMap[] = {
        {.name = "date.tv_sec", .type = EVENTFILTER_VALUE_EVENT_DATE_SEC},
        {.name = "date.sec", .type = EVENTFILTER_VALUE_EVENT_DATE_SEC},
        {.name = "date.tv_nsec", .type = EVENTFILTER_VALUE_EVENT_DATE_NSEC},
        {.name = "date.nsec", .type = EVENTFILTER_VALUE_EVENT_DATE_NSEC},
        {.name = "source.pid", .type = EVENTFILTER_VALUE_EVENT_SOURCE_PID},
        {.name = "source.appName", .type = EVENTFILTER_VALUE_EVENT_SOURCE_APPNAME},
        {.name = "source.fileName", .type = EVENTFILTER_VALUE_EVENT_SOURCE_FILENAME},
        {.name = "severity", .type = EVENTFILTER_VALUE_EVENT_SEVERITY},
        {.name = "hardwareid", .type = EVENTFILTER_VALUE_EVENT_HARDWAREID},
        {.name = "classification", .type = EVENTFILTER_VALUE_EVENT_CLASSIFICATION},
        {.name = "messageCode", .type = EVENTFILTER_VALUE_EVENT_MESSAGECODE},
        {.name = "payload", .type = EVENTFILTER_VALUE_EVENT_PAYLOAD},
    };
    elosRpnFilterResultE_t result;
    size_t idx = 0;

    result = _searchMemberMap(memberMap, ARRAY_SIZE(memberMap), token, &idx);
    if (result == RPNFILTER_RESULT_OK) {
        elosRpnFilterBuilderStepEntry_t const step = {
            .type = EVENTFILTER_STEP_LDI_EV,
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

    if (result == RPNFILTER_RESULT_NOT_FOUND) {
        elosRpnFilterResultE_t resVal;
        _memberMap_t const unsupportedMemberMap[] = {
            {.name = "source", .type = EVENTFILTER_VALUE_EVENT_SOURCE},
            {.name = "date", .type = EVENTFILTER_VALUE_EVENT_DATE},
        };

        // Try to give a more meaningful error message with certain elements
        resVal = _searchMemberMap(unsupportedMemberMap, ARRAY_SIZE(unsupportedMemberMap), token, &idx);
        if (resVal == RPNFILTER_RESULT_OK) {
            char const *errStr = "Direct access of struct .event.%.*s is not supported, use the structs members.";
            safuLogErrF(errStr, (int)token->length, token->string);
        } else {
            safuLogErrF("Invalid member .event.%.*s", (int)token->length, token->string);
        }

        result = RPNFILTER_RESULT_ERROR;
    }

    return result;
}

elosRpnFilterResultE_t elosEventFilterBuilderCompileToken(elosRpnFilterBuilderTokenEntry_t const *token,
                                                          elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((token == NULL) || (data == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosRpnFilterBuilderTokenType_t const type = token->type;

        if (type == EVENTFILTER_TOKEN_EVENT) {
            result = _compileEventLocation(token, data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErrF("Token compile failed (type:%d, str:%.*s)", type, (int)token->length, token->string);
            }
        } else {
            result = RPNFILTER_RESULT_NOT_FOUND;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosEventFilterBuilderCompile(elosRpnFilterBuilder_t *data) {
    elosRpnFilterBuilderCompileFunc_t func[] = {elosEventFilterBuilderCompileToken, elosRpnFilterBuilderCompileToken};
    return elosRpnFilterBuilderCompileWith(data, func, ARRAY_SIZE(func));
}
