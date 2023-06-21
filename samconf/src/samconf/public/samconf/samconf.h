// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf_types.h>

const struct samconfConfigBackendOps *samconfGetBackendOps(size_t idx);
samconfConfigStatusE_t samconfLookupBackend(const char *location, struct samconfConfigBackend **backend);
samconfConfigStatusE_t samconfVerifySignature(const char *location);
samconfConfigStatusE_t samconfLoad(const char *location, bool enforceSignature, samconfConfig_t **const config);

samconfConfigStatusE_t samconfConfigNew(samconfConfig_t **const config);
samconfConfigStatusE_t samconfConfigInit(samconfConfig_t *config);
samconfConfigStatusE_t samconfConfigDelete(samconfConfig_t *config);
samconfConfigStatusE_t samconfConfigAdd(samconfConfig_t *parent, samconfConfig_t *child);
samconfConfigStatusE_t samconfConfigDeleteMembers(samconfConfig_t *config);
samconfConfigStatusE_t samconfConfigGet(const samconfConfig_t *root, const char *path, const samconfConfig_t **result);
samconfConfigStatusE_t samconfConfigGetString(const samconfConfig_t *root, const char *path, const char **result);
samconfConfigStatusE_t samconfConfigSetString(samconfConfig_t *config, const char *stringValue);
samconfConfigStatusE_t samconfConfigGetBool(const samconfConfig_t *root, const char *path, bool *result);
samconfConfigStatusE_t samconfConfigGetInt32(const samconfConfig_t *root, const char *path, int32_t *result);
samconfConfigStatusE_t samconfConfigGetReal(const samconfConfig_t *root, const char *path, double *result);
samconfConfigStatusE_t samconfConfigSetInt(samconfConfig_t *config, int64_t intValue);
samconfConfigStatusE_t samconfConfigSetBool(samconfConfig_t *config, bool value);
samconfConfigStatusE_t samconfConfigSetReal(samconfConfig_t *config, double value);
int samconfInitConfig();
