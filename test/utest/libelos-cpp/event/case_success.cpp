// SPDX-License-Identifier: MIT
#include <cstdint>
#include <memory>

#include "event_utest.h"

int elosTestEventSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestEventSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEventSuccess(UNUSED void **state) {
    using namespace elos;

    TEST("event");
    SHOULD("%s", "create successfully an event instance");

    PARAM("Default Constructor");
    Event defC;

    PARAM("Parameterize Constructor");
    char mockName[] = MOCK_NAME;
    elosEventSource_t source{mockName, mockName, MOCK_PID};
    struct timespec date {
        MOCK_TIME, MOCK_TIME
    };

    Event paramC(date, source, MOCK_SEVERITY, MOCK_ID, MOCK_CLASSIF, MOCK_MSG_CODE, MOCK_PAYLOAD);
    assert_int_equal(paramC.event.date.tv_sec, MOCK_TIME);
    assert_int_equal(paramC.event.date.tv_nsec, MOCK_TIME);
    assert_string_equal(paramC.event.source.appName, MOCK_NAME);
    assert_string_equal(paramC.event.source.fileName, MOCK_NAME);
    assert_int_equal(paramC.event.source.pid, MOCK_PID);
    assert_int_equal(paramC.event.severity, MOCK_SEVERITY);
    assert_int_equal(paramC.event.messageCode, MOCK_MSG_CODE);
    assert_int_equal(paramC.event.classification, MOCK_CLASSIF);
    assert_string_equal(paramC.event.hardwareid, MOCK_ID);
    assert_string_equal(paramC.event.payload, MOCK_PAYLOAD);

    PARAM("Default Destructor");
    try {
        auto defD = std::make_unique<Event>();
    } catch (const std::bad_alloc &e) {
        assert_true(false);
    } catch (safuResultE_t &ex) {
        assert_true(ex == SAFU_RESULT_OK);
    } catch (...) {
        assert_true(false);
    }
}
