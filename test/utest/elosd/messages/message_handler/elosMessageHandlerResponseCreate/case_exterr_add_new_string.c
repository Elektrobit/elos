// SPDX-License-Identifier: MIT

#include "elosMessageHandlerResponseCreate_utest.h"

void elosTestElosMessageHandlerResponseCreateExterrAddNewString(UNUSED void **state) {
    const char mockErrString[] = "example error text";
    json_object *response = NULL;

    TEST("elosMessageHandlerResponseCreate");
    SHOULD("%s", "return NULL when safuJsonAddNewString fails");

    json_object *mockJobj = __real_json_object_new_object();
    assert_non_null(mockJobj);

    will_return(__wrap_json_object_new_object, mockJobj);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    expect_value(__wrap_safuJsonAddNewString, jobj, mockJobj);
    expect_string(__wrap_safuJsonAddNewString, name, "error");
    expect_value(__wrap_safuJsonAddNewString, val, mockErrString);
    will_return(__wrap_safuJsonAddNewString, NULL);
    MOCK_FUNC_AFTER_CALL(safuJsonAddNewString, 0);

    response = elosMessageHandlerResponseCreate(mockErrString);
    assert_null(response);
}
