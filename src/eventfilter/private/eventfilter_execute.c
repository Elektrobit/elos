// SPDX-License-Identifier: MIT

#include "elos/eventfilter/builder.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/eventfilter/execute.h"
#include "safu/common.h"
#include "safu/log.h"

elosRpnFilterResultE_t _executeEvent(elosRpnFilterExecute_t *ctx) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    elosRpnFilterStackEntry_t const *lastValueEntry = &ctx->filter->values->entry[ctx->valueStackIdx];
    elosRpnFilterStackEntry_t eventEntry = {0};
    elosEvent_t *event = (elosEvent_t *)ctx->input;

    switch (lastValueEntry->data.e) {
        case EVENTFILTER_VALUE_EVENT_DATE:
            safuLogErr("Accessing .event.date directly is not supported yet. Use members like .event.date.tv_sec");
            result = RPNFILTER_RESULT_ERROR;
            break;
        case EVENTFILTER_VALUE_EVENT_DATE_SEC:
            eventEntry.type = RPNFILTER_VALUE_I64;
            eventEntry.bytes = sizeof(int64_t);
            eventEntry.data.i64 = event->date.tv_sec;
            break;
        case EVENTFILTER_VALUE_EVENT_DATE_NSEC:
            eventEntry.type = RPNFILTER_VALUE_I64;
            eventEntry.bytes = sizeof(int64_t);
            eventEntry.data.i64 = event->date.tv_nsec;
            break;
        case EVENTFILTER_VALUE_EVENT_SOURCE:
            safuLogErr("Accessing .event.source directly is not supported. Use members like .event.source.pid");
            result = RPNFILTER_RESULT_ERROR;
            break;
        case EVENTFILTER_VALUE_EVENT_SOURCE_PID:
            eventEntry.type = RPNFILTER_VALUE_I32;
            eventEntry.bytes = sizeof(int32_t);
            eventEntry.data.i32 = event->source.pid;
            break;
        case EVENTFILTER_VALUE_EVENT_SOURCE_APPNAME:
            eventEntry.type = RPNFILTER_VALUE_STRING;
            if (event->source.appName != NULL) {
                eventEntry.bytes = strlen(event->source.appName);
                eventEntry.data.str = event->source.appName;
            }
            break;
        case EVENTFILTER_VALUE_EVENT_SOURCE_FILENAME:
            eventEntry.type = RPNFILTER_VALUE_STRING;
            if (event->source.fileName != NULL) {
                eventEntry.bytes = strlen(event->source.fileName);
                eventEntry.data.str = event->source.fileName;
            }
            break;
        case EVENTFILTER_VALUE_EVENT_SEVERITY:
            eventEntry.type = RPNFILTER_VALUE_I32;
            eventEntry.bytes = sizeof(int32_t);
            eventEntry.data.e = event->severity;
            break;
        case EVENTFILTER_VALUE_EVENT_HARDWAREID:
            eventEntry.type = RPNFILTER_VALUE_STRING;
            eventEntry.bytes = strlen(event->hardwareid);
            eventEntry.data.str = event->hardwareid;
            break;
        case EVENTFILTER_VALUE_EVENT_CLASSIFICATION:
            eventEntry.type = RPNFILTER_VALUE_U64;
            eventEntry.bytes = sizeof(uint64_t);
            eventEntry.data.u64 = event->classification;
            break;
        case EVENTFILTER_VALUE_EVENT_MESSAGECODE:
            eventEntry.type = RPNFILTER_VALUE_I32;
            eventEntry.bytes = sizeof(int32_t);
            eventEntry.data.e = event->messageCode;
            break;
        case EVENTFILTER_VALUE_EVENT_PAYLOAD:
            eventEntry.type = RPNFILTER_VALUE_STRING;
            if (event->payload != NULL) {
                eventEntry.bytes = strlen(event->payload);
                eventEntry.data.str = event->payload;
            }
            break;
        default:
            safuLogErrF("Invalid EVENTFILTER_VALUE number: %d", lastValueEntry->data.i32);
            result = RPNFILTER_RESULT_ERROR;
            break;
    }

    if (result == RPNFILTER_RESULT_OK) {
        elosRpnFilterStackEntry_t *lastStackEntry = &ctx->stack->entry[ctx->stackIdx];
        memcpy(lastStackEntry, &eventEntry, sizeof(elosRpnFilterStackEntry_t));
        ctx->valueStackIdx += 1;
        ctx->stackIdx += 1;
    }

    return result;
}

elosRpnFilterResultE_t elosEventFilterExecuteStep(elosRpnFilterExecute_t *ctx, uint32_t idx) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_NOT_FOUND;
    elosRpnFilterStepType_t const type = ctx->filter->steps->entry[idx].type;

    switch (type) {
        case EVENTFILTER_STEP_LDI_EV:
            result = _executeEvent(ctx);
            break;
        default:
            break;
    }

    return result;
}

elosRpnFilterResultE_t elosEventFilterExecute(elosRpnFilter_t const *filter, elosRpnFilterStack_t const *param,
                                              elosEvent_t *input) {
    elosRpnFilterExecuteFunc_t func[] = {elosEventFilterExecuteStep, elosRpnFilterExecuteStep};
    return elosRpnFilterExecuteWith(filter, param, input, func, ARRAY_SIZE(func));
}
