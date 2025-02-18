// SPDX-License-Identifier: MIT

#include "mock_kmsg_mapper.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_VAR_NEW(elosKmsgMapperInit);
safuResultE_t MOCK_FUNC_WRAP(elosKmsgMapperInit)(elosKmsgMapper_t *mapper) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosKmsgMapperInit)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosKmsgMapperInit)(mapper);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(mapper);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosKmsgMapperInit)(mapper);
            break;
    }

    return result;
}