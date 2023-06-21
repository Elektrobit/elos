// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_extensions/cmocka_extensions.h"
#include "cmocka_mocks/mock_libc.h"
#include "elos/event/event.h"
#include "elos/event/event_source.h"
#include "elosLoglineMapperDoMapping_utest.h"
#include "logline_mapper.h"
#include "safu/common.h"
#include "safu/mock_safu.h"

static const char *elosElosLogline[] = {
    "<38>Jan  1 00:00:04 sshd[98]: Server listening on 0.0.0.0 port 22.",
    "<38>Jan  1 00:00:04 sshd[98]: Server listening on :: port 22.",
    "<85>Jan  1 00:02:28 login[82]: ROOT LOGIN  on '/dev/console'",
    "<13>Jan  1 01:41:57 root: hehyoi",
    "<13>May 23 10:42:48 root: hehyoi",
    "test syslog daemon",
};

// temporary solution: ld.so mock
// NOLINTNEXTLINE
safuResultE_t safuTimeGetLocalTime(struct tm *localTime) {
    check_expected_ptr(localTime);
    localTime->tm_sec = mock_type(int);
    localTime->tm_min = mock_type(int);
    localTime->tm_hour = mock_type(int);
    localTime->tm_mday = mock_type(int);
    localTime->tm_mon = mock_type(int);
    localTime->tm_year = mock_type(int);
    localTime->tm_wday = mock_type(int);
    localTime->tm_yday = mock_type(int);
    localTime->tm_isdst = mock_type(int);
    return mock_type(safuResultE_t);
}

static const elosEvent_t elosExpectedEvent[] = {
    {.date = {.tv_sec = 1640995204, .tv_nsec = 0},
     .severity = ELOS_SEVERITY_INFO,
     .classification = ELOS_CLASSIFICATION_SECURITY,
     .messageCode = 4000,
     .payload = ": Server listening on 0.0.0.0 port 22.",
     .source = {.appName = "sshd", .fileName = NULL, .pid = 98}},
    {.date = {.tv_sec = 1640995204, .tv_nsec = 0},
     .severity = ELOS_SEVERITY_INFO,
     .classification = ELOS_CLASSIFICATION_SECURITY,
     .messageCode = 4000,
     .payload = ": Server listening on :: port 22.",
     .source = {.appName = "sshd", .fileName = NULL, .pid = 98}},
    {.date = {.tv_sec = 1640995348, .tv_nsec = 0},
     .severity = ELOS_SEVERITY_INFO,
     .classification = ELOS_CLASSIFICATION_SECURITY,
     .messageCode = 2000,
     .payload = ": ROOT LOGIN  on '/dev/console'",
     .source = {.appName = "login", .fileName = NULL, .pid = 82}},
    {.date = {.tv_sec = 1641001317, .tv_nsec = 0},
     .severity = ELOS_SEVERITY_INFO,
     .classification = ELOS_CLASSIFICATION_UNDEFINED,
     .payload = " hehyoi",
     .messageCode = 1000,
     .source = {.appName = "root", .fileName = NULL, .pid = 0}},
    {.date = {.tv_sec = 1653302568, .tv_nsec = 0},
     .severity = ELOS_SEVERITY_INFO,
     .classification = ELOS_CLASSIFICATION_UNDEFINED,
     .messageCode = 1000,
     .payload = " hehyoi",
     .source = {.appName = "root", .fileName = NULL, .pid = 0}},
    {
        .payload = "test syslog daemon",
    },
};

int elosTestElosLoglineMapperDoMappingSuccessSetup(void **state) {
    elosLoglineMapperDoMappingUtestInit(state);

    elosTestState_t *testState = *state;

    samconfConfig_t *messageCodesChildren[] = {
        &testState->childrenData[2],
        &testState->childrenData[3],
        &testState->childrenData[4],
    };

    testState->childrenData[1].children = messageCodesChildren;
    testState->childrenData[1].childCount = ARRAY_SIZE(messageCodesChildren);

    samconfConfig_t *mappingRulesChildren[] = {
        &testState->childrenData[1],
    };

    testState->childrenData[0].children = mappingRulesChildren;
    testState->childrenData[0].childCount = ARRAY_SIZE(mappingRulesChildren);

    samconfConfig_t *syslogScannerChildren[] = {
        &testState->childrenData[0],
    };

    //  "SyslogScanner": {
    //      "MappingRules": {
    //          "MessageCodes": {
    //              "4000": ...,
    //              "2000": "\": ROOT LOGIN  on '/dev/console'\" .event.payload STRCMP",
    //              "1000": "\" hehyoi\" .event.payload STRCMP"
    //          }
    //      }
    //  }
    samconfConfig_t root = {
        .parent = NULL,
        .key = "SyslogScanner",
        .type = SAMCONF_CONFIG_VALUE_OBJECT,
        .children = syslogScannerChildren,
        .childCount = 1,
    };

    testState->timezone = getenv("TZ");
    setenv("TZ", "UTC", 1);
    tzset();

    safuResultE_t result = elosLoglineMapperInit(&testState->mapper, &root);
    if (result != SAFU_RESULT_OK) {
        return -1;
    }

    return 0;
}

