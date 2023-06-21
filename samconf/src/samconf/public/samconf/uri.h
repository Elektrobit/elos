// SPDX-License-Identifier: MIT
#ifndef SAMCONF_URI_H
#define SAMCONF_URI_H

#include <regex.h>

#include "samconf/samconf.h"

/**
 * Simple uri parsing taken from RFC3986 appendix B
 * https://www.rfc-editor.org/rfc/rfc3986#appendix-B
 *
 * FIXME: This is not suffizient to verify relative and absolute uris correctly,
 *        thus this has to be extended following the ABNF specified in RFC3986.
 */
#define SAMCONF_URI_PATTERN "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?"

typedef enum samconfUriMatchGroupE {
    SAMCONF_MATCH_PLAIN,
    SAMCONF_MATCH_SCHEME = 2,
    SAMCONF_MATCH_AUTHORITY = 4,
    SAMCONF_MATCH_PATH = 5,
    SAMCONF_MATCH_QUERY = 6,
    SAMCONF_MATCH_FRAGMENT = 8
} samconfUriMatchGroupE_t;

typedef struct samconfUri {
    char *plain;
    char *scheme;
    char *authority;
    char *path;
    char *query;
    char *fragment;
} samconfUri_t;

samconfConfigStatusE_t samconfUriPattern(regex_t **pattern);
samconfConfigStatusE_t samconfUriNew(samconfUri_t **uri, const char *uriString);
samconfConfigStatusE_t samconfUriInit(samconfUri_t *uri, const char *uriString);
samconfConfigStatusE_t samconfUriDelete(samconfUri_t *uri);
samconfConfigStatusE_t samconfUriDeleteMembers(samconfUri_t *uri);

#endif /* SAMCONF_URI_H */
