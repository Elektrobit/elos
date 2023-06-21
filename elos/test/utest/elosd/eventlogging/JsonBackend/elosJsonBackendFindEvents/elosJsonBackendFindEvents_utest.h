// SPDX-License-Identifier: MIT
#ifndef __ELOS_JSONBACKENDFINDEVENTS_UTEST_H__
#define __ELOS_JSONBACKENDFINDEVENTS_UTEST_H__

#include <JsonBackend.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <elos/eventfilter/eventfilter.h>
#include <safu/vector.h>

#define NUM_FILTER 3

typedef struct {
    elosStorageBackend_t *backend;
    size_t eventStrLen;
    char *eventJsonString;
    safuVec_t *eventVector;
    const char **filterRules;
    elosEventFilter_t *filter;
} elosUteststateT_t;

void elosJsonBackendFindEventsUtestCreateEvent(void **state);
void elosJsonBackendFindEventsUtestCreateFilter(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosJsonBackendFindEventsSuccess)

#endif /* __ELOS_JSONBACKENDFINDEVENTS_UTEST_H__ */
