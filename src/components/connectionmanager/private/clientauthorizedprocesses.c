// SPDX-License-Identifier: MIT
#include "elos/connectionmanager/clientauthorizedprocesses.h"

#include <safu/vector.h>
#include <stddef.h>
#include <stdio.h>

#include "elos/processfilter/processfilter.h"
#include "safu/log.h"

safuResultE_t _createFilter(const char *filterRule, safuVec_t *filterList) {
    safuResultE_t result = SAFU_RESULT_OK;

    elosProcessFilter_t processFilter = (elosProcessFilter_t)ELOS_PROCESSFILTER_INIT;
    const elosProcessFilterParam_t param = {.filterString = filterRule};

    elosRpnFilterResultE_t filterResult = elosProcessFilterCreate(&processFilter, &param);

    if (filterResult == RPNFILTER_RESULT_ERROR) {
        safuLogWarnF("Failed to create filter for rule '%s'", filterRule);
    } else {
        int retValue = safuVecPush(filterList, &processFilter);
        if (retValue != 0) {
            safuLogWarn("Failed to insert process filter into filter list");
            elosProcessFilterDeleteMembers(&processFilter);
        }
    }

    return result;
}

safuResultE_t _loadFilterRules(const samconfConfig_t *config, safuVec_t *filterList) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retValue = safuVecCreate(filterList, config->childCount, sizeof(elosProcessFilter_t));

    if (retValue == 0) {
        for (size_t childItr = 0; childItr < config->childCount; childItr++) {
            if (config->children[childItr]->type == SAMCONF_CONFIG_VALUE_STRING) {
                result = _createFilter(config->children[childItr]->value.string, filterList);
                if (result == SAFU_RESULT_FAILED) {
                    safuLogWarnF("Failed to create authorized process filter: %s",
                                 config->children[childItr]->value.string);
                }
            } else {
                safuLogErr("authorized process filter is not a string");
            }
        }
    } else {
        safuLogErr("Failed to create vector of filters");
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

safuResultE_t elosAuthorizedProcessInitialize(safuVec_t *authorizedprocesses, const samconfConfig_t *config) {
    const samconfConfig_t *authProcConfig = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    safuResultE_t result = SAFU_RESULT_OK;

    if (authorizedprocesses == NULL || config == NULL) {
        safuLogErr("Invalid or null parameter");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        status = samconfConfigGet(config, "/Config/authorizedProcesses", &authProcConfig);
        if (status != SAMCONF_CONFIG_OK) {
            safuLogErr("Given configuration does not have authorized process filters");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = _loadFilterRules(authProcConfig, authorizedprocesses);
    }

    return result;
}

safuResultE_t elosAuthorizedProcessDelete(safuVec_t *authorizedprocesses) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosRpnFilterResultE_t retval = RPNFILTER_RESULT_ERROR;
    elosProcessFilter_t *processFilter = NULL;
    int deleteFail = 0;

    if (authorizedprocesses == NULL) {
        safuLogErr("Invalid argument");
        deleteFail = 1;
    } else {
        size_t filterCount = safuVecElements(authorizedprocesses);
        for (size_t i = 0; i < filterCount; i++) {
            processFilter = safuVecGet(authorizedprocesses, i);
            retval = elosProcessFilterDeleteMembers(processFilter);
            if (retval != RPNFILTER_RESULT_OK) {
                safuLogWarn("not able to delete authorized process filter");
                deleteFail++;
            }
        }
        safuVecFree(authorizedprocesses);
    }

    if (deleteFail == 0) {
        result = SAFU_RESULT_OK;
    }

    return result;
}

bool elosAuthorizedProcessCheck(safuVec_t *authorizedprocesses, elosProcessIdentity_t *process) {
    bool retval = false;
    elosProcessFilter_t *processFilter;
    elosRpnFilterResultE_t filterResult = RPNFILTER_RESULT_ERROR;

    if (authorizedprocesses == NULL || process == NULL) {
        safuLogErr("Invalid or null parameter");
    } else {
        size_t filterCount = safuVecElements(authorizedprocesses);

        for (size_t i = 0; i < filterCount; i++) {
            processFilter = (elosProcessFilter_t *)safuVecGet(authorizedprocesses, i);
            filterResult = elosProcessFilterExecute(processFilter, NULL, process);
            if (filterResult == RPNFILTER_RESULT_MATCH) {
                safuLogInfo("client matches is an authorized processes");
                retval = true;
            }
        }
    }

    return retval;
}
