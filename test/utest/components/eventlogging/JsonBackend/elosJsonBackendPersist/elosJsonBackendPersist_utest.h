// SPDX-License-Identifier: MIT
#ifndef __ELOS_JSONBACKENDPERSIST_UTEST_H__
#define __ELOS_JSONBACKENDPERSIST_UTEST_H__

#include <JsonBackend.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <elos/event/event.h>
#include <elos/libelosplugin/StorageBackend_types.h>
#include <safu/common.h>
#include <safu/mock_safu.h>
#include <safu/time.h>
#include <stdlib.h>

#define WRITE_FD 10
#define READ_FD  11

typedef struct elosTestState {
    elosStorageBackend_t *backend;
    char *expectedString;
    char *filePath;
    elosEvent_t event;
    elosEventSource_t eventSource;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosJsonBackendPersistExterrEventSerialize)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosJsonBackendPersistExterrWriteExactly)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosJsonBackendPersistSuccess)

#endif /* __ELOS_JSONBACKENDPERSIST_UTEST_H__ */
