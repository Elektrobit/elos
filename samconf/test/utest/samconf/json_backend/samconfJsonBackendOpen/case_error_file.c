// SPDX-License-Identifier: MIT

#include <samconf/json_backend.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "safu/mock_log.h"
#include "samconfJsonBackendOpen_utest.h"

int samconfTestSamconfJsonBackendOpenFileErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfJsonBackendOpenFileErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfJsonBackendOpenFileError(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    char *path = "test.json";

    samconfConfigBackend_t backend = {
        .originalHandle = path,
        .backendHandle = NULL,
    };

    TEST("samconfJsonBackendOpen File Input with error");
    SHOULD("%s", "Return Error Status and log error message");

    MOCK_FUNC_AFTER_CALL(json_object_from_file, 0);
    expect_not_value(__wrap_json_object_from_file, filename, NULL);
    will_return(__wrap_json_object_from_file, NULL);

    result = samconfJsonBackendOpen(path, &backend);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_string_equal(backend.originalHandle, path);
    assert_null(backend.backendHandle);
}
