// SPDX-License-Identifier: MIT
#define _GNU_SOURCE
#include <safu/common.h>
#include <safu/json.h>
#include <safu/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "samconf/config_backend.h"
#include "samconf/json_backend.h"
#include "samconf/samconf.h"

static void _usage(const char *progName) {
    printf("Usage: %s <config_location>\n", progName);
}

static char *_convert_to_path(const char *pkey, const char *ckey) {
    size_t psize = 2 + strlen(pkey) + strlen(ckey);
    char *path = safuAllocMem(NULL, psize);

    if (path != NULL) {
        strcpy(path, pkey);
        strcat(path, "/");
        strcat(path, ckey);
    }

    return path;
}

static void _printValue(const char *path, const char *value, const char *type) {
    fprintf(stdout, "%s\t%s\t%s\n", path, value, type);
}

static int _readConfig(samconfConfig_t *config) {
    int status = EXIT_SUCCESS;
    size_t ccnt = 0;
    char *path = "";
    const char *value = NULL;
    char *dynValue = NULL;
    const char *type = "";

    if (config == NULL) {
        fprintf(stderr, "ERR: config to read is invalid or null");
        return EXIT_FAILURE;
    }

    ccnt = config->childCount;
    char inttstrbuf[DECIMAL_DIGITS_BOUND(config->value.integer)];

    for (size_t i = 0; i < ccnt; i++) {
        if (strcmp(config->key, "config") == 0) {
            path = _convert_to_path("", config->children[i]->key);
        } else {
            path = _convert_to_path(config->key, config->children[i]->key);
        }

        switch (config->children[i]->type) {
            case SAMCONF_CONFIG_VALUE_STRING:
                type = "SAMCONF_CONFIG_VALUE_STRING";
                value = config->children[i]->value.string;
                break;
            case SAMCONF_CONFIG_VALUE_INT:
                type = "SAMCONF_CONFIG_VALUE_INT";
                sprintf(inttstrbuf, "%li", config->children[i]->value.integer);
                _printValue(path, inttstrbuf, type);
                value = NULL;
                break;
            case SAMCONF_CONFIG_VALUE_BOOLEAN:
                type = "SAMCONF_CONFIG_VALUE_BOOLEAN";
                value = config->children[i]->value.boolean ? "TRUE" : "FALSE";
                break;
            case SAMCONF_CONFIG_VALUE_REAL:
                type = "SAMCONF_CONFIG_VALUE_REAL";
                if (asprintf(&dynValue, "%.06f", config->children[i]->value.real) == -1) {
                    value = "Failed to convert real value to string";
                }
                break;
            case SAMCONF_CONFIG_VALUE_ARRAY:
                type = "SAMCONF_CONFIG_VALUE_ARRAY";
                value = "None";
                break;
            case SAMCONF_CONFIG_VALUE_OBJECT:
                type = "SAMCONF_CONFIG_VALUE_OBJECT";
                value = "None";
                break;
            default:
                type = "None";
                value = "None";
                break;
        }

        if (value != NULL) {
            _printValue(path, value, type);
        } else {
            if (dynValue != NULL) {
                _printValue(path, dynValue, type);
                free(dynValue);
                value = NULL;
            }
        }

        free(path);

        if (config->children[i]->childCount > 0) {
            status = _readConfig(config->children[i]);
        }
    }

    return status;
}

int main(int argc, char **argv) {
    samconfConfig_t *config;
    samconfConfigStatusE_t status;

    if (argc == 1) {
        _usage(argv[0]);
        return EXIT_FAILURE;
    }

    /* TODO: Introduce parameter for enforcing signature testing */
    status = samconfLoad(argv[1], false, &config);

    if (status == SAMCONF_CONFIG_ERROR) {
        fprintf(stderr, "ERR: An error occured while loading form config file");
        return EXIT_FAILURE;
    } else if (status == SAMCONF_CONFIG_INVALID_SIGNATURE) {
        fprintf(stderr, "ERR: Signature was invalid");
        return EXIT_FAILURE;
    } else if (config == NULL) {
        fprintf(stderr, "ERR: unexpected error occured, where config was intialized but no expected error was caught");
        return EXIT_FAILURE;
    }

    _printValue("PATH", "VALUE", "TYPE");
    int result = _readConfig(config);

    status = samconfConfigDelete(config);

    if (status == SAMCONF_CONFIG_ERROR) {
        fprintf(stderr, "ERR: An error occured while deleting config");
        return EXIT_FAILURE;
    }

    return result;
}
