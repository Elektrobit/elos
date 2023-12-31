// SPDX-License-Identifier: MIT

#include "mock_eventprocessor.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventProcessorInitialize, safuResultE_t, elosEventProcessor_t *eventProcessor,
               elosEventProcessorParam_t const *param) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorInitialize)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorInitialize)(eventProcessor, param);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected_ptr(param);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorInitialize)(eventProcessor, param);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorDeleteMembers, safuResultE_t, elosEventProcessor_t *eventProcessor) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorDeleteMembers)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorDeleteMembers)(eventProcessor);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorDeleteMembers)(eventProcessor);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorFilterNodeCreateWithQueue, safuResultE_t, elosEventProcessor_t *eventProcessor,
               char const **filterStrings, size_t filterStringCount, elosEventQueueId_t *eventQueueId,
               elosEventFilterNodeId_t *eventFilterNodeId) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorFilterNodeCreateWithQueue)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorFilterNodeCreateWithQueue)(
                eventProcessor, filterStrings, filterStringCount, eventQueueId, eventFilterNodeId);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected_ptr(filterStrings);
            check_expected(filterStringCount);
            check_expected_ptr(eventQueueId);
            check_expected_ptr(eventFilterNodeId);
            if (eventQueueId != NULL) {
                *eventQueueId = mock_type(elosEventQueueId_t);
            }
            if (eventFilterNodeId != NULL) {
                *eventFilterNodeId = mock_type(elosEventFilterNodeId_t);
            }
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorFilterNodeCreateWithQueue)(
                eventProcessor, filterStrings, filterStringCount, eventQueueId, eventFilterNodeId);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorFilterNodeCreate, safuResultE_t, elosEventProcessor_t *eventProcessor,
               char const **filterStrings, size_t filterStringCount, elosEventQueueId_t eventQueueId,
               elosEventFilterNodeId_t *eventFilterNodeId) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorFilterNodeCreate)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorFilterNodeCreate)(
                eventProcessor, filterStrings, filterStringCount, eventQueueId, eventFilterNodeId);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected_ptr(filterStrings);
            check_expected(filterStringCount);
            check_expected(eventQueueId);
            check_expected_ptr(eventFilterNodeId);
            if (eventFilterNodeId != NULL) {
                *eventFilterNodeId = mock_type(elosEventFilterNodeId_t);
            }
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorFilterNodeCreate)(
                eventProcessor, filterStrings, filterStringCount, eventQueueId, eventFilterNodeId);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorFilterNodeRemove, safuResultE_t, elosEventProcessor_t *eventProcessor,
               elosEventFilterNodeId_t eventFilterNodeId) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorFilterNodeRemove)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorFilterNodeRemove)(eventProcessor, eventFilterNodeId);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected(eventFilterNodeId);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorFilterNodeRemove)(eventProcessor, eventFilterNodeId);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorQueueCreate, safuResultE_t, elosEventProcessor_t *eventProcessor,
               elosEventQueueId_t *eventQueueId) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorQueueCreate)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorQueueCreate)(eventProcessor, eventQueueId);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected_ptr(eventQueueId);
            if (eventQueueId != NULL) {
                *eventQueueId = mock_type(elosEventQueueId_t);
            }
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorQueueCreate)(eventProcessor, eventQueueId);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorQueueRemove, safuResultE_t, elosEventProcessor_t *eventProcessor,
               elosEventQueueId_t eventQueueId) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorQueueRemove)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorQueueRemove)(eventProcessor, eventQueueId);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected(eventQueueId);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorQueueRemove)(eventProcessor, eventQueueId);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorQueueRead, safuResultE_t, elosEventProcessor_t *eventProcessor,
               elosEventQueueId_t eventQueueId, elosEventVector_t **eventVector) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorQueueRead)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorQueueRead)(eventProcessor, eventQueueId, eventVector);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected(eventQueueId);
            check_expected_ptr(eventVector);
            if (eventVector != NULL) {
                *eventVector = mock_type(elosEventVector_t *);
            }
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorQueueRead)(eventProcessor, eventQueueId, eventVector);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventProcessorPublish, safuResultE_t, elosEventProcessor_t *eventProcessor,
               elosEvent_t const *event) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventProcessorPublish)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventProcessorPublish)(eventProcessor, event);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventProcessor);
            check_expected_ptr(event);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventProcessorPublish)(eventProcessor, event);
            break;
    }

    return result;
}
