// SPDX-License-Identifier: MIT

#include "elosMessageHandlerResponseCreate_utest.h"

void elosTestElosMessageHandlerResponseCreateSuccessNullString(UNUSED void **state) {
    const char expectedResponse[] = "{ \"error\": null }";
    json_object *response = NULL;

    TEST("elosMessageHandlerResponseCreate");
    SHOULD("%s", "return successfully a new response");

    json_object *mockJobj = __real_json_object_new_object();
    assert_non_null(mockJobj);

    will_return(__wrap_json_object_new_object, mockJobj);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    response = elosMessageHandlerResponseCreate(NULL);
    const char *responseString = json_object_to_json_string(response);
    assert_string_equal(expectedResponse, responseString);

    json_object_put(mockJobj);
}
