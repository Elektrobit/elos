// SPDX-License-Identifier: MIT

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <safu/json.h>

#include "elosMessageGetVersion_utest.h"
#include "version.h"

extern int elosMessageGetVersion(elosClientConnection_t *conn, UNUSED elosMessage_t const *const msg);

int elosTestElosMessageGetVersionSuccessSetup(void **state) {
    elosClientConnection_t *conn = elosMessageGetVersionCreateConnection();
    *state = conn;
    return 0;
}

int elosTestElosMessageGetVersionSuccessTeardown(void **state) {
    elosClientConnection_t *conn = *(elosClientConnection_t **)state;
    free(conn->sharedData);
    free(conn);
    return 0;
}

static int _check_version(const long unsigned int json, const long unsigned int version) {
    json_object *response = (json_object *)json;
    const char *expectedVersion = (const char *)version;
    const char *versionString = NULL;
    if (safuJsonGetString(response, "version", 0, &versionString) == -1) {
        print_error("JSON Object has no version key!\n");
        return 0;
    }
    if (strcmp(versionString, expectedVersion) == 0) {
        return 1;
    }
    print_error("JSON Object[\"version\"] = \"%s\" != \"%s\"\n", versionString, expectedVersion);
    return 0;
}

void elosTestElosMessageGetVersionSuccess(void **state) {
    elosClientConnection_t *conn = *state;
    int ret;

    TEST("elosMessageGetVersion");
    SHOULD("%s", "receive version string successfully");

    expect_value(elosMessageHandlerSendJson, conn, conn);
    expect_value(elosMessageHandlerSendJson, messageId, ELOS_MESSAGE_RESPONSE_GET_VERSION);
    expect_check(elosMessageHandlerSendJson, jobj, _check_version, elosGetVersionString());
    will_return(elosMessageHandlerSendJson, SAFU_RESULT_OK);
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSendJson, 0);

    ret = elosMessageGetVersion(conn, NULL);
    assert_int_equal(ret, 0);
}
