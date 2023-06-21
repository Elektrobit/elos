// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <stdlib.h>

#include "elos/rpnfilter/builder.h"

#define _ALIGNMENT      8
#define _ALIGNMENT_MASK (uint32_t)(~(_ALIGNMENT - 1))

static inline uint32_t _alignedSize(uint32_t bytes) {
    uint32_t result = bytes & _ALIGNMENT_MASK;

    if (bytes & (_ALIGNMENT - 1)) {
        result += _ALIGNMENT;
    }

    return result;
}

static inline bool _hasPtr(elosRpnFilterStackEntry_t const *entry) {
    bool result = false;

    switch (entry->type) {
        case RPNFILTER_VALUE_UNDEFINED:
        case RPNFILTER_VALUE_BINARY:
        case RPNFILTER_VALUE_STRING:
        case RPNFILTER_VALUE_REGEX:
            result = true;
            break;
        default:
            break;
    }

    return result;
}

static elosRpnFilterResultE_t _countMemoryBytes(elosRpnFilterBuilder_t const *data, uint32_t *bytes) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    uint32_t tmpBytes = 0;
    uint32_t elements;
    uint32_t idx;

    elements = safuVecElements(&data->step);
    tmpBytes = _alignedSize((elements * sizeof(elosRpnFilterStepEntry_t)) + sizeof(elosRpnFilterStep_t));

    elements = safuVecElements(&data->stack);
    tmpBytes += _alignedSize((elements * sizeof(elosRpnFilterStackEntry_t)) + sizeof(elosRpnFilterStack_t));

    for (idx = 0; idx < elements; idx += 1) {
        elosRpnFilterStackEntry_t *entry;

        entry = safuVecGet(&data->stack, idx);
        if (entry == NULL) {
            safuLogErrF("stack[%d]: safuVecGet failed", idx);
            result = RPNFILTER_RESULT_ERROR;
            break;
        }

        if (_hasPtr(entry) == true) {
            tmpBytes += _alignedSize(entry->bytes);
        }
    }

    if (result == RPNFILTER_RESULT_OK) {
        *bytes = _alignedSize(tmpBytes);
    }

    return result;
}

static elosRpnFilterResultE_t _packSteps(elosRpnFilterBuilder_t *data, elosRpnFilter_t *filter, size_t *offset) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    uint8_t *memory = (uint8_t *)filter->memory;
    uint32_t elements;
    uint32_t idx;

    elements = safuVecElements(&data->step);
    filter->steps = (elosRpnFilterStep_t *)&memory[*offset];
    filter->steps->count = elements;
    *offset += _alignedSize((elements * sizeof(elosRpnFilterStepEntry_t)) + sizeof(elosRpnFilterStep_t));

    for (idx = 0; idx < elements; idx += 1) {
        elosRpnFilterStepEntry_t *entry;

        entry = safuVecGet(&data->step, idx);
        if (entry == NULL) {
            safuLogErrF("stack[%d]: safuVecGet failed", idx);
            result = RPNFILTER_RESULT_ERROR;
            break;
        }

        memcpy(&filter->steps->entry[idx], entry, sizeof(elosRpnFilterStepEntry_t));
    }

    return result;
}

static elosRpnFilterResultE_t _packStack(elosRpnFilterBuilder_t *data, elosRpnFilter_t *filter, size_t *offset) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    uint8_t *memory = (uint8_t *)filter->memory;
    uint32_t elements;
    uint32_t idx;

    elements = safuVecElements(&data->stack);
    filter->values = (elosRpnFilterStack_t *)&memory[*offset];
    filter->values->count = elements;
    *offset += _alignedSize((elements * sizeof(elosRpnFilterStackEntry_t)) + sizeof(elosRpnFilterStack_t));

    for (idx = 0; idx < elements; idx += 1) {
        elosRpnFilterStackEntry_t *dEntry;
        elosRpnFilterStackEntry_t *pEntry = &filter->values->entry[idx];

        dEntry = safuVecGet(&data->stack, idx);
        if (dEntry == NULL) {
            safuLogErrF("stack [%d]: safuVecGet failed", idx);
            result = RPNFILTER_RESULT_ERROR;
            break;
        }

        memcpy(pEntry, dEntry, sizeof(elosRpnFilterStackEntry_t));
        if (_hasPtr(dEntry) == true) {
            pEntry->data.ptr = &memory[*offset];
            memcpy(pEntry->data.ptr, dEntry->data.ptr, dEntry->bytes);
            *offset += _alignedSize(dEntry->bytes);
        }
    }

    return result;
}

elosRpnFilterResultE_t elosRpnFilterBuilderFinalize(elosRpnFilterBuilder_t *data, elosRpnFilter_t *filter) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_ERROR;
    elosRpnFilter_t tmpFilter = ELOS_RPNFILTER_INIT;

    if ((data == NULL) || (filter == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (data->status != RPNFILTER_CREATE_COMPILED) {
        safuLogErr("The given elosRpnFilterBuilder_t is not in state 'FILTER_CREATE_COMPILED'");
    } else {
        uint32_t bytes = 0;

        result = _countMemoryBytes(data, &bytes);
        if (result == RPNFILTER_RESULT_ERROR) {
            safuLogErr("_countMemoryBytes failed");
        } else {
            void *memory = NULL;

            memory = safuAllocMem(NULL, bytes);
            if (memory == NULL) {
                safuLogErr("safuAllocMem failed");
                result = RPNFILTER_RESULT_ERROR;
            } else {
                size_t offset = 0;

                memset(memory, 0, bytes);
                tmpFilter.memory = memory;
                tmpFilter.memorySize = bytes;

                result = _packSteps(data, &tmpFilter, &offset);
                if (result == RPNFILTER_RESULT_ERROR) {
                    safuLogErr("_packSteps failed");
                    free(memory);
                } else {
                    result = _packStack(data, &tmpFilter, &offset);
                    if (result == RPNFILTER_RESULT_ERROR) {
                        safuLogErr("_packStack failed");
                        free(memory);
                    } else {
                        tmpFilter.state = RPNFILTER_FINALIZED;
                        memcpy(filter, &tmpFilter, sizeof(elosRpnFilter_t));
                    }
                }
            }
        }
    }

    return result;
}
