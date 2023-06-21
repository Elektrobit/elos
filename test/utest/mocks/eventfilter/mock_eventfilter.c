// SPDX-License-Identifier: MIT

#include "mock_eventfilter.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventFilterInit, elosRpnFilterResultE_t, elosEventFilter_t *filter) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterInit)) {
        check_expected_ptr(filter);
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterInit)(filter);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNew, elosRpnFilterResultE_t, elosEventFilter_t **filter) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNew)) {
        check_expected_ptr(filter);
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNew)(filter);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterDeepCopy, elosRpnFilterResultE_t, elosEventFilter_t *filter,
               const elosRpnFilter_t *from) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterDeepCopy)) {
        check_expected_ptr(filter);
        check_expected_ptr(from);
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterDeepCopy)(filter, from);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterClone, elosRpnFilterResultE_t, elosEventFilter_t **filter, const elosRpnFilter_t *from) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterClone)) {
        check_expected_ptr(filter);
        check_expected_ptr(from);
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterClone)(filter, from);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterDeleteMembers, elosRpnFilterResultE_t, elosEventFilter_t *filter) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterDeleteMembers)) {
        check_expected_ptr(filter);
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterDeleteMembers)(filter);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterDelete, elosRpnFilterResultE_t, elosEventFilter_t *filter) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterDelete)) {
        check_expected_ptr(filter);
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterDelete)(filter);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterCreate, elosRpnFilterResultE_t, elosEventFilter_t *filter,
               const elosEventFilterParam_t *param) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterCreate)) {
        check_expected_ptr(param);
        check_expected_ptr(filter);
        if (filter != NULL) {
            *filter = *mock_type(elosEventFilter_t *);
        }
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterCreate)(filter, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterExecute, elosRpnFilterResultE_t, const elosEventFilter_t *filter,
               const elosEventFilterStack_t *param, elosEvent_t *input) {
    elosRpnFilterResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterExecute)) {
        check_expected_ptr(filter);
        check_expected_ptr(param);
        check_expected_ptr(input);
        result = mock_type(elosRpnFilterResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterExecute)(filter, param, input);
    }

    return result;
}
