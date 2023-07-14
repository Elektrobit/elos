// SPDX-License-Identifier: MIT

#include "elos/eventfilter/eventfilter.h"

#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>
#include <string.h>

#include "elos/eventfilter/builder.h"
#include "elos/eventfilter/types.h"
#include "elos/rpnfilter/builder.h"

inline elosRpnFilterResultE_t elosEventFilterInit(elosEventFilter_t *filter) {
    return elosRpnFilterInit((elosRpnFilter_t *)filter);
}

inline elosRpnFilterResultE_t elosEventFilterNew(elosEventFilter_t **filter) {
    return elosRpnFilterNew((elosRpnFilter_t **)filter);
}

inline elosRpnFilterResultE_t elosEventFilterDeepCopy(elosEventFilter_t *to, const elosEventFilter_t *from) {
    return elosRpnFilterDeepCopy((elosRpnFilter_t *)to, (const elosRpnFilter_t *)from);
}

inline elosRpnFilterResultE_t elosEventFilterClone(elosEventFilter_t **to, const elosEventFilter_t *from) {
    return elosRpnFilterClone((elosRpnFilter_t **)to, (const elosRpnFilter_t *)from);
}

inline elosRpnFilterResultE_t elosEventFilterDeleteMembers(elosEventFilter_t *filter) {
    return elosRpnFilterDeleteMembers((elosRpnFilter_t *)filter);
}

inline elosRpnFilterResultE_t elosEventFilterDelete(elosEventFilter_t *filter) {
    return elosRpnFilterDelete((elosRpnFilter_t *)filter);
}

elosRpnFilterResultE_t elosEventFilterCreate(elosEventFilter_t *filter, const elosEventFilterParam_t *param) {
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
            result = elosEventFilterBuilderTokenize(data);
            if (result == RPNFILTER_RESULT_ERROR) {
                safuLogErr("elosRpnFilterBuilderTokenize failed");
            } else {
                result = elosEventFilterBuilderCompile(data);
                if (result == RPNFILTER_RESULT_ERROR) {
                    safuLogErr("elosRpnFilterBuilderCompile failed");
                } else {
                    result = elosRpnFilterBuilderFinalize(data, filter);
                    if (result == RPNFILTER_RESULT_ERROR) {
                        safuLogErr("elosEventFilterBuilderFinalize failed");
                    }
                }
            }
        }

        elosRpnFilterBuilderDelete(data);
    }

    return result;
}
