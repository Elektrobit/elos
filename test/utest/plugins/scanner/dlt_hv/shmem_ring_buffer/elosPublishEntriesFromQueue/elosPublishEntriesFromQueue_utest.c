// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>
#include <safu/ringbuffer_types.h>
#include <safu/vector.h>
#include <samconf/json_backend.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <samconf/test_utils.h>

#include "dlt_hv/scanner.h"
#include "dlt_hv/types.h"
#include "elos/event/event.h"
#include "elos/libelosdlt/dltmapper.h"
#include "elosPublishEntriesFromQueue_utets.h"

TEST_SUITE_FUNC_PROTOTYPES(elosPublishEntriesFromQueueUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosPublishEntriesFromQueueSuccess),
    };

    return RUN_TEST_SUITE(tests, elosPublishEntriesFromQueueUtest);
}

safuResultE_t elosDeleteEbLogEntry(void *entry) {
    return elosFreeEbLogEntry((elosEbLogEntry_t *)entry);
}

static inline safuResultE_t elosTestPublishEvent(struct elosPublisher *publisher, const elosEvent_t *event) {
    check_expected_ptr(publisher);
    check_expected_ptr(event);
    return mock_type(safuResultE_t);
}
static inline safuResultE_t elosTestStoreEvent(UNUSED struct elosPluginControl *instanceRef, const elosEvent_t *event) {
    check_expected_ptr(event);
    return mock_type(safuResultE_t);
}

int elosPublishEntriesFromQueueUtestSetup(void **state) {
    struct elosTestData *testData = malloc(sizeof(struct elosTestData));

    safuRingBufferParam_t param = {
        .deleteEntries = true,
        .elements = BUFF_SIZE,
        .deleteFunc = elosDeleteEbLogEntry,
    };
    safuRingBufferNew(&testData->readQueue, &param);
    testData->plugin = malloc(sizeof(elosPlugin_t));
    testData->plugin->store = elosTestStoreEvent;
    testData->plugin->publish = elosTestPublishEvent;
    elosDltScanner_t *dlt = malloc(sizeof(elosDltScanner_t));
    elosDltMapperInit(&dlt->mapper, "DLT_FILE", "DLT-APP-ID", "DLT-HARDWARE-ID", 0);
    testData->plugin->data = dlt;
    const char *jstr =
        "{"
        "	\"File\": \"scanner_dlt_hv.so\","
        "	\"Run\": \"always\","
        "	\"Config\": {"
        "		\"DeviceFile\": \"/dev/shm/test_dlt_shmem\","
        "		\"OffsetAddress\": \"0x1000\","
        "		\"BufferSize\": \"1448\""
        "	}"
        "}";
    samconfConfig_t *mockConfig = NULL;
    samconfConfigNew(&mockConfig);
    samconfUtilCreateMockConfigFromStr(jstr, false, mockConfig);
    mockConfig->key = strdup("DltHv");
    testData->plugin->config = mockConfig;

    *state = testData;
    return 0;
}
int elosPublishEntriesFromQueueUtestTeardown(void **state) {
    struct elosTestData *testData = *state;

    safuRingBufferDelete(&testData->readQueue);
    testData->readQueue = NULL;
    elosDltScanner_t *dlt = testData->plugin->data;
    elosDltMapperDeleteMembers(&dlt->mapper);
    free(dlt);
    samconfConfigDelete((samconfConfig_t *)testData->plugin->config);
    free(testData->plugin);
    free(testData);
    testData = NULL;
    return 0;
}
