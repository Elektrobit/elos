// SPDX-License-Identifier: MIT

#include "mock_plugincontrol_vector.h"

MOCK_FUNC_BODY(elosPluginControlVectorInitialize, safuResultE_t, elosPluginControlVector_t *controlVector,
               size_t elements) {
    if (MOCK_IS_ACTIVE(elosPluginControlVectorInitialize)) {
        check_expected_ptr(controlVector);
        check_expected(elements);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlVectorInitialize)(controlVector, elements);
}

MOCK_FUNC_BODY(elosPluginControlVectorPush, safuResultE_t, elosPluginControlVector_t *controlVector,
               elosPluginControl_t *control) {
    if (MOCK_IS_ACTIVE(elosPluginControlVectorPush)) {
        check_expected_ptr(controlVector);
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlVectorPush)(controlVector, control);
}

MOCK_FUNC_BODY(elosPluginControlVectorIterate, safuResultE_t, elosPluginControlVector_t *controlVector,
               safuVecFunc_t *func, void *data, int *iterResult) {
    if (MOCK_IS_ACTIVE(elosPluginControlVectorIterate)) {
        check_expected_ptr(controlVector);
        check_expected_ptr(func);
        check_expected_ptr(data);
        check_expected_ptr(iterResult);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlVectorIterate)(controlVector, func, data, iterResult);
}

MOCK_FUNC_BODY(elosPluginControlVectorGetById, safuResultE_t, elosPluginControlVector_t *controlVector,
               elosPluginId_t id, elosPluginControl_t **control) {
    if (MOCK_IS_ACTIVE(elosPluginControlVectorGetById)) {
        check_expected_ptr(controlVector);
        check_expected(id);
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlVectorGetById)(controlVector, id, control);
}

MOCK_FUNC_BODY(elosPluginControlVectorRemoveById, safuResultE_t, elosPluginControlVector_t *controlVector,
               elosPluginId_t id) {
    if (MOCK_IS_ACTIVE(elosPluginControlVectorRemoveById)) {
        check_expected_ptr(controlVector);
        check_expected(id);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlVectorRemoveById)(controlVector, id);
}

MOCK_FUNC_BODY(elosPluginControlVectorDeleteMembers, safuResultE_t, elosPluginControlVector_t *controlVector) {
    if (MOCK_IS_ACTIVE(elosPluginControlVectorDeleteMembers)) {
        check_expected_ptr(controlVector);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlVectorDeleteMembers)(controlVector);
}
