// SPDX-License-Identifier: MIT

#include <pthread.h>
#include <regex.h>

#include "elosEventFilterExecute_utest.h"

#define _STR_HELPER(__s) #__s
#define _STR(x)          _STR_HELPER(x)

#define _VALUE0 1
#define _VALUE1 2

#define REGEX_COUNT 11

static regex_t elosRegex[11];
static size_t elosRegexOffset;
pthread_mutex_t elosRegexLock;

int elosTestEloEventFilterExecuteSuccessElementSetup(void **state) {
    static elosStateTestExecute_t test = {0};

    *state = &test;

    return 0;
}

static regex_t *elosTestSetupRegex(const char *pattern) {
    int res;

    pthread_mutex_lock(&elosRegexLock);

    res = regcomp(&elosRegex[elosRegexOffset], pattern, REG_EXTENDED);
    if (res != 0) {
        regfree(&elosRegex[elosRegexOffset]);
        return NULL;
    }

    elosRegexOffset++;

    pthread_mutex_unlock(&elosRegexLock);

    return &elosRegex[elosRegexOffset - 1];
}

void elosTestEloEventFilterExecuteSuccessElement(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;
    elosEventSource_t eventSourceSet[] = {
        {.appName = "testApp", .fileName = "/tmp/test.elf", .pid = 123},
    };

    // clang-format off
    elosTestExecuteSet_t const testSet[] = {
        {.filter = ".event.date.tv_sec .1 EQ",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 4,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.date = {.tv_sec = 42, .tv_nsec = 21}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(42)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.date = {.tv_sec = 42, .tv_nsec = 21}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(21)},
                 },
                {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.date = {.tv_sec = 42, .tv_nsec = 21}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(0)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.date = {.tv_sec = 0, .tv_nsec = 0}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(42)},
                 },
             }},
        {.filter = ".event.date.tv_nsec .1 EQ",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 4,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.date = {.tv_sec = 42, .tv_nsec = 21}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(21)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.date = {.tv_sec = 42, .tv_nsec = 21}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(42)},
                 },
                {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.date = {.tv_sec = 42, .tv_nsec = 21}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(0)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.date = {.tv_sec = 0, .tv_nsec = 0}},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(42)},
                 },
             }},
        {.filter = ".event.source.pid .1 EQ",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 2,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(123)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(0)},
                 },
             }},
        {.filter = ".event.source.appName .1 STRCMP",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 4,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("testApp")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("testApp2")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("otherApp")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("")},
                 },
             }},
        {.filter = ".event.source.appName .1 REGEX",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 6,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("testApp"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("testApp2"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("otherApp"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("^test.*"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("^test[a-zA-Z]*"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex(""))},
                 },
             }},
        {.filter = ".event.source.fileName .1 STRCMP",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 4,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("/tmp/test.elf")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("/test.elf")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("test.elf")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("")},
                 },
             }},
        {.filter = ".event.source.fileName .1 REGEX",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 5,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("/tmp/test\\.elf"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("/test\\.elf"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("test\\.elf"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex("\\.elf"))},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.source = eventSourceSet[0]},
                     .paramCount = 1,
                     .param = {ELOS_STACK_REGEX(elosTestSetupRegex(".*elf"))},
                 },
             }},
        {.filter = ".event.severity .1 EQ",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 2,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.severity = ELOS_SEVERITY_ERROR},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(ELOS_SEVERITY_ERROR)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.severity = ELOS_SEVERITY_ERROR},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(ELOS_SEVERITY_OFF)},
                 },
             }},
        {.filter = ".event.hardwareid .1 STRCMP",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 4,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.hardwareid = "testHardwareXyz"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("testHardwareXyz")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.hardwareid = "testHardwareXyz"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("testHardware123")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.hardwareid = "testHardwareXyz"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("testHardware")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.hardwareid = "testHardwareXyz"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("")},
                 },
             }},
        {.filter = ".event.classification .1 GE",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 12,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = 0},
                     .paramCount = 1,
                     .param = {ELOS_STACK_U64(0)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = 0},
                     .paramCount = 1,
                     .param = {ELOS_STACK_U32(0)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = 0},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(0)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = 0},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(0)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = ELOS_CLASSIFICATION_ELOS},
                     .paramCount = 1,
                     .param = {ELOS_STACK_U64(ELOS_CLASSIFICATION_ELOS)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = ELOS_CLASSIFICATION_ELOS},
                     .paramCount = 1,
                     .param = {ELOS_STACK_U32(ELOS_CLASSIFICATION_ELOS)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = ELOS_CLASSIFICATION_ELOS},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(ELOS_CLASSIFICATION_ELOS)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = ELOS_CLASSIFICATION_ELOS},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(ELOS_CLASSIFICATION_ELOS)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = 0xFFFFFFFFFFFFFFFFULL},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(-1)},
                 },
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.classification = 0xFFFFFFFFFFFFFFFFULL},
                     .paramCount = 1,
                     .param = {ELOS_STACK_U64(0x7FFFFFFFFFFFFFFFULL)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.classification = 0x7FFFFFFFFFFFFFFFULL},
                     .paramCount = 1,
                     .param = {ELOS_STACK_U64(0xFFFFFFFFFFFFFFFFULL)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.classification = 0x7FFFFFFFFFFFFFFFULL},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I64(-1)},
                 },
             }},
        {.filter = ".event.messageCode .1 EQ",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 2,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.messageCode = ELOS_MSG_CODE_PROCESS_CREATED},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(ELOS_MSG_CODE_PROCESS_CREATED)},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.messageCode = ELOS_MSG_CODE_PROCESS_CREATED},
                     .paramCount = 1,
                     .param = {ELOS_STACK_I32(ELOS_MSG_CODE_PROCESS_EXITED)},
                 },
             }},
        {.filter = ".event.payload .1 STRCMP",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 4,
         .entry =
             {
                 {
                     .result = RPNFILTER_RESULT_MATCH,
                     .event = {.payload = "payload123"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("payload123")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.payload = "payload123"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("payloadXyz")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.payload = "payload123"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("payload")},
                 },
                 {
                     .result = RPNFILTER_RESULT_NO_MATCH,
                     .event = {.payload = "payload123"},
                     .paramCount = 1,
                     .param = {ELOS_STACK_STRING("")},
                 },
             }}
    };
    // clang-format on

    TEST("elosEventFilterExecute");
    SHOULD("%s", "test if event access works properly");

    elosTestExecuteSet(test, testSet, ARRAY_SIZE(testSet));
}

int elosTestEloEventFilterExecuteSuccessElementTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosEventFilterDeleteMembers(&test->filter);

    return 0;
}
