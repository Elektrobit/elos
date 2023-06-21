// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_VECTOR_TO_JSON_OBJECT_UTEST_H
#define ELOS_EVENT_VECTOR_TO_JSON_OBJECT_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>

#include "elos/event/event_vector.h"

#define NUM_TEST_EVENTS 1

typedef struct elosUteststateT {
    json_object *eventVecJobj;
    elosEventVector_t *expectedEventVector;
} elosUteststateT_t;

void elosEventVectorFromJsonObjectUtestCreateEventVectorJobj(void **state);
void elosEventVectorFromJsonObjectUtestDeleteEventVectorJobj(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonObjectErrEventVectorJobj)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonObjectErrEventVector)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonObjectExtErrGetArray)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonObjectExtErrNewVector)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonObjectExtErrEventFromJsonObject)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonObjectSuccess)

#endif /* ELOS_EVENT_VECTOR_TO_JSON_OBJECT_UTEST_H */
