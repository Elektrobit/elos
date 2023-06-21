// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/json.h>

int safuJsonGetInt64(const struct json_object *jobj, const char *name, size_t idx, int64_t *val) {
    check_expected_ptr(jobj);
    check_expected_ptr(name);
    check_expected(idx);
    check_expected_ptr(val);
    int ret = mock_type(int);
    if (!ret) {
        *val = mock_type(int64_t);
    }
    return ret;
}
