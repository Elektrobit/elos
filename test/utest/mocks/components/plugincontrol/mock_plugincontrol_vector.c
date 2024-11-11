// SPDX-License-Identifier: MIT

#include "mock_plugincontrol_vector.h"

MOCK_FUNC_BODY(elosPluginControlPtrVectorInitialize, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
               size_t elements) {
    if (MOCK_IS_ACTIVE(elosPluginControlPtrVectorInitialize)) {
        check_expected_ptr(controlPtrVector);
        check_expected(elements);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlPtrVectorInitialize)(controlPtrVector, elements);
}

MOCK_FUNC_BODY(elosPluginControlPtrVectorPush, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
               elosPluginControl_t *control) {
    if (MOCK_IS_ACTIVE(elosPluginControlPtrVectorPush)) {
        check_expected_ptr(controlPtrVector);
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlPtrVectorPush)(controlPtrVector, control);
}

MOCK_FUNC_BODY(elosPluginControlPtrVectorIterate, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
               safuVecFunc_t *func, void *data, int *iterResult) {
    if (MOCK_IS_ACTIVE(elosPluginControlPtrVectorIterate)) {
        check_expected_ptr(controlPtrVector);
        check_expected_ptr(func);
        check_expected_ptr(data);
        check_expected_ptr(iterResult);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlPtrVectorIterate)(controlPtrVector, func, data, iterResult);
}

MOCK_FUNC_BODY(elosPluginControlPtrVectorGetById, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
               elosPluginId_t id, elosPluginControl_t **control) {
    if (MOCK_IS_ACTIVE(elosPluginControlPtrVectorGetById)) {
        check_expected_ptr(controlPtrVector);
        check_expected(id);
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlPtrVectorGetById)(controlPtrVector, id, control);
}

MOCK_FUNC_BODY(elosPluginControlPtrVectorRemoveById, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
               elosPluginId_t id) {
    if (MOCK_IS_ACTIVE(elosPluginControlPtrVectorRemoveById)) {
        check_expected_ptr(controlPtrVector);
        check_expected(id);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlPtrVectorRemoveById)(controlPtrVector, id);
}

MOCK_FUNC_BODY(elosPluginControlPtrVectorDeleteMembers, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector) {
    if (MOCK_IS_ACTIVE(elosPluginControlPtrVectorDeleteMembers)) {
        check_expected_ptr(controlPtrVector);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlPtrVectorDeleteMembers)(controlPtrVector);
}
