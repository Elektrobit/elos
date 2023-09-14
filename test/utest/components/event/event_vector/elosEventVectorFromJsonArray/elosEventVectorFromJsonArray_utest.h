// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_VECTOR_FROM_JSON_ARRAY_UTEST_H
#define ELOS_EVENT_VECTOR_FROM_JSON_ARRAY_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>

#include "elos/event/event_vector.h"

typedef struct elosUnitTestState {
    elosEventVector_t *eventVector;
    json_object *jEventArray;
    json_object *jEventArrayEmpty;
    json_object *jEventArrayInvalid;
} elosUnitTestState_t;

void elosEventVectorFromJsonArrayUtestCreateEventVector(void **state);
void elosEventVectorFromJsonArrayUtestDeleteEventVector(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonArrayErrArray)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonArrayErrVector)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonArrayErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorFromJsonArraySuccess)

#endif /* ELOS_EVENT_VECTOR_FROM_JSON_ARRAY_UTEST_H */