int elosTestElosLoglineMapperDoMappingSuccessTeardown(void **state) {
    elosTestState_t *testState = *state;

    if (testState->timezone == NULL) {
        unsetenv("TZ");
    } else {
        setenv("TZ", testState->timezone, 1);
    }
    tzset();

    elosLoglineMapperDoMappingUtestCleanUp(state);
    return 0;
}

static void _testElosLoglineMapperDoMappingSuccessParam(elosLoglineMapper_t *mapper, const char *logLine,
                                                        const elosEvent_t *expectedEvent) {
    elosEvent_t event = {0};

    PARAM("logline: '%s'\n", logLine);

    struct tm expectedDate = {0};
    localtime_r(&expectedEvent->date.tv_sec, &expectedDate);

    if (*logLine == '<') {
        expect_any(safuTimeGetLocalTime, localTime);
        will_set_parameter(safuTimeGetLocalTime, tm_sec, expectedDate.tm_sec);
        will_set_parameter(safuTimeGetLocalTime, tm_min, expectedDate.tm_min);
        will_set_parameter(safuTimeGetLocalTime, tm_hour, expectedDate.tm_hour);
        will_set_parameter(safuTimeGetLocalTime, tm_mday, expectedDate.tm_mday);
        will_set_parameter(safuTimeGetLocalTime, tm_mon, expectedDate.tm_mon);
        will_set_parameter(safuTimeGetLocalTime, tm_year, expectedDate.tm_year);
        will_set_parameter(safuTimeGetLocalTime, tm_wday, expectedDate.tm_wday);
        will_set_parameter(safuTimeGetLocalTime, tm_yday, expectedDate.tm_yday);
        will_set_parameter(safuTimeGetLocalTime, tm_isdst, expectedDate.tm_isdst);
        will_return(safuTimeGetLocalTime, SAFU_RESULT_OK);
    }

    elosLoglineMapperDoMapping(mapper, &event, logLine);

    assert_int_equal(event.date.tv_sec, expectedEvent->date.tv_sec);
    assert_int_equal(event.date.tv_nsec, expectedEvent->date.tv_nsec);

    assert_int_equal(event.classification, expectedEvent->classification);
    assert_int_equal(event.hardwareid, expectedEvent->hardwareid);
    assert_int_equal(event.messageCode, expectedEvent->messageCode);
    assert_string_equal(event.payload, expectedEvent->payload);
    assert_int_equal(event.severity, expectedEvent->severity);

    if (expectedEvent->source.appName == NULL) {
        assert_ptr_equal(event.source.appName, expectedEvent->source.appName);
    } else {
        assert_string_equal(event.source.appName, expectedEvent->source.appName);
    }
    if (expectedEvent->source.fileName == NULL) {
        assert_ptr_equal(event.source.fileName, expectedEvent->source.fileName);
    } else {
        assert_string_equal(event.source.fileName, expectedEvent->source.fileName);
    }
    assert_int_equal(event.source.pid, expectedEvent->source.pid);

    free(event.payload);
    if (event.source.appName != NULL) {
        free(event.source.appName);
        event.source.appName = NULL;
    }
    if (event.source.fileName != NULL) {
        free(event.source.fileName);
        event.source.fileName = NULL;
    }
}

void elosTestElosLoglineMapperDoMappingSuccess(void **state) {
    elosTestState_t *testState = *state;
    for (size_t i = 0; i < ARRAY_SIZE(elosElosLogline); i++) {
        _testElosLoglineMapperDoMappingSuccessParam(&testState->mapper, elosElosLogline[i], &elosExpectedEvent[i]);
    }
}
