// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>

#include "mock_libelosplugin.h"
#include "plugincontrol_stubs.h"

MOCK_FUNC_BODY(elosPluginControlInitialize, safuResultE_t, elosPluginControl_t *control,
               elosPluginControlParam_t const *param) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlInitialize)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlInitialize)(control, param);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            check_expected_ptr(param);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlInitialize(control, param);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlNew, safuResultE_t, elosPluginControl_t **control,
               elosPluginControlParam_t const *param) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlNew)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlNew)(control, param);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            check_expected_ptr(param);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlNew(control, param);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlLoad, safuResultE_t, elosPluginControl_t *control) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlLoad)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlLoad)(control);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlLoad(control);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlUnload, safuResultE_t, elosPluginControl_t *control) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlUnload)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlUnload)(control);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlUnload(control);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlStart, safuResultE_t, elosPluginControl_t *control) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlStart)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlStart)(control);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlStart(control);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlStop, safuResultE_t, elosPluginControl_t *control) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlStop)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlStop)(control);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlStop(control);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlDeleteMembers, safuResultE_t, elosPluginControl_t *control) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlDeleteMembers)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlDeleteMembers)(control);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlDeleteMembers(control);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlDelete, safuResultE_t, elosPluginControl_t **control) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlDelete)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlDelete)(control);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlDelete(control);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlGetName, safuResultE_t, const elosPluginControl_t *control, const char **name) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlGetName)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlGetName)(control, name);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(control);
            check_expected_ptr(name);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlGetName(control, name);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlCreatePublisher, safuResultE_t, struct elosPluginControl *pluginControl,
               struct elosPublisher **publisher) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlCreatePublisher)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlCreatePublisher)(pluginControl, publisher);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(pluginControl);
            check_expected_ptr(publisher);
            if (publisher != NULL) {
                *publisher = mock_type(struct elosPublisher *);
            }
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlCreatePublisher(pluginControl, publisher);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosPluginControlDeletePublisher, safuResultE_t, struct elosPluginControl *pluginControl,
               struct elosPublisher *publisher) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosPluginControlDeletePublisher)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosPluginControlDeletePublisher)(pluginControl, publisher);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(pluginControl);
            check_expected_ptr(publisher);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = elosPluginControlDeletePublisher(pluginControl, publisher);
            break;
    }

    return result;
}