// SPDX-License-Identifier: MIT
#ifndef __ELOS_LOGAGGREGATORFINDEVENTS_UTEST_H__
#define __ELOS_LOGAGGREGATORFINDEVENTS_UTEST_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <elos/eventlogging/LogAggregator.h>
#include <safu/vector.h>

#define NUM_FILTER 3

typedef struct {
    elosLogAggregator_t logAggregator;
    safuVec_t eventVector;
    char *eventJsonString;
    const char **filterRules;
} elosUteststateT_t;

void elosLogAggregatorFindEventsUtestCreateEvent(void **state);
void elosLogAggregatorFindEventsUtestFreeEvent(void **state);

void elosLogAggregatorFindEventsUtestCreateFilterRules(void **state);
void elosLogAggregatorFindEventsUtestFreeFilterRules(void **state);

void elosLogAggregatorFindEventsUtestCreateLogAggregator(void **state);
void elosLogAggregatorFindEventsUtestFreeLogAggregator(void **state);

void elosLogAggregatorFindEventsUtestCreateVector(void **state);
void elosLogAggregatorFindEventsUtestFreeVector(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogAggregatorFindEventsSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogAggregatorFindEventsErrParamLogAggregatorNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogAggregatorFindEventsErrParamRuleNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogAggregatorFindEventsErrParamEventsNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogAggregatorFindEventsExterrEventFilterCreate)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLogAggregatorFindEventsExterrMutex)

#endif /* __ELOS_LOGAGGREGATORFINDEVENTS_UTEST_H__ */
