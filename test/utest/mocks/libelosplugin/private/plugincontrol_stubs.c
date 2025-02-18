// SPDX-License-Identifier: MIT
#include "plugincontrol_stubs.h"

#include <safu/common.h>

safuResultE_t elosPluginControlInitialize(UNUSED elosPluginControl_t *control,
                                          UNUSED elosPluginControlParam_t const *param) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlNew(UNUSED elosPluginControl_t **control, UNUSED elosPluginControlParam_t const *param) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlLoad(UNUSED elosPluginControl_t *control) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlUnload(UNUSED elosPluginControl_t *control) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlStart(UNUSED elosPluginControl_t *control) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlStop(UNUSED elosPluginControl_t *control) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlDeleteMembers(UNUSED elosPluginControl_t *control) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlDelete(UNUSED elosPluginControl_t **control) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlGetName(UNUSED const elosPluginControl_t *control, UNUSED const char **name) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlCreatePublisher(UNUSED struct elosPluginControl *pluginControl,
                                               UNUSED struct elosPublisher **publisher) {
    return SAFU_RESULT_FAILED;
}

safuResultE_t elosPluginControlDeletePublisher(UNUSED struct elosPluginControl *pluginControl,
                                               UNUSED struct elosPublisher *publisher) {
    return SAFU_RESULT_FAILED;
}