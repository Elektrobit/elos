// SPDX-License-Identifier: MIT
#include "samconf/signature.h"

#include <safu/common.h>
#include <safu/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static const char samconfSigSuffix[] = ".sig";
static const size_t samconfSigSuffixLen = ARRAY_SIZE(samconfSigSuffix) - 1;

samconfConfigStatusE_t samconfSignatureLoaderGetSignatureFor(const char *file, char **signature, size_t *len) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    ssize_t read;
    char *signatureFile = NULL;
    size_t fileLen = strlen(file);
    size_t sigFileLen = fileLen + samconfSigSuffixLen + 1;

    signatureFile = safuAllocMem(NULL, sizeof(char) * sigFileLen);
    if (signatureFile != NULL) {
        strncpy(signatureFile, file, fileLen + 1);
        strcat(signatureFile, samconfSigSuffix);

        if (access(signatureFile, R_OK | F_OK) == 0) {
            read = safuReadFileToString(signatureFile, -1, signature);
            if (read >= 0) {
                *len = read;
                status = SAMCONF_CONFIG_OK;
            } else {
                safuLogErrF("Failed to read signature from %s: Read failed.\n", signatureFile);
            }
        } else {
            safuLogInfoF("Signature file %s does not exist.\n", signatureFile);
            status = SAMCONF_CONFIG_SIGNATURE_NOT_FOUND;
        }

        free(signatureFile);
    } else {
        safuLogErrF("Failed to allocate memory for signature path: %s.sig.\n", file);
    }

    return status;
}
