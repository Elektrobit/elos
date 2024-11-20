// SPDX-License-Identifier: MIT

#include "read_utest.h"

int elosTestReadErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestReadErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestReadError(UNUSED void **state) {
    using namespace elos;
    Elos testObject(MOCK_IP_ADDR, MOCK_PORT);
    Subscription mockSubscription;

    TEST("read error");
    SHOULD("%s", "should return empty vector if reading from an empty queue");

    MOCK_FUNC_AFTER_CALL(elosConnectSessionTcpip, 0);
    expect_string(elosConnectSessionTcpip, host, MOCK_IP_ADDR);
    expect_value(elosConnectSessionTcpip, port, MOCK_PORT);
    expect_any(elosConnectSessionTcpip, session);
    will_return(elosConnectSessionTcpip, ELOS_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventQueueRead, 0);
    expect_any(elosEventQueueRead, session);
    expect_any(elosEventQueueRead, eventQueueId);
    expect_any(elosEventQueueRead, eventVector);
    will_set_parameter(elosEventQueueRead, eventVector, NULL);
    will_return(elosEventQueueRead, ELOS_RESULT_FAILED);

    auto events = testObject.read(mockSubscription);

    assert_true(events.empty());
}
