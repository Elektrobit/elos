// SPDX-License-Identifier: MIT
#pragma once

#include <safu/defines.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct samconfConfigBackend;
struct samconfConfigBackendOps;

typedef enum samconfConfigStatusE {
    SAMCONF_CONFIG_ERROR = -1,
    SAMCONF_CONFIG_OK = 0,
    SAMCONF_CONFIG_EOF,
    SAMCONF_CONFIG_NOT_FOUND,
    SAMCONF_CONFIG_INVALID_TYPE,
    SAMCONF_CONFIG_NOT_IMPLEMENTED,
    SAMCONF_CONFIG_PARSE_ERROR,
    SAMCONF_CONFIG_INVALID_SIGNATURE,
    SAMCONF_CONFIG_SIGNATURE_NOT_FOUND,
} samconfConfigStatusE_t;

typedef enum samconfConfigValueTypeE {
    SAMCONF_CONFIG_VALUE_NONE = 0,
    SAMCONF_CONFIG_VALUE_STRING,
    SAMCONF_CONFIG_VALUE_INT,
    SAMCONF_CONFIG_VALUE_REAL,
    SAMCONF_CONFIG_VALUE_BOOLEAN,
    SAMCONF_CONFIG_VALUE_ARRAY,
    SAMCONF_CONFIG_VALUE_OBJECT,
} samconfConfigValueTypeE_t;

typedef enum samconfConfigSourceTypeE {
    SAMCONF_CONFIG_SOURCE_TYPE_NONE = 0,
    SAMCONF_CONFIG_SOURCE_TYPE_PARAMETER,
    SAMCONF_CONFIG_SOURCE_TYPE_ENVIRONMENT,
    SAMCONF_CONFIG_SOURCE_TYPE_FILE,
    SAMCONF_CONFIG_SOURCE_TYPE_BUILDIN,
} samconfConfigSourceTypeE_t;

typedef struct samconfConfigSource {
    samconfConfigSourceTypeE_t type;
    struct samconfConfigBackend *backend;
} samconfConfigSource_t;

typedef struct samconfConfig {
    struct samconfConfig *parent;
    char *key;
    bool isSigned;
    samconfConfigValueTypeE_t type;
    union {
        char *string;
        int64_t integer;
        bool boolean;
        double real;
    } value;
    struct samconfConfig **children;
    size_t childCount;
} samconfConfig_t;
