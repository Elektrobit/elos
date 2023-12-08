// SPDX-License-Identifier: MIT

#include "mock_plugincontrol.h"

MOCK_FUNC_BODY(elosPluginControlInitialize, safuResultE_t, elosPluginControl_t *control,
               elosPluginControlParam_t const *param) {
    if (MOCK_IS_ACTIVE(elosPluginControlInitialize)) {
        check_expected_ptr(control);
        check_expected_ptr(param);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlInitialize)(control, param);
}

MOCK_FUNC_BODY(elosPluginControlNew, safuResultE_t, elosPluginControl_t **control,
               elosPluginControlParam_t const *param) {
    if (MOCK_IS_ACTIVE(elosPluginControlNew)) {
        check_expected_ptr(control);
        check_expected_ptr(param);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlNew)(control, param);
}

MOCK_FUNC_BODY(elosPluginControlLoad, safuResultE_t, elosPluginControl_t *control) {
    if (MOCK_IS_ACTIVE(elosPluginControlLoad)) {
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlLoad)(control);
}

MOCK_FUNC_BODY(elosPluginControlUnload, safuResultE_t, elosPluginControl_t *control) {
    if (MOCK_IS_ACTIVE(elosPluginControlUnload)) {
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlUnload)(control);
}

MOCK_FUNC_BODY(elosPluginControlStart, safuResultE_t, elosPluginControl_t *control) {
    if (MOCK_IS_ACTIVE(elosPluginControlStart)) {
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlStart)(control);
}

MOCK_FUNC_BODY(elosPluginControlStop, safuResultE_t, elosPluginControl_t *control) {
    if (MOCK_IS_ACTIVE(elosPluginControlStop)) {
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlStop)(control);
}

MOCK_FUNC_BODY(elosPluginControlDeleteMembers, safuResultE_t, elosPluginControl_t *control) {
    if (MOCK_IS_ACTIVE(elosPluginControlDeleteMembers)) {
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlDeleteMembers)(control);
}

MOCK_FUNC_BODY(elosPluginControlDelete, safuResultE_t, elosPluginControl_t **control) {
    if (MOCK_IS_ACTIVE(elosPluginControlDelete)) {
        check_expected_ptr(control);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlDelete)(control);
}

MOCK_FUNC_BODY(elosPluginControlGetName, safuResultE_t, const elosPluginControl_t *control, const char **name) {
    if (MOCK_IS_ACTIVE(elosPluginControlGetName)) {
        check_expected_ptr(control);
        check_expected_ptr(name);
        *name = mock_type(const char *);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosPluginControlGetName)(control, name);
}
