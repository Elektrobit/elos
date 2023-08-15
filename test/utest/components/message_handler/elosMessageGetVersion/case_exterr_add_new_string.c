// SPDX-License-Identifier: MIT

#include "elosMessageGetVersion_utest.h"
#include "safu/mock_safu.h"
#include "version.h"

extern int elosMessageGetVersion(elosClientConnection_t *conn, UNUSED elosMessage_t const *const msg);

int elosTestElosMessageGetVersionExtErrAddNewStringSetup(void **state) {
    elosClientConnection_t *connection = elosMessageGetVersionCreateConnection();
    *state = connection;
    return 0;
}

int elosTestElosMessageGetVersionExtErrAddNewStringTeardown(void **state) {
    elosClientConnection_t *connection = *state;
    free(connection->sharedData);
    free(connection);
    return 0;
}

void elosTestElosMessageGetVersionExtErrAddNewString(void **state) {
    elosClientConnection_t *connection = *state;
    int ret;

    TEST("elosMessageGetVersion");
    SHOULD("%s", "return -1 when safuJsonAddNewString fails");

    // should not be a NULL pointer but a json object
    expect_not_value(__wrap_safuJsonAddNewString, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewString, name, "version");
    expect_string(__wrap_safuJsonAddNewString, val, elosGetVersionString());
    will_return(__wrap_safuJsonAddNewString, NULL);
    MOCK_FUNC_AFTER_CALL(safuJsonAddNewString, 0);

    ret = elosMessageGetVersion(connection, NULL);
    assert_int_equal(ret, -1);
}
