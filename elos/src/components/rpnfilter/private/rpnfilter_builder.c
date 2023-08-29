// SPDX-License-Identifier: MIT

#include <regex.h>
#include <stdlib.h>

#include "elos/rpnfilter/builder.h"
#include "safu/common.h"
#include "safu/log.h"
#include "safu/vector.h"

#define _STACK_SIZE ELOS_RPNFILTER_CREATE_STACK_DEFAULT_SIZE
#define _TOKEN_SIZE ELOS_RPNFILTER_CREATE_TOKEN_DEFAULT_SIZE
#define _STEP_SIZE  ELOS_RPNFILTER_CREATE_STEP_DEFAULT_SIZE

static elosRpnFilterResultE_t _deleteMembers(elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    uint32_t elements;
    uint32_t idx;

    data->status = RPNFILTER_CREATE_UNDEFINED;

    // data->param and data->extension have no fields allocated by elosRpnFilterNew(), so we ignore these here

    elements = safuVecElements(&data->stack);
    for (idx = 0; idx < elements; idx += 1) {
        elosRpnFilterStackEntry_t *entry;

        entry = safuVecGet(&data->stack, idx);
        if (entry == NULL) {
            safuLogErr("safuVecGet failed");
            continue;
        }

        switch (entry->type) {
            case RPNFILTER_VALUE_UNDEFINED:
            case RPNFILTER_VALUE_BINARY:
            case RPNFILTER_VALUE_STRING:
            case RPNFILTER_VALUE_REGEX:
                free(entry->data.ptr);
                break;
            default:
                break;
        }
    }
    safuVecFree(&data->stack);

    safuVecFree(&data->step);
    safuVecFree(&data->token);
    result = RPNFILTER_RESULT_OK;

    return result;
}

static elosRpnFilterResultE_t _init(elosRpnFilterParam_t const *param, elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    int retval;

    if ((data == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
        result = RPNFILTER_RESULT_ERROR;
    } else {
        memset(data, 0, sizeof(elosRpnFilterBuilder_t));

        retval = safuVecCreate(&data->stack, _STACK_SIZE, sizeof(elosRpnFilterStackEntry_t));
        if (retval < 0) {
            safuLogErr("safuVecCreate 'stack' failed");
            result = RPNFILTER_RESULT_ERROR;
        }

        if (result == RPNFILTER_RESULT_OK) {
            retval = safuVecCreate(&data->token, _TOKEN_SIZE, sizeof(elosRpnFilterBuilderTokenEntry_t));
            if (retval < 0) {
                safuLogErr("safuVecCreate 'token' failed");
                result = RPNFILTER_RESULT_ERROR;
            }
        }

        if (result == RPNFILTER_RESULT_OK) {
            retval = safuVecCreate(&data->step, _STEP_SIZE, sizeof(elosRpnFilterStepType_t));
            if (retval < 0) {
                safuLogErr("safuVecCreate 'step' failed");
                result = RPNFILTER_RESULT_ERROR;
            }
        }

        if (result == RPNFILTER_RESULT_ERROR) {
            _deleteMembers(data);
        } else {
            data->status = RPNFILTER_CREATE_INITIALIZED;
            data->param = *param;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderNew(elosRpnFilterParam_t const *param, elosRpnFilterBuilder_t **data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    elosRpnFilterBuilder_t *ptr;

    if ((param == NULL) || (data == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        ptr = safuAllocMem(NULL, sizeof(elosRpnFilterBuilder_t));
        if (ptr == NULL) {
            safuLogErrErrno("safuAllocMem");
        } else {
            result = _init(param, ptr);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("_init failed");
                elosRpnFilterBuilderDelete(ptr);
            } else {
                *data = ptr;
            }
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderDelete(elosRpnFilterBuilder_t *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;

    if (data != NULL) {
        result = _deleteMembers(data);
        if (result == RPNFILTER_RESULT_ERROR) {
            safuLogErr("Freeing member data failed");
        }

        free(data);
    }

    return result;
}
