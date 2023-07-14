// SPDX-License-Identifier: MIT

#include "elosMessageGetVersion_utest.h"

extern int elosMessageGetVersion(elosClientManagerConnection_t *conn, UNUSED elosMessage_t const *const msg);

int elosTestElosMessageGetVersionExtErrNewObjectSetup(void **state) {
    elosClientManagerConnection_t *connection = elosMessageGetVersionCreateConnection();
    *state = connection;
    return 0;
}

int elosTestElosMessageGetVersionExtErrNewObjectTeardown(void **state) {
    elosClientManagerConnection_t *connection = *state;
    free(connection->sharedData);
    free(connection);
    return 0;
}

void elosTestElosMessageGetVersionExtErrNewObject(void **state) {
    elosClientManagerConnection_t *connection = *state;
    int ret;

    TEST("elosMessageGetVersion");
    SHOULD("%s", "return -1 when json_object_new_object fails");

    expect_value(elosMessageHandlerResponseCreate, errstr, NULL);
    will_return(elosMessageHandlerResponseCreate, NULL);
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerResponseCreate, 0);

    ret = elosMessageGetVersion(connection, NULL);
    assert_int_equal(ret, -1);
}
