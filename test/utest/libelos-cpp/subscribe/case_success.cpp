// SPDX-License-Identifier: MIT
#include <cmocka_extensions/mock_extensions.h>

#include "subscribe_utest.h"

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

int elosTestSubscribeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestSubscribeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestSubscribeSuccess(UNUSED void **state) {
    using namespace elos;
    Elos testObject;
    std::string testFilter = ".e.messageCode 5005 EQ";

    TEST("subscribe");
    SHOULD("%s", "Create a subscription message, send it, evaluate the response and return list of message queue ids");

    MOCK_FUNC_AFTER_CALL(elosEventSubscribe, 0);
    expect_any(elosEventSubscribe, session);
    expect_any(elosEventSubscribe, filterRuleArray);
    expect_any(elosEventSubscribe, filterRuleArraySize);
    expect_any(elosEventSubscribe, eventQueueId);
    will_return(elosEventSubscribe, 0);
    will_return(elosEventSubscribe, ELOS_RESULT_OK);

    Subscription returnedSubscription = testObject.subscribe(testFilter);
    TestSubscription testSubscription(returnedSubscription);

    assert_int_equal(testSubscription.getQueueId(), 0);
}
