// SPDX-License-Identifier: MIT
#include <cmocka_extensions/mock_extensions.h>

#include "unsubscribe_utest.h"

namespace elos {
class TestSubscription : public Subscription {
   public:
    TestSubscription(Subscription &testSubscription) : Subscription(testSubscription) {}
    ~TestSubscription() = default;
    uint32_t getQueueId() {
        return subscription.eventQueueId;
    }
};
}  // namespace elos

int elosTestUnsubscribeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestUnsubscribeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestUnsubscribeSuccess(UNUSED void **state) {
    using namespace elos;

    elosResultE result;
    Elos testObject;
    std::string testFilter = ".e.messageCode 5005 EQ";

    TEST("unsubscribe");
    SHOULD("%s", "Successfully unsubscribe from an event queue");

    MOCK_FUNC_AFTER_CALL(elosEventSubscribe, 0);
    expect_any(elosEventSubscribe, session);
    expect_any(elosEventSubscribe, filterRuleArray);
    expect_any(elosEventSubscribe, filterRuleArraySize);
    expect_any(elosEventSubscribe, eventQueueId);
    will_return(elosEventSubscribe, 42);
    will_return(elosEventSubscribe, ELOS_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosEventUnsubscribe, 0);
    expect_any(elosEventUnsubscribe, session);
    expect_value(elosEventUnsubscribe, eventQueueId, 42);
    will_return(elosEventUnsubscribe, ELOS_RESULT_OK);

    Subscription returnedSubscription = testObject.subscribe(testFilter);
    TestSubscription testSubscription(returnedSubscription);
    assert_int_equal(testSubscription.getQueueId(), 42);

    result = testObject.unsubscribe(returnedSubscription);
    assert_int_equal(result, ELOS_RESULT_OK);
}
