// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>

#include "elos/eventfilter/builder.h"

elosRpnFilterResultE_t elosEventFilterBuilderTokenizeParse(size_t *startIdx, elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    // There's quite a bit of redundant code in this function,
    // we need to try to share as much with rpnFilter as possible here

    if ((startIdx == NULL) || (data == NULL) || (data->param.filterString == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (*startIdx > strlen(data->param.filterString)) {
        safuLogErr("Invalid index");
    } else if (data->status != RPNFILTER_CREATE_INITIALIZED) {
        safuLogErr("The given data struct is not in state 'FILTER_CREATE_INITIALIZED'");
    } else {
        char const *evLocStart[] = {".event.", ".ev.", ".e."};
        char const *str = data->param.filterString;
        size_t idx = *startIdx;

        result = RPNFILTER_RESULT_NOT_FOUND;

        for (size_t i = 0; i < ARRAY_SIZE(evLocStart); i += 1) {
            size_t len = strlen(evLocStart[i]);
            int retVal;

            retVal = strncmp(evLocStart[i], &str[idx], len);
            if (retVal == 0) {
                idx += len;

                char c = str[idx];
                while (!RPNFILTER_MATCH_TERMINATOR(c)) {
                    c = str[++idx];
                }

                elosRpnFilterBuilderTokenEntry_t entry = {
                    .length = idx - len - *startIdx,
                    .string = &str[*startIdx + len],
                    .type = EVENTFILTER_TOKEN_EVENT,
                };
                retVal = safuVecPush(&data->token, &entry);
                if (retVal < 0) {
                    safuLogErr("safuVecPush failed");
                    result = RPNFILTER_RESULT_ERROR;
                } else {
                    result = RPNFILTER_RESULT_OK;
                    *startIdx = idx;
                }

                break;
            }
        }
    }

    return result;
}

elosRpnFilterResultE_t elosEventFilterBuilderTokenize(elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if (data == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        char const *str = data->param.filterString;
        size_t const len = strlen(str);
        size_t idx = 0;
        elosRpnFilterBuilderTokenizeParseFunc_t parseFunc[] = {
            elosEventFilterBuilderTokenizeParse,
            elosRpnFilterBuilderTokenizeParse,
        };

        while (idx < len) {
            for (size_t i = 0; i < ARRAY_SIZE(parseFunc); i += 1) {
                result = parseFunc[i](&idx, data);
                if (result != RPNFILTER_RESULT_NOT_FOUND) {
                    break;
                }
            }

            switch (result) {
                case RPNFILTER_RESULT_OK:
                    break;
                case RPNFILTER_RESULT_NOT_FOUND:
                    safuLogErrF("Invalid filterString '%s'", &str[idx]);
                default:
                    result = RPNFILTER_RESULT_ERROR;
                    idx = len;
                    break;
            }
        }

        if (result == RPNFILTER_RESULT_OK) {
            data->status = RPNFILTER_CREATE_TOKENIZED;
        }
    }

    return result;
}
