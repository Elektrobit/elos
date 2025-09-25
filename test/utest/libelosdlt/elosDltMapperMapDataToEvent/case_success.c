// SPDX-License-Identifier: MIT

#include <bits/types/struct_iovec.h>
#include <safu/defines.h>

#include "elos/libelosdlt/dltmapper.h"
#include "elosDltMapperMapDataToEvent_utest.h"

int elosTestElosDltMapperMapDataToEventSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosDltMapperMapDataToEventSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosDltMapperMapDataToEventSuccess(UNUSED void **state) {
    safuResultE_t result;
    elosEvent_t testEvent = {0};
    unsigned char testpayload[] = "testpayload";

    elosDltMapper_t testMapper = {.fileName = "testFile",
                                  .appId = "testApp",
                                  .hardwareid = "testhwid",
                                  .pid = 0,
                                  .startTime = {.tv_sec = 1, .tv_nsec = 0}};
    char testParam[][50] = {"with stdHeader timestamp",
                            "without stdHeader timestamp",
                            "info log with appId",
                            "fatal log",
                            "error log",
                            "warn log",
                            "debug log",
                            "verbose log",
                            "app trace message",
                            "network trace message",
                            "control message",
                            "extended header appId is not set"};
    elosDltData_t
        testDltData[] =
            {
                {.stdHeader = {.headerType = {.wtms = true}, .timestamp = 1},
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.wtms = false}},
                 .storageHeader =
                     {
                         .seconds = 1,
                         .microseconds = 1,
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_LOG,
                                         .messageTypeInfo = {.logInfo = ELOS_DLT_LOG_INFO}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_LOG,
                                         .messageTypeInfo = {.logInfo = ELOS_DLT_LOG_FATAL}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_LOG,
                                         .messageTypeInfo = {.logInfo = ELOS_DLT_LOG_ERROR}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_LOG,
                                         .messageTypeInfo = {.logInfo = ELOS_DLT_LOG_WARN}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_LOG,
                                         .messageTypeInfo = {.logInfo = ELOS_DLT_LOG_DEBUG}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_LOG,
                                         .messageTypeInfo = {.logInfo = ELOS_DLT_LOG_VERBOSE}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_APP_TRACE,
                                         .messageTypeInfo = {.traceInfo = ELOS_DLT_TRACE_VARIABLE}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_NW_TRACE,
                                         .messageTypeInfo = {.networkInfo = ELOS_DLT_NW_TRACE_IPC}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .wtms = true}, .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_CONTROL,
                                         .messageTypeInfo = {.controlInfo = ELOS_DLT_CONTROL_REQUEST}},
                         .noOfArguments = 0,
                         .applicationId = {'A', 'P', 'P'},
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
                {.stdHeader = {.headerType = {.ueh = true, .weid = true, .wtms = true},
                               .ecuID = {'C', '0', '0', '2'},
                               .timestamp = 1},
                 .extHeader =
                     {
                         .messageInfo = {.verbose = false,
                                         .messageType = ELOS_DLT_TYPE_LOG,
                                         .messageTypeInfo = {.logInfo = ELOS_DLT_LOG_INFO}},
                         .noOfArguments = 0,
                         .contextId = {'C', 'O', 'N'},
                     },
                 .payload = {.data = testpayload},
                 .dltDataSize = {.dataSize = 11}},
            };

    elosEvent_t expectedEvent[] = {
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "testApp", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_OFF,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_FORWARDED_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 1, .tv_nsec = 1000},
         .source = {.appName = "testApp", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_OFF,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_FORWARDED_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_INFO,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_INFO_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_FATAL,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_FATAL_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_ERROR,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_ERROR_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_WARN,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_WARN_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_DEBUG,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_DEBUG_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_VERBOSE,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_VERBOSE_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_OFF,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_PROCESS,
         .messageCode = ELOS_MSG_CODE_TRACE_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_OFF,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_NETWORK,
         .messageCode = ELOS_MSG_CODE_TRACE_LOG,
         .payload = "testpayload"},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "APP", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_OFF,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_IPC,
         .messageCode = ELOS_MSG_CODE_TRACE_LOG,
         .payload = "dGVzdHBheWxvYWQ="},
        {.date = {.tv_sec = 2, .tv_nsec = 0},
         .source = {.appName = "testApp_C002", .fileName = "testFile", .pid = 0},
         .severity = ELOS_SEVERITY_INFO,
         .hardwareid = "testhwid",
         .classification = ELOS_CLASSIFICATION_LOG,
         .messageCode = ELOS_MSG_CODE_INFO_LOG,
         .payload = "testpayload"},

    };

    TEST("elosDltMapperMapDataToEvent");
    SHOULD("%s", "convert dlt data into elos event successfully");

    for (size_t i = 0; i < ARRAY_SIZE(testDltData); i++) {
        PARAM("%s", testParam[i]);

        result = elosDltMapperMapDataToEvent(&testMapper, &testDltData[i], &testEvent);
        assert_int_equal(result, SAFU_RESULT_OK);

        assert_int_equal(expectedEvent[i].date.tv_sec, testEvent.date.tv_sec);
        assert_int_equal(expectedEvent[i].date.tv_nsec, testEvent.date.tv_nsec);
        assert_string_equal(expectedEvent[i].source.appName, testEvent.source.appName);
        assert_string_equal(expectedEvent[i].source.fileName, testEvent.source.fileName);
        assert_string_equal(expectedEvent[i].hardwareid, testEvent.hardwareid);
        assert_int_equal(expectedEvent[i].source.pid, testEvent.source.pid);
        assert_int_equal(expectedEvent[i].severity, testEvent.severity);
        assert_int_equal(expectedEvent[i].classification, testEvent.classification);
        assert_int_equal(expectedEvent[i].messageCode, testEvent.messageCode);
        assert_string_equal(expectedEvent[i].payload, testEvent.payload);

        result = elosEventDeleteMembers(&testEvent);
        assert_int_equal(result, SAFU_RESULT_OK);
    }
}
