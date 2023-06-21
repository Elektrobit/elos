// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/json.h>

int safuJsonAddObject(struct json_object *jobj, const char *name, struct json_object *jdata) {
    check_expected_ptr(jobj);
    check_expected_ptr(name);
    check_expected_ptr(jdata);
    return mock_type(int);
}
