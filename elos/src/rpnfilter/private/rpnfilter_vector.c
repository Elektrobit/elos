// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>

#include "elos/rpnfilter/rpnfilter.h"
#include "elos/rpnfilter/vector.h"

elosRpnFilterResultE_t elosRpnFilterVectorInit(elosRpnFilterVector_t *rpnFilterVector, uint32_t elements) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if (rpnFilterVector == NULL) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecCreate(rpnFilterVector, elements, sizeof(elosRpnFilter_t));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
        } else {
            result = RPNFILTER_RESULT_OK;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterVectorPush(elosRpnFilterVector_t *rpnFilterVector, elosRpnFilter_t *rpnFilter) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((rpnFilterVector == NULL) || (rpnFilter == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecPush(rpnFilterVector, rpnFilter);
        if (retVal < 0) {
            safuLogErr("safuVecPush failed");
        } else {
            result = RPNFILTER_RESULT_OK;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterVectorIterate(elosRpnFilterVector_t *rpnFilterVector, safuVecFunc_t *func,
                                                  void *data) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if ((rpnFilterVector == NULL) || (func == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecIterate(rpnFilterVector, func, data);
        if (retVal < 0) {
            safuLogWarn("safuVecIterate failed");
        } else {
            result = RPNFILTER_RESULT_OK;
        }
    }

    return result;
}

static int _deleteMemberfunc(void const *element, UNUSED void const *data) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t *rpnFilter = (elosRpnFilter_t *)element;

    result = elosRpnFilterDeleteMembers(rpnFilter);
    if (result < 0) {
        safuLogWarn("elosRpnFilterVectorDeleteMembers failed (likely a memory leak)");
    }

    return 0;
}

elosRpnFilterResultE_t elosRpnFilterVectorDeleteMembers(elosRpnFilterVector_t *rpnFilterVector) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;

    if (rpnFilterVector == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosRpnFilterVectorIterate(rpnFilterVector, _deleteMemberfunc, NULL);
        if (result == RPNFILTER_RESULT_ERROR) {
            safuLogErr("elosRpnFilterVectorIterate failed");
        } else {
            int retVal;

            retVal = safuVecFree(rpnFilterVector);
            if (retVal < 0) {
                safuLogErr("safuVecCreate failed");
            } else {
                result = RPNFILTER_RESULT_OK;
            }
        }
    }

    return result;
}
