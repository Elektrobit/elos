// SPDX-License-Identifier: MIT

#include "elosMessageHandlerResponseCreate_utest.h"

void elosTestElosMessageHandlerResponseCreateSuccessErrString(UNUSED void **state) {
    const char mockErrString[] = "example error text";
    const char expectedResponse[] = "{ \"error\": \"example error text\" }";
    json_object *response = NULL;

    TEST("elosMessageHandlerResponseCreate");
    SHOULD("%s", "return successfully the response string");

    json_object *mockJobj = __real_json_object_new_object();
    assert_non_null(mockJobj);

    will_return(__wrap_json_object_new_object, mockJobj);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    response = elosMessageHandlerResponseCreate(mockErrString);
    const char *responseString = json_object_to_json_string(response);
    assert_string_equal(expectedResponse, responseString);

    json_object_put(mockJobj);
}
