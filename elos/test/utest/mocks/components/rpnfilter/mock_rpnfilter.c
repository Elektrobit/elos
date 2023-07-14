// SPDX-License-Identifier: MIT

#include "mock_rpnfilter.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosRpnFilterDeleteMembers, elosRpnFilterResultE_t, elosRpnFilter_t *filter) {
    if (MOCK_IS_ACTIVE(elosRpnFilterDeleteMembers)) {
        check_expected_ptr(filter);
        return mock_type(elosRpnFilterResultE_t);
    }
    return MOCK_FUNC_REAL(elosRpnFilterDeleteMembers)(filter);
}

MOCK_FUNC_BODY(elosRpnFilterVectorPush, elosRpnFilterResultE_t, elosRpnFilterVector_t *rpnFilterVector,
               elosRpnFilter_t *rpnFilter) {
    if (MOCK_IS_ACTIVE(elosRpnFilterVectorPush)) {
        check_expected_ptr(rpnFilterVector);
        check_expected_ptr(rpnFilter);
        return mock_type(elosRpnFilterResultE_t);
    }
    return MOCK_FUNC_REAL(elosRpnFilterVectorPush)(rpnFilterVector, rpnFilter);
}
