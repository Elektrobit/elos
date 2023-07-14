// SPDX-License-Identifier: MIT
#include "elos/eventlogging/StorageBackend.h"

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>

#include "elos/event/event.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/rpnfilter/rpnfilter.h"

typedef struct _foreachFilterData {
    elosEvent_t const *event;
    elosRpnFilterResultE_t filterResult;
} _foreachFilterData_t;

static int _foreachFilter(void const *element, void const *data) {
    elosEventFilter_t *filter = (elosEventFilter_t *)element;
    _foreachFilterData_t *helperData = (_foreachFilterData_t *)data;

    if (helperData->filterResult != RPNFILTER_RESULT_MATCH && helperData->filterResult == RPNFILTER_RESULT_NO_MATCH) {
        helperData->filterResult = elosEventFilterExecute(filter, NULL, (elosEvent_t *)helperData->event);
    }

    return 0;
}

safuResultE_t elosStorageBackendAccepts(const elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend == NULL || event == NULL) {
        safuLogErr("Invalid parameter");
        result = SAFU_RESULT_FAILED;
    } else {
        result = SAFU_RESULT_OK;
    }

    if (result == SAFU_RESULT_OK) {
        _foreachFilterData_t helperData = {.event = event, .filterResult = RPNFILTER_RESULT_NO_MATCH};
        safuVecIterate(&backend->filter, _foreachFilter, &helperData);

        if (helperData.filterResult == RPNFILTER_RESULT_MATCH) {
            safuLogDebug("Match");
            result = SAFU_RESULT_OK;
        } else if (helperData.filterResult == RPNFILTER_RESULT_NO_MATCH) {
            safuLogDebug("No Match");
            result = SAFU_RESULT_NOT_FOUND;
        } else {
            safuLogErr("elosEventFilterExecute failed");
            safuLogDebugF("Free Event: RPN filter result is: %d", helperData.filterResult);
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}
