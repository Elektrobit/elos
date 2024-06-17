// SPDX-License-Identifier: MIT
#include "connectionmanager/clientblacklist.h"

#include <safu/common.h>
#include <safu/log.h>
#include <samconf/samconf.h>

safuResultE_t elosBlacklistInitialize(elosEventFilter_t *blacklist, samconfConfig_t const *const config) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    const char *paramstring = NULL;

    if (blacklist == NULL || config == NULL) {
        safuLogErr("Invalid argument");
    } else {
        *blacklist = (elosEventFilter_t)ELOS_EVENTFILTER_INIT;
        status = samconfConfigGetString(config, "/Config/EventBlacklist", &paramstring);
        if (status == SAMCONF_CONFIG_OK) {
            if (paramstring != NULL && paramstring[0] != '\0') {
                elosEventFilterParam_t param = {.filterString = paramstring};
                result = elosEventFilterCreate(blacklist, &param);
                if (result == RPNFILTER_RESULT_OK && blacklist->state == RPNFILTER_FINALIZED) {
                    retval = SAFU_RESULT_OK;
                } else {
                    safuLogErrF("event filter creation failed with : %d", result);
                }
            } else {
                safuLogErr("blacklist filter string is not set in config");
            }
        } else {
            safuLogErrF("blacklist attribute retrieval from config failed : %d", status);
        }
    }

    return retval;
}

safuResultE_t elosBlacklistDelete(elosEventFilter_t *blacklist) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosRpnFilterResultE_t retval = RPNFILTER_RESULT_ERROR;

    if (blacklist == NULL) {
        safuLogErr("Invalid argument");
    } else {
        retval = elosEventFilterDeleteMembers(blacklist);
        if (retval == RPNFILTER_RESULT_OK) {
            result = SAFU_RESULT_OK;
        } else {
            safuLogDebug("not able to delete blacklist filter");
        }
    }

    return result;
}
