// SPDX-License-Identifier: MIT

#include "mock_eventqueuevector.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventQueueVectorPushByParam, safuResultE_t, elosEventQueueVector_t *eventQueueVector,
               elosEventQueueParam_t const *param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueVectorPushByParam)) {
        check_expected_ptr(eventQueueVector);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueVectorPushByParam)(eventQueueVector, param);
    }

    return result;
}
