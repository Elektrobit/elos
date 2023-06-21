// SPDX-License-Identifier: MIT
#include "elosMessageGetVersion_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosMessageGetVersionUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosMessageGetVersionExtErrNewObject),
                                       TEST_CASE(elosTestElosMessageGetVersionExtErrAddNewString),
                                       TEST_CASE(elosTestElosMessageGetVersionSuccess)};

    return RUN_TEST_SUITE(tests, elosMessageGetVersionUtest);
}

elosClientManagerConnection_t *elosMessageGetVersionCreateConnection() {
    elosClientManagerSharedData_t *mockSharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    assert_non_null(mockSharedData);

    elosClientManagerConnection_t *connection = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    assert_non_null(connection);
    connection->sharedData = mockSharedData;

    return connection;
}

static int elosMessageGetVersionUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosMessageGetVersionUtestTeardown(UNUSED void **state) {
    return 0;
}
