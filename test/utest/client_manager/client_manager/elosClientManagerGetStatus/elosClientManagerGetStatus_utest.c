// SPDX-License-Identifier: MIT
#include "elosClientManagerGetStatus_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientManagerGetStatusUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosClientManagerGetStatusExtErrMutexLock),
        TEST_CASE(elosTestElosClientManagerGetStatusExtErrMutexUnlock),
        TEST_CASE(elosTestElosClientManagerGetStatusSuccess),
    };
    return RUN_TEST_SUITE(tests, elosClientManagerGetStatusUtest);
}

static int elosClientManagerGetStatusUtestSetup(void **state) {
    elosClientManagerContext_t *context = safuAllocMem(NULL, sizeof(elosClientManagerContext_t));
    int retval = context == NULL ? -1 : 0;
    assert_int_equal(retval, 0);
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status = MOCK_FLAGS;
    retval = pthread_mutex_init(&context->lock, NULL);
    assert_int_equal(retval, 0);
    *state = context;
    return 0;
}

static int elosClientManagerGetStatusUtestTeardown(void **state) {
    elosClientManagerContext_t *context = *state;
    free(context);
    return 0;
}
