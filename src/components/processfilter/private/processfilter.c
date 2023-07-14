// SPDX-License-Identifier: MIT

#include "elos/processfilter/processfilter.h"

#include <stdlib.h>
#include <string.h>

#include "elos/processfilter/builder.h"
#include "safu/common.h"
#include "safu/log.h"

inline elosRpnFilterResultE_t elosProcessFilterInit(elosProcessFilter_t *filter) {
    return elosRpnFilterInit((elosRpnFilter_t *)filter);
}

inline elosRpnFilterResultE_t elosProcessFilterNew(elosProcessFilter_t **filter) {
    return elosRpnFilterNew((elosRpnFilter_t **)filter);
}

inline elosRpnFilterResultE_t elosProcessFilterDeepCopy(elosProcessFilter_t *to, const elosProcessFilter_t *from) {
    return elosRpnFilterDeepCopy((elosRpnFilter_t *)to, (const elosRpnFilter_t *)from);
}

inline elosRpnFilterResultE_t elosProcessFilterClone(elosProcessFilter_t **to, const elosProcessFilter_t *from) {
    return elosRpnFilterClone((elosRpnFilter_t **)to, (const elosRpnFilter_t *)from);
}

inline elosRpnFilterResultE_t elosProcessFilterDeleteMembers(elosProcessFilter_t *filter) {
    return elosRpnFilterDeleteMembers((elosRpnFilter_t *)filter);
}

inline elosRpnFilterResultE_t elosProcessFilterDelete(elosProcessFilter_t *filter) {
    return elosRpnFilterDelete((elosRpnFilter_t *)filter);
}

elosRpnFilterResultE_t elosProcessFilterCreate(elosProcessFilter_t *filter, const elosProcessFilterParam_t *param) {
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
            result = elosProcessFilterBuilderTokenize(data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("elosRpnFilterBuilderTokenize failed");
            } else {
                result = elosProcessFilterBuilderCompile(data);
                if (result == RPNFILTER_RESULT_ERROR) {
                    safuLogErr("elosRpnFilterBuilderCompile failed");
                } else {
                    result = elosRpnFilterBuilderFinalize(data, filter);
                    if (result == RPNFILTER_RESULT_ERROR) {
                        safuLogErr("elosProcessFilterBuilderFinalize failed");
                    }
                }
            }
        }

        elosRpnFilterBuilderDelete(data);
    }

    return result;
}
