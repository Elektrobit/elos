// SPDX-License-Identifier: MIT
#include <elos/event/event.h>

#include "elosStorageBackendAccepts_utest.h"

int elosTestElosStorageBackendAcceptsErrInvalidParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageBackendAcceptsErrInvalidParameterTeardown(UNUSED void **state) {
    return 0;
}

void _testelosStorageBackendAcceptsErrInvalidParameterParam(const char *parameterUnderTest,
                                                            const elosStorageBackend_t *backend,
                                                            const elosEvent_t *event) {
    PARAM("%s", parameterUnderTest);
    safuResultE_t result = elosStorageBackendAccepts(backend, event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

void elosTestElosStorageBackendAcceptsErrInvalidParameter(UNUSED void **state) {
    TEST("elosStorageBackendAccepts");
    SHOULD("%s", "Test if invalid parameter are processed correctly");

    elosEvent_t event = {0};
    _testelosStorageBackendAcceptsErrInvalidParameterParam("backend is NULL", NULL, &event);

    elosStorageBackend_t backend;
    _testelosStorageBackendAcceptsErrInvalidParameterParam("event is NULL", &backend, NULL);
}
