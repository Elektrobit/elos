// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_VECTOR_TO_JSON_ARRAY_UTEST_H
#define ELOS_EVENT_VECTOR_TO_JSON_ARRAY_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>

#include "elos/event/event_vector.h"

typedef struct elosUnitTestState {
    elosEventVector_t *eventVector;
    json_object *jEventArray;
} elosUnitTestState_t;

void elosEventVectorSerializeUtestCreateEventVector(void **state);
void elosEventVectorSerializeUtestDeleteEventVector(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorSerializeErrConversion)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorSerializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorSerializeSuccess)

#endif /* ELOS_EVENT_VECTOR_TO_JSON_ARRAY_UTEST_H */
