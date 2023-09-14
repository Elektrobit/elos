// SPDX-License-Identifier: MIT

#include "eventprocessor/eventfilternodevector.h"

#include <safu/log.h>
#include <safu/vector.h>

#include "elos/eventfilter/eventfilter.h"

safuResultE_t elosEventFilterNodeVectorInitialize(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                                  uint32_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventFilterNodeVector != NULL) {
        int retVal;

        retVal = safuVecCreate(eventFilterNodeVector, elements, sizeof(elosEventFilterNode_t));
        if (retVal == 0) {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosEventFilterNodeVectorPushByParam(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                                   elosEventFilterNodeParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventFilterNodeVector == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        elosEventFilterNode_t newEventFilterNode = {0};
        result = elosEventFilterNodeInitialize(&newEventFilterNode, param);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosEventFilterNodeInitialize failed");
        } else {
            int retVal;
            retVal = safuVecPush(eventFilterNodeVector, &newEventFilterNode);
            if (retVal < 0) {
                safuLogErr("safuVecPush failed");
                elosEventFilterNodeDeleteMembers(&newEventFilterNode);
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

static int _matchElementById(const void *element, const void *data) {
    elosEventFilterNode_t const *eventFilterNode = (elosEventFilterNode_t const *)element;
    elosEventFilterNodeId_t const id = *(elosEventFilterNodeId_t const *)data;
    int result = 0;

    if (eventFilterNode->id == id) {
        result = 1;
    }

    return result;
}

safuResultE_t elosEventFilterNodeVectorRemoveById(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                                  elosEventFilterNodeId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventFilterNodeVector == NULL) || (id == ELOS_EVENTQUEUE_ID_INVALID)) {
        safuLogErrF("%s", "Invalid parameter");
    } else {
        elosEventFilterNode_t *eventFilterNode = NULL;
        uint32_t idx = 0;
        int retVal;

        retVal = safuVecFindGet(eventFilterNodeVector, (void **)&eventFilterNode, &idx, _matchElementById, &id);
        if (retVal > 0) {
            result = elosEventFilterNodeDeleteMembers(eventFilterNode);
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("elosEventFilterNodeDeleteMembers failed (possible memory leak)");
            }

            retVal = safuVecRemove(eventFilterNodeVector, idx);
            if (retVal < 0) {
                safuLogErrF("%s", "safuVecRemove failed");
                result = SAFU_RESULT_FAILED;
            }
        } else {
            safuLogErrF("eventFilterNodeId '%u' not found", id);
        }
    }

    return result;
}

static int _deleteMembersFunc(const void *element, UNUSED const void *data) {
    elosEventFilterNode_t *efe = (elosEventFilterNode_t *)element;
    safuResultE_t result;

    result = elosEventFilterNodeDeleteMembers(efe);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("elosEventFilterNodeDeleteMembers failed (possible memory leak)");
    }

    return 0;
}

safuResultE_t elosEventFilterNodeVectorDeleteMembers(elosEventFilterNodeVector_t *eventFilterNodeVector) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((eventFilterNodeVector != NULL) && (eventFilterNodeVector->data != NULL)) {
        int retVal;

        retVal = safuVecIterate(eventFilterNodeVector, _deleteMembersFunc, NULL);
        if (retVal < 0) {
            safuLogWarn("safuVecFind failed (possible memory leak)");
            result = SAFU_RESULT_FAILED;
        }

        retVal = safuVecFree(eventFilterNodeVector);
        if (retVal < 0) {
            safuLogWarn("safuVecFree failed (possible memory leak)");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

typedef struct _processData {
    elosEventFilterStack_t const *filterStack;
    elosEvent_t const *event;
} _processData_t;

static int _processFunc(const void *element, const void *data) {
    elosEventFilterNode_t *efn = (elosEventFilterNode_t *)element;
    _processData_t *processData = (_processData_t *)data;
    safuResultE_t retVal;
    int result = 0;

    retVal = elosEventFilterNodeProcess(efn, processData->filterStack, processData->event);
    if (retVal != SAFU_RESULT_OK) {
        safuLogErr("elosEventFilterNodeProcess failed");
        result = 1;
    }

    return result;
}

safuResultE_t elosEventFilterNodeVectorProcess(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                               elosEventFilterStack_t const *filterStack, elosEvent_t const *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((eventFilterNodeVector == NULL) || (event == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        _processData_t const processData = {.filterStack = filterStack, .event = event};
        int retVal;

        retVal = safuVecIterate(eventFilterNodeVector, _processFunc, (void const *)&processData);
        if ((retVal < 0) || (retVal == 1)) {
            safuLogWarn("safuVecIterate failed (events might be lost)");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
