// SPDX-License-Identifier: MIT

#include "elosEventFilterExecute_utest.h"

int elosTestElosEventFilterExecuteSuccessHardwareidSetup(void **state) {
    static elosStateTestExecute_t test = {0};

    *state = &test;

    return 0;
}

#define ID "theID"

void elosTestElosEventFilterExecuteSuccessHardwareid(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;
    elosEventSource_t eventSourceSet[] = {
        {.appName = "testApp", .fileName = "/tmp/test.elf"},
        {.appName = NULL, .fileName = "/tmp/test.elf"},
        {.appName = "testApp", .fileName = NULL},
        {.appName = NULL, .fileName = NULL},
    };
    char *payload = "an Event payload";
    char *id = "Anotherid_not_found_by_the_filter";

    // clang-format off
    elosTestExecuteSet_t const testSet[] = {
        {.filter = ".event.hardwareid '" ID "' STRCMP",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 8,
         .entry = {{
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[0], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[0], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[1], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[1], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[2], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[2], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[3], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_MATCH,
             .event = {.hardwareid = ID, .source = eventSourceSet[3], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }}},
        {.filter = ".event.hardwareid '" ID "' STRCMP",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 8,
         .entry = {{
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[0], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[0], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[1], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[1], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[2], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[2], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[3], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = id, .source = eventSourceSet[3], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }}},
        {.filter = ".event.hardwareid '" ID "' STRCMP",
         .createResult = RPNFILTER_RESULT_OK,
         .entryCount = 8,
         .entry = {{
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[0], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[0], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[1], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[1], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[2], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[2], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[3], .payload = payload},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }, {
             .result = RPNFILTER_RESULT_NO_MATCH,
             .event = {.hardwareid = NULL, .source = eventSourceSet[3], .payload = NULL},
             .paramCount = 1,
             .param = {ELOS_STACK_STRING(ID)},
         }}}};
    // clang-format on

    TEST("elosEventFilterExecute");
    SHOULD("%s", "test if event access works properly for hrdwareid");

    elosTestExecuteSet(test, testSet, ARRAY_SIZE(testSet));
}

int elosTestElosEventFilterExecuteSuccessHardwareidTeardown(void **state) {
    elosStateTestExecute_t *test = *(elosStateTestExecute_t **)state;

    elosEventFilterDeleteMembers(&test->filter);

    return 0;
}
