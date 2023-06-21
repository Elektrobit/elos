// SPDX-License-Identifier: MIT
#pragma once
#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>
#include <safu/json.h>

#define SETUP(__test)                           \
    int __test##Setup(void **state) {           \
        static struct json_object *jobj = NULL; \
        *state = jobj;                          \
        return 0;                               \
    }

#define TEARDOWN(__test)                                          \
    int __test##Teardown(void **state) {                          \
        struct json_object *jobj = *(struct json_object **)state; \
        if (jobj != NULL) {                                       \
            json_object_put(jobj);                                \
        }                                                         \
        return 0;                                                 \
    }
