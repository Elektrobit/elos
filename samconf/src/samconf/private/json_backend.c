// SPDX-License-Identifier: MIT
#include "samconf/json_backend.h"

#include <fcntl.h>
#include <json-c/json.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>
#include <unistd.h>

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

samconfConfigStatusE_t samconfJsonBackendOpen(const char *location, samconfConfigBackend_t *backend) {
    json_object *root = NULL;

    root = json_object_from_file(location);

    if (!root) {
        safuLogCritF("Failed to read json Config: %s.", json_util_get_last_err());
        return SAMCONF_CONFIG_ERROR;
    }

    backend->backendHandle = root;
    return SAMCONF_CONFIG_OK;
}

samconfConfigStatusE_t samconfJsonBackendSupports(const char *location, bool *isSupported) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    const char *ext = strrchr(location, '.');

    if (ext) {
        if (strcasecmp(ext + 1, "json") == 0) {
            *isSupported = true;
            status = SAMCONF_CONFIG_OK;
        }
    }

    return status;
}

samconfConfigStatusE_t samconfJsonBackendClose(samconfConfigBackend_t *backend) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;
    int returnCode = json_object_put(backend->backendHandle);
    if (returnCode != 1) {
        safuLogErrF("Failed to free json Config: %s.", json_util_get_last_err());
        status = SAMCONF_CONFIG_ERROR;
    }
    free(backend->originalHandle);
    // no need to free backend->internal, element of pointer already freed by json_object_put
    return status;
}

static samconfConfigStatusE_t _transform_json_to_config(json_object *jobj, bool isSigned, samconfConfig_t *config) {
    int n;
    json_type type;
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    samconfConfig_t *subconfig;
    json_object *newJobj;
    const char *key;
    struct lh_entry *entry;

    config->isSigned = isSigned;

    type = json_object_get_type(jobj);
    switch (type) {
        case json_type_object:
            config->type = SAMCONF_CONFIG_VALUE_OBJECT;
            entry = json_object_get_object(jobj)->head;
            while (entry) {
                key = (char *)entry->k;
                newJobj = (json_object *)entry->v;
                if (newJobj == NULL) {
                    safuLogErr("Failed to load Object from Map");
                    result = SAMCONF_CONFIG_PARSE_ERROR;
                    break;
                }
                entry = entry->next;
                result = samconfConfigNew(&subconfig);
                if (result != SAMCONF_CONFIG_OK) {
                    break;
                }
                subconfig->key = strdup(key);
                result = _transform_json_to_config(newJobj, isSigned, subconfig);
                if (result != SAMCONF_CONFIG_OK) {
                    samconfConfigDelete(subconfig);  // ensuring cleanup of unlinked subconfig
                    break;
                }
                result = samconfConfigAdd(config, subconfig);
                if (result != SAMCONF_CONFIG_OK) {
                    samconfConfigDelete(subconfig);  // ensuring cleanup of unlinked subconfig
                    break;
                }
            }
            break;

        case json_type_array:
            config->type = SAMCONF_CONFIG_VALUE_ARRAY;
            n = json_object_array_length(jobj);
            for (int idx = 0; idx < n; idx++) {
                newJobj = json_object_array_get_idx(jobj, idx);
                if (newJobj == NULL) {
                    result = SAMCONF_CONFIG_PARSE_ERROR;
                    safuLogErr("Failed to load Object from Array");
                    break;
                }
                result = samconfConfigNew(&subconfig);
                if (result != SAMCONF_CONFIG_OK) {
                    break;
                }
                result = _transform_json_to_config(newJobj, isSigned, subconfig);
                if (result != SAMCONF_CONFIG_OK) {
                    samconfConfigDelete(subconfig);  // ensuring cleanup of unlinked subconfig
                    break;
                }
                result = samconfConfigAdd(config, subconfig);
                if (result != SAMCONF_CONFIG_OK) {
                    samconfConfigDelete(subconfig);  // ensuring cleanup of unlinked subconfig
                    break;
                }
                subconfig->key = safuAllocMem(NULL, DECIMAL_DIGITS_BOUND(idx));
                snprintf(subconfig->key, DECIMAL_DIGITS_BOUND(idx), "%i", idx);
            }
            break;

        case json_type_string:
            result = samconfConfigSetString(config, json_object_get_string(jobj));
            break;
        case json_type_int:
            result = samconfConfigSetInt(config, json_object_get_int(jobj));
            break;
        case json_type_boolean:
            result = samconfConfigSetBool(config, json_object_get_boolean(jobj));
            break;
        case json_type_double:
            result = samconfConfigSetReal(config, json_object_get_double(jobj));
            break;
        default:
            break;
    }

    return result;
}

samconfConfigStatusE_t samconfJsonBackendLoad(samconfConfigBackend_t *backend, bool isSigned,
                                              samconfConfig_t **config) {
    samconfConfigStatusE_t result;

    if (backend == NULL) {
        safuLogErr("Failed to create config tree due to invalid backend");
        return SAMCONF_CONFIG_ERROR;
    }

    result = samconfConfigNew(config);

    if (result != SAMCONF_CONFIG_OK) {
        safuLogErr("Failed to initialize Config");
        return result;
    }

    (*config)->type = SAMCONF_CONFIG_VALUE_OBJECT;
    (*config)->key = strdup("config");

    return _transform_json_to_config(backend->backendHandle, isSigned, *config);
}

const samconfConfigBackendOps_t samconfJsonOps = {
    .supports = samconfJsonBackendSupports,
    .open = samconfJsonBackendOpen,
    .close = samconfJsonBackendClose,
    .load = samconfJsonBackendLoad,
};
