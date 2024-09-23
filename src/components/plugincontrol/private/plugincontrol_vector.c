// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/plugincontrol/plugincontrol.h"
#include "elos/plugincontrol/vector.h"

safuResultE_t elosPluginControlVectorInitialize(elosPluginControlVector_t *controlVector, size_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (controlVector == NULL) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecCreate(controlVector, elements, sizeof(elosPluginControl_t *));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginControlVectorPush(elosPluginControlVector_t *controlVector, elosPluginControl_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((controlVector == NULL) || (plugin == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecPush(controlVector, &plugin);
        if (retVal < 0) {
            safuLogErr("safuVecPush failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginControlVectorIterate(elosPluginControlVector_t *controlVector, safuVecFunc_t *func, void *data,
                                             int *iterResult) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((controlVector == NULL) || (func == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecIterate(controlVector, func, data);
        if (retVal < 0) {
            safuLogWarn("safuVecIterate failed");
        } else {
            result = SAFU_RESULT_OK;
        }

        if (iterResult != NULL) {
            *iterResult = retVal;
        }
    }

    return result;
}

static int _pluginVectorMatchById(const void *element, const void *data) {
    elosPluginControl_t const *control = *(elosPluginControl_t const **)element;
    elosPluginId_t const id = *(elosPluginId_t const *)data;
    int result = 0;

    if (control->context.id == id) {
        result = 1;
    }

    return result;
}

safuResultE_t elosPluginControlVectorGetById(elosPluginControlVector_t *controlVector, elosPluginId_t id,
                                             elosPluginControl_t **control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((controlVector == NULL) || (id == ELOS_ID_INVALID) || (control == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;
        elosPluginControl_t **tmpControl = NULL;

        retVal = safuVecFindGet(controlVector, (void **)&tmpControl, NULL, _pluginVectorMatchById, &id);
        if (retVal < 0) {
            safuLogErr("safuVecFindGet failed");
        } else if (retVal == 0) {
            safuLogErrF("safuVecFindGet couldn't find plugin with id '%d' in vector", id);
        } else {
            *control = *tmpControl;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static int _pluginVectorRemoveById(const void *element, const void *data) {
    elosPluginControl_t *control = *(elosPluginControl_t **)element;
    elosPluginId_t const id = *(elosPluginId_t const *)data;
    int result = 0;

    if (control->context.id == id) {
        result = elosPluginControlDeleteMembers(control);
        if (result < 0) {
            safuLogWarn("elosPluginControlDeleteMembers failed (likely creating a memory leak)");
        }
        free(control);

        result = 1;
    }

    return result;
}

safuResultE_t elosPluginControlVectorRemoveById(elosPluginControlVector_t *controlVector, elosPluginId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((controlVector == NULL) || (id == ELOS_ID_INVALID)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecFindRemove(controlVector, _pluginVectorRemoveById, &id);
        if (retVal < 0) {
            safuLogErr("safuVecFindRemove failed");
        } else if (retVal == 0) {
            safuLogErrF("safuVecFindRemove couldn't find plugin with id '%d' in vector", id);
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static int _deleteMemberfunc(void const *element, UNUSED void const *data) {
    safuResultE_t result;
    elosPluginControl_t *control = *(elosPluginControl_t **)element;

    result = elosPluginControlDeleteMembers(control);
    if (result < 0) {
        safuLogWarn("elosPluginVectorDeleteMembers failed (likely creating a memory leak)");
    }
    free(control);

    return 0;
}

safuResultE_t elosPluginControlVectorDeleteMembers(elosPluginControlVector_t *controlVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (controlVector == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosPluginControlVectorIterate(controlVector, _deleteMemberfunc, NULL, NULL);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginVectorIterate failed");
        } else {
            int retVal;

            retVal = safuVecFree(controlVector);
            if (retVal < 0) {
                safuLogErr("safuVecCreate failed");
            } else {
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}
