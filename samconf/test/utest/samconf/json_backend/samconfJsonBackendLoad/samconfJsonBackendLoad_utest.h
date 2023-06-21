// SPDX-License-Identifier: MIT
#ifndef SAMCONF_JSON_BACKEND_LOAD_UTEST_H
#define SAMCONF_JSON_BACKEND_LOAD_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "samconf/config_backend.h"
#include "samconf/json_backend.h"
#include "samconf/samconf.h"

#define JSON_CONFIG                                         \
    "{ "                                                    \
    "\"root\": { "                                          \
    "\"categoryA\" : { "                                    \
    "\"value\" : \"the value of a configuration option\", " \
    "\"number\": 23, "                                      \
    "\"bool\": true, "                                      \
    "\"real\": 4.231, "                                     \
    "}, "                                                   \
    "\"categoryB\" : [ "                                    \
    "\"itemA\", "                                           \
    "\"itemB\", "                                           \
    "\"itemC\", "                                           \
    "42, "                                                  \
    "] "                                                    \
    "} "                                                    \
    "}"

#define JSON_CONFIG_SWAPPED                                \
    "{ "                                                   \
    "\"root\": { "                                         \
    "\"categoryB\" : [ "                                   \
    "\"itemA\", "                                          \
    "\"itemB\", "                                          \
    "\"itemC\", "                                          \
    "], "                                                  \
    "\"categoryA\" : { "                                   \
    "\"value\" : \"the value of a configuration option\" " \
    "} "                                                   \
    "} "                                                   \
    "}"

typedef struct samconfJsonBackendLoadTestData {
    samconfConfigBackend_t *backend;
    samconfConfig_t *config;
} samconfJsonBackendLoadTestData_t;

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadSuccessArrayNoloop)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorCompromisedConfig)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorCompromisedSubconfigObject)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorCompromisedSubconfigArray)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorFailedAddObject)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorFailedAddArray)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorNullObjectArray)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorNullObjectObject)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfJsonBackendLoadErrorNullBackend)

#endif /* SAMCONF_JSON_BACKEND_LOAD_UTEST_H */
