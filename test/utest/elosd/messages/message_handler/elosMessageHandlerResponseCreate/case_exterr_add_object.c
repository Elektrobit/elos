// SPDX-License-Identifier: MIT

#include "elosMessageHandlerResponseCreate_utest.h"

void elosTestElosMessageHandlerResponseCreateExterrAddObject(UNUSED void **state) {
    json_object *response = NULL;

    TEST("elosMessageHandlerResponseCreate");
    SHOULD("%s", "return NULL when safuJsonAddObject fails");

    json_object *mockJobj = __real_json_object_new_object();
    assert_non_null(mockJobj);

    will_return(__wrap_json_object_new_object, mockJobj);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    expect_value(__wrap_safuJsonAddObject, jobj, mockJobj);
    expect_string(__wrap_safuJsonAddObject, name, "error");
    expect_value(__wrap_safuJsonAddObject, jdata, NULL);
    will_return(__wrap_safuJsonAddObject, -1);
    MOCK_FUNC_AFTER_CALL(safuJsonAddObject, 0);

    response = elosMessageHandlerResponseCreate(NULL);
    assert_null(response);
}
