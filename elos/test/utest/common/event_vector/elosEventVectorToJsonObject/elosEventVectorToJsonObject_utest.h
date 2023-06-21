// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_VECTOR_TO_JSON_OBJECT_UTEST_H
#define ELOS_EVENT_VECTOR_TO_JSON_OBJECT_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>

#include "elos/event/event_vector.h"

typedef struct elosUteststateT {
    elosEventVector_t *eventVector;
} elosUteststateT_t;

void elosEventVectorToJsonObjectUtestCreateEventVector(void **state);
void elosEventVectorToJsonObjectUtestDeleteEventVector(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorToJsonObjectErrEventVector)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorToJsonObjectErrEventVectorJobj)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorToJsonObjectExtErrNewJobj)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorToJsonObjectExtErrAddNewArray)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorToJsonObjectSuccess)

#endif /* ELOS_EVENT_VECTOR_TO_JSON_OBJECT_UTEST_H */
