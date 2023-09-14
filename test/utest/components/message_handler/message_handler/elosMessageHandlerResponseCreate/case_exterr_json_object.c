// SPDX-License-Identifier: MIT

#include "elosMessageHandlerResponseCreate_utest.h"

void elosTestElosMessageHandlerResponseCreateExterrJsonObject(UNUSED void **state) {
    const char mockErrString[] = "example error text";
    json_object *response = NULL;

    TEST("elosMessageHandlerResponseCreate");
    SHOULD("%s", "return NULL when creating a new json object fails");

    will_return(__wrap_json_object_new_object, NULL);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    response = elosMessageHandlerResponseCreate(mockErrString);
    assert_null(response);
}
