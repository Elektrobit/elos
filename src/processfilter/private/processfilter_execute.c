// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>

#include "elos/processfilter/builder.h"
#include "elos/processfilter/execute.h"
#include "elos/processfilter/processfilter.h"
#include "elos/processfilter/types.h"

elosRpnFilterResultE_t _executeProcess(elosRpnFilterExecute_t *ctx) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    elosRpnFilterStackEntry_t const *lastValueEntry = &ctx->filter->values->entry[ctx->valueStackIdx];
    elosRpnFilterStackEntry_t processEntry = {0};
    elosProcessIdentity_t *process = (elosProcessIdentity_t *)ctx->input;

    switch (lastValueEntry->data.e) {
        case PROCESSFILTER_VALUE_PROCESS_UID:
            processEntry.type = RPNFILTER_VALUE_U64;
            processEntry.bytes = sizeof(uint64_t);
            processEntry.data.e = process->uid;
            break;
        case PROCESSFILTER_VALUE_PROCESS_GID:
            processEntry.type = RPNFILTER_VALUE_U64;
            processEntry.bytes = sizeof(uint64_t);
            processEntry.data.e = process->gid;
            break;
        case PROCESSFILTER_VALUE_PROCESS_PID:
            processEntry.type = RPNFILTER_VALUE_U64;
            processEntry.bytes = sizeof(uint64_t);
            processEntry.data.e = process->pid;
            break;
        case PROCESSFILTER_VALUE_PROCESS_EXEC:
            processEntry.type = RPNFILTER_VALUE_STRING;
            processEntry.data.str = "";
            if (process->exec != NULL) {
                processEntry.bytes = strlen(process->exec);
                processEntry.data.str = process->exec;
            }
            break;
        default:
            safuLogErrF("Invalid PROCESSFILTER_VALUE number: %d", lastValueEntry->data.i32);
            result = RPNFILTER_RESULT_ERROR;
            break;
    }

    if (result == RPNFILTER_RESULT_OK) {
        elosRpnFilterStackEntry_t *lastStackEntry = &ctx->stack->entry[ctx->stackIdx];
        memcpy(lastStackEntry, &processEntry, sizeof(elosRpnFilterStackEntry_t));
        ctx->valueStackIdx += 1;
        ctx->stackIdx += 1;
    }

    return result;
}

elosRpnFilterResultE_t elosProcessFilterExecuteStep(elosRpnFilterExecute_t *ctx, uint32_t idx) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    elosRpnFilterStepType_t const type = ctx->filter->steps->entry[idx].type;

    switch (type) {
        case PROCESSFILTER_STEP_LDI_EV:
            result = _executeProcess(ctx);
            break;
        default:
            break;
    }

    return result;
}

elosRpnFilterResultE_t elosProcessFilterExecute(elosRpnFilter_t const *filter, elosRpnFilterStack_t const *param,
                                                elosProcessIdentity_t *input) {
    elosRpnFilterExecuteFunc_t func[] = {elosProcessFilterExecuteStep, elosRpnFilterExecuteStep};
    return elosRpnFilterExecuteWith(filter, param, input, func, ARRAY_SIZE(func));
}
