// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <string.h>

#include "elos/rpnfilter/builder.h"
#include "safu/common.h"
#include "safu/log.h"

elosRpnFilterResultE_t elosRpnFilterInit(elosRpnFilter_t *filter) {
    const elosRpnFilter_t init = ELOS_RPNFILTER_INIT;

    memcpy(filter, &init, sizeof(elosRpnFilter_t));

    return RPNFILTER_RESULT_OK;
}

elosRpnFilterResultE_t elosRpnFilterNew(elosRpnFilter_t **filter) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    void *ptr = NULL;

    ptr = safuAllocMem(NULL, sizeof(elosRpnFilter_t));
    if (ptr == NULL) {
        safuLogErrErrno("safuAllocMem");
    } else {
        result = elosRpnFilterInit(ptr);
        if (result != RPNFILTER_RESULT_OK) {
            safuLogErr("elosRpnFilterInitialize failed");
        } else {
            *filter = ptr;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterDeepCopy(UNUSED elosRpnFilter_t *to, UNUSED const elosRpnFilter_t *from) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;

    safuLogWarnF("%s not implemented yet!", __func__);

    return result;
}

elosRpnFilterResultE_t elosRpnFilterClone(elosRpnFilter_t **to, const elosRpnFilter_t *from) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t *ptr = NULL;

    result = elosRpnFilterNew(&ptr);
    if (result != RPNFILTER_RESULT_OK) {
        safuLogErr("elosRpnFilterNew failed");
    } else {
        result = elosRpnFilterDeepCopy(ptr, from);
        if (result != RPNFILTER_RESULT_OK) {
            safuLogErr("elosRpnFilterDeepCopy failed");
        } else {
            *to = ptr;
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterDeleteMembers(elosRpnFilter_t *filter) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;

    if (filter == NULL) {
        result = RPNFILTER_RESULT_ERROR;
    } else if (filter->memory != NULL) {
        free(filter->memory);
        memset(filter, 0, sizeof(elosRpnFilter_t));
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterDelete(elosRpnFilter_t *filter) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;

    if (filter != NULL) {
        result = elosRpnFilterDeleteMembers(filter);
        free(filter);
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterCreate(elosRpnFilter_t *filter, const elosRpnFilterParam_t *param) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    elosRpnFilterBuilder_t *data = NULL;

    if ((filter == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if ((param->filterString == NULL) || (param->filterString[0] == '\0')) {
        safuLogErr("Invalid filterString");
    } else {
        result = elosRpnFilterBuilderNew(param, &data);
        if (result == RPNFILTER_RESULT_ERROR) {
            safuLogErr("elosRpnFilterBuilderNew failed");
        } else {
            result = elosRpnFilterBuilderTokenize(data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("elosRpnFilterBuilderTokenize failed");
            } else {
                result = elosRpnFilterBuilderCompile(data);
                if (result == RPNFILTER_RESULT_ERROR) {
                    safuLogErr("elosRpnFilterBuilderCompile failed");
                } else {
                    result = elosRpnFilterBuilderFinalize(data, filter);
                    if (result == RPNFILTER_RESULT_ERROR) {
                        safuLogErr("elosRpnFilterBuilderFinalize failed");
                    }
                }
            }
        }

        elosRpnFilterBuilderDelete(data);
    }

    return result;
}
