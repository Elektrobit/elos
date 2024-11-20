// SPDX-License-Identifier: MIT
#include <memory>

#include "read_utest.h"

int elosTestReadSuccessSetup(void **state) {
    elosEventVector_t *mockEventVector = nullptr;
    elosEventVectorNew(&mockEventVector, 2);

    elosEvent_t event0;
    elosEvent_t event1;
    event0.source.appName = strdup(MOCK_NAME);
    event0.payload = strdup(MOCK_NAME);
    event0.source.fileName = strdup(MOCK_NAME);
    event0.hardwareid = strdup(MOCK_ID);
    event1.source.appName = strdup(MOCK_NAME);
    event1.payload = strdup(MOCK_NAME);
    event1.source.fileName = strdup(MOCK_NAME);
    event1.hardwareid = strdup(MOCK_ID);
    event1.severity = ELOS_SEVERITY_FATAL;

    elosEventVectorPush(mockEventVector, &event0);
    elosEventVectorPush(mockEventVector, &event1);

    *state = mockEventVector;
    return 0;
}

int elosTestReadSuccessTeardown(void **state) {
    auto mockEventVector = reinterpret_cast<elosEventVector_t *>(*state);
    if (mockEventVector != nullptr) {
        if (mockEventVector->data != nullptr) {
            free(mockEventVector->data);
        }
        free(mockEventVector);
    }
    return 0;
}

void elosTestReadSuccess(void **state) {
    using namespace elos;
    Elos testObject(MOCK_IP_ADDR, MOCK_PORT);
    auto mockEventVector = reinterpret_cast<elosEventVector_t *>(*state);
    Subscription mockSubscription;

    TEST("read success");
    SHOULD("%s", "read events from elos queue");

    MOCK_FUNC_AFTER_CALL(elosConnectSessionTcpip, 0);
    expect_string(elosConnectSessionTcpip, host, MOCK_IP_ADDR);
    expect_value(elosConnectSessionTcpip, port, MOCK_PORT);
    expect_any(elosConnectSessionTcpip, session);
    will_return(elosConnectSessionTcpip, ELOS_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventQueueRead, 0);
    expect_any(elosEventQueueRead, session);
    expect_any(elosEventQueueRead, eventQueueId);
    expect_any(elosEventQueueRead, eventVector);
    will_set_parameter(elosEventQueueRead, eventVector, mockEventVector);
    will_return(elosEventQueueRead, SAFU_RESULT_OK);

    auto events = std::make_unique<std::vector<elos::Event>>(testObject.read(mockSubscription));

    assert_string_equal(events->at(0).event.payload, MOCK_NAME);
    assert_string_equal(events->at(0).event.source.appName, MOCK_NAME);
    assert_string_equal(events->at(0).event.source.fileName, MOCK_NAME);
    assert_string_equal(events->at(1).event.hardwareid, MOCK_ID);
    assert_int_equal(events->at(1).event.severity, MOCK_SEVERITY);

    *state = nullptr;
}
