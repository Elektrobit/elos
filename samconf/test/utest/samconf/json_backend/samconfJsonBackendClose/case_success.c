// SPDX-License-Identifier: MIT

#include <samconf/json_backend.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "cmocka_mocks/mock_libc.h"
#include "samconfJsonBackendClose_utest.h"

int samconfTestSamconfJsonBackendCloseSuccessSetup(void **state) {
    json_object *jobjDummy = malloc(sizeof(json_object *));
    if (jobjDummy == NULL) {
        return -1;
    }
    *state = (void *)jobjDummy;
    return 0;
}

int samconfTestSamconfJsonBackendCloseSuccessTeardown(void **state) {
    __real_free(*state);
    return 0;
}

void samconfTestSamconfJsonBackendCloseSuccess(void **state) {
    json_object *jobjDummy = (json_object *)*state;
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    char *testPath = "test.json";
    samconfConfigBackend_t backend = {
        .originalHandle = testPath,
        .backendHandle = jobjDummy,
    };

    MOCK_FUNC_AFTER_CALL(json_object_put, 0);
    MOCK_FUNC_AFTER_CALL(free, 0);
    expect_value(__wrap_json_object_put, obj, jobjDummy);
    expect_value(__wrap_free, ptr, testPath);

    will_return(__wrap_json_object_put, 1);

    result = samconfJsonBackendClose(&backend);

    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
