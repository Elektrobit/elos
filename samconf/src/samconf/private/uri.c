// SPDX-License-Identifier: MIT
#include "samconf/uri.h"

#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>
#include <string.h>

// FIXME: Ensure thread safety using a mutex (see elos/mutex.h).
NOINLINE samconfConfigStatusE_t samconfUriPattern(regex_t **pattern) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;
    int err;
    char errBuff[256];
    static regex_t pregParse;
    static regex_t *pregParsePtr = NULL;

    if (pregParsePtr == NULL) {
        if ((err = regcomp(&pregParse, SAMCONF_URI_PATTERN, REG_EXTENDED)) == 0) {
            *pattern = pregParsePtr = &pregParse;
        } else {
            regerror(err, &pregParse, errBuff, 256);
            safuLogErrF("Compilation of uri parse regex failed: %s\n", errBuff);
            status = SAMCONF_CONFIG_ERROR;
        }
    } else {
        *pattern = pregParsePtr;
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfUriNew(samconfUri_t **uri, const char *uriString) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    samconfUri_t *tmp;

    tmp = safuAllocMem(NULL, sizeof(*tmp));
    if (tmp != NULL) {
        status = samconfUriInit(tmp, uriString);
        if (status == SAMCONF_CONFIG_OK) {
            *uri = tmp;
        }
    } else {
        safuLogErr("Failed to allocate uri.\n");
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfUriInit(samconfUri_t *uri, const char *uriString) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;
    regmatch_t pmatch[9];
    size_t i, nmatch = 9;

    regex_t *pattern;
    status = samconfUriPattern(&pattern);
    if (status == SAMCONF_CONFIG_OK) {
        memset(uri, 0, sizeof(*uri));
        if (regexec(pattern, uriString, nmatch, pmatch, 0) == 0) {
            for (i = 0; i < nmatch; i++) {
                char *match = strndup(&uriString[pmatch[i].rm_so], pmatch[i].rm_eo - pmatch[i].rm_so);

                switch (i) {
                    case SAMCONF_MATCH_PLAIN:
                        uri->plain = match;
                        break;
                    case SAMCONF_MATCH_SCHEME:
                        uri->scheme = match;
                        break;
                    case SAMCONF_MATCH_AUTHORITY:
                        uri->authority = match;
                        break;
                    case SAMCONF_MATCH_PATH:
                        uri->path = match;
                        break;
                    case SAMCONF_MATCH_QUERY:
                        uri->query = match;
                        break;
                    case SAMCONF_MATCH_FRAGMENT:
                        uri->fragment = match;
                        break;
                    default:
                        free(match);
                        break;
                }
            }
        } else {
            safuLogErrF("Failed to match uri '%s'.\n", uriString);
            status = SAMCONF_CONFIG_ERROR;
        }
    } else {
        safuLogErrF("Failed to fetch uri pattern to check '%s'.\n", uriString);
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfUriDelete(samconfUri_t *uri) {
    samconfConfigStatusE_t status;

    status = samconfUriDeleteMembers(uri);

    free(uri);

    return status;
}

NOINLINE samconfConfigStatusE_t samconfUriDeleteMembers(samconfUri_t *uri) {
    free(uri->plain);
    free(uri->scheme);
    free(uri->authority);
    free(uri->path);
    free(uri->query);
    free(uri->fragment);

    return SAMCONF_CONFIG_OK;
}
