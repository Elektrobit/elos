// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/json.h>

int safuJsonGetUint64(const struct json_object *jobj, const char *name, size_t idx, uint64_t *val) {
    check_expected_ptr(jobj);
    check_expected_ptr(name);
    check_expected(idx);
    *val = mock_type(uint64_t);
    return mock_type(int);
}
