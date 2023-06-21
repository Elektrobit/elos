// SPDX-License-Identifier: MIT

#include <samconf/json_backend.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "cmocka_mocks/mock_libc.h"
#include "samconfJsonBackendOpen_utest.h"

int samconfTestSamconfJsonBackendOpenFileSuccessSetup(UNUSED void **state) {
    json_object *jobjDummy = malloc(sizeof(json_object *));
    *state = jobjDummy;
    return 0;
}

int samconfTestSamconfJsonBackendOpenFileSuccessTeardown(UNUSED void **state) {
    __real_free(*state);
    return 0;
}

void samconfTestSamconfJsonBackendOpenFileSuccess(UNUSED void **state) {
    char path[] = "test.json";
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfigBackend_t backend = {
        .originalHandle = path,
        .backendHandle = NULL,
    };

    TEST("samconfJsonBackendOpen with File input");
    SHOULD("%s", "return expected json_object inside struct");

    MOCK_FUNC_AFTER_CALL(json_object_from_file, 0);
    expect_string(__wrap_json_object_from_file, filename, path);
    will_return(__wrap_json_object_from_file, *state);

    result = samconfJsonBackendOpen(path, &backend);

    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(backend.originalHandle, path);
}
