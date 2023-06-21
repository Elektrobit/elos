// SPDX-License-Identifier: MIT

#include "safu/ringbuffer.h"

#include <stdlib.h>

#include "safu/flags.h"
#include "safu/log.h"
#include "safu/mutex.h"

safuResultE_t _replaceEntry(safuRingBuffer_t *ringBuffer, size_t idx, void *element) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (idx >= ringBuffer->elements) {
        safuLogErr("Invalid ringBuffer state (idx > ringBuffer.elements)");
        result = SAFU_RESULT_FAILED;
    } else {
        if (SAFU_FLAG_HAS_DELETE_ENTRIES_BIT(&ringBuffer->flags) == true) {
            if (ringBuffer->data[idx] != NULL) {
                if (ringBuffer->callback.delete != NULL) {
                    result = ringBuffer->callback.delete(ringBuffer->data[idx]);
                } else {
                    free(ringBuffer->data[idx]);
                }
            }
        }
    }

    ringBuffer->data[idx] = element;

    return result;
}

safuResultE_t safuRingBufferInitialize(safuRingBuffer_t *ringBuffer, safuRingBufferParam_t const *const param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((ringBuffer == NULL) || (param == NULL) || (param->elements == 0)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&ringBuffer->flags) == true) {
        safuLogErr("The given ringBuffer is already initialized");
    } else {
        SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(&ringBuffer->lock, NULL, result);
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT_IF_OK(&ringBuffer->lock, result);
        if (result == SAFU_RESULT_OK) {
            size_t const elements = param->elements;
            size_t const memorySize = elements * SAFU_RINGBUFFER_ELEMENT_SIZE;
            void **newData;

            newData = calloc(1, memorySize);
            if (newData == NULL) {
                safuLogErr("Memory allocation failed");
                result = SAFU_RESULT_FAILED;
            } else {
                ringBuffer->callback.delete = param->deleteFunc;
                ringBuffer->elements = elements;
                ringBuffer->memorySize = memorySize;
                ringBuffer->data = newData;
                ringBuffer->nextIdx = 0;

                atomic_store(&ringBuffer->flags, SAFU_FLAG_INITIALIZED_BIT);

                if (param->deleteEntries == true) {
                    atomic_fetch_or(&ringBuffer->flags, SAFU_RINGBUFFER_DELETE_ENTRIES_BIT);
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&ringBuffer->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t safuRingBufferNew(safuRingBuffer_t **ringBuffer, safuRingBufferParam_t const *const param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (ringBuffer == NULL) {
        safuLogErr("Invalid parameter given");
    } else {
        void *newRingBuffer;

        newRingBuffer = calloc(1, sizeof(safuRingBuffer_t));
        if (newRingBuffer == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            result = safuRingBufferInitialize(newRingBuffer, param);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("RingBuffer initialization failed");
                free(newRingBuffer);
            } else {
                *ringBuffer = newRingBuffer;
            }
        }
    }

    return result;
}

safuResultE_t safuRingBufferDeleteMembers(safuRingBuffer_t *ringBuffer) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (ringBuffer == NULL) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&ringBuffer->flags) == false) {
        safuLogErr("The given ringBuffer is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&ringBuffer->lock, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&ringBuffer->flags) == false) {
                safuLogErr("The given ringBuffer is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                if (ringBuffer->data != NULL) {
                    for (size_t i = 0; i < ringBuffer->elements; i += 1) {
                        safuResultE_t iterResult;

                        iterResult = _replaceEntry(ringBuffer, i, NULL);
                        if (iterResult != SAFU_RESULT_OK) {
                            safuLogWarnF("Delete for Ringbuffer idx:%lu failed (possible memory leak)", i);
                            result = iterResult;
                        }
                    }

                    free(ringBuffer->data);
                    ringBuffer->data = NULL;
                }

                ringBuffer->callback.delete = NULL;
                ringBuffer->elements = 0;
                ringBuffer->nextIdx = 0;
                ringBuffer->memorySize = 0;
                ringBuffer->elementsWritten = 0;

                atomic_store(&ringBuffer->flags, SAFU_FLAG_NONE);
            }

            SAFU_PTHREAD_MUTEX_DESTROY(&ringBuffer->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t safuRingBufferDelete(safuRingBuffer_t **ringBuffer) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (ringBuffer != NULL) {
        result = safuRingBufferDeleteMembers(*ringBuffer);
        if (result == SAFU_RESULT_OK) {
            free(*ringBuffer);
            *ringBuffer = NULL;
        }
    }

    return result;
}

safuResultE_t safuRingBufferWrite(safuRingBuffer_t *ringBuffer, void *element) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((ringBuffer == NULL) || (element == NULL)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&ringBuffer->flags) == false) {
        safuLogErr("The given ringBuffer is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&ringBuffer->lock, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&ringBuffer->flags) == false) {
                safuLogErr("The given ringBuffer is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                size_t const elements = ringBuffer->elements;
                size_t const idx = ringBuffer->nextIdx;

                result = _replaceEntry(ringBuffer, idx, element);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("Writing into RingBuffer failed");
                } else {
                    ringBuffer->elementsWritten += 1;

                    if (elements <= (idx + 1)) {
                        ringBuffer->nextIdx = 0;
                    } else {
                        ringBuffer->nextIdx = idx + 1;
                    }
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&ringBuffer->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

static void _copyData(safuRingBuffer_t *ringBuffer, void **newData) {
    void **oldData = ringBuffer->data;
    size_t const elements = ringBuffer->elements;
    size_t curIdx = ringBuffer->nextIdx;

    if (curIdx == 0) {
        curIdx = ringBuffer->elements - 1;
    } else {
        curIdx -= 1;
    }

    if (ringBuffer->elementsWritten <= elements) {
        memcpy(newData, oldData, ringBuffer->memorySize);
    } else {
        // Linearize out of order elements after an overflow, e.g. {6,7,3,4,5} => {3,4,5,6,7}
        size_t const elementSize = SAFU_RINGBUFFER_ELEMENT_SIZE;
        size_t const elementsOlder = (elements - 1 - curIdx);
        size_t const idxOlder = (curIdx + 1);
        size_t const sizeOlder = (elementsOlder * elementSize);
        size_t const idxNewer = elementsOlder;
        size_t const sizeNewer = (idxOlder * elementSize);

        memcpy(&newData[0], &oldData[idxOlder], sizeOlder);
        memcpy(&newData[idxNewer], &oldData[0], sizeNewer);
    }
}

safuResultE_t safuRingBufferRead(safuRingBuffer_t *ringBuffer, safuVec_t *elementVector, size_t *elementsWritten) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((ringBuffer == NULL) || (elementVector == NULL)) {
        safuLogErr("Invalid parameter given");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&ringBuffer->flags) == false) {
        safuLogErr("The given ringBuffer is not initialized");
    } else {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&ringBuffer->lock, result);
        if (result == SAFU_RESULT_OK) {
            if (SAFU_FLAG_HAS_INITIALIZED_BIT(&ringBuffer->flags) == false) {
                safuLogErr("The given ringBuffer is not initialized");
                result = SAFU_RESULT_FAILED;
            } else {
                void **newData;

                newData = safuAllocMem(elementVector->data, ringBuffer->memorySize);
                if (newData == NULL) {
                    result = SAFU_RESULT_FAILED;
                } else {
                    size_t elementCount = ringBuffer->elements;

                    if (ringBuffer->elementsWritten <= elementCount) {
                        elementCount = ringBuffer->elementsWritten;
                    }

                    _copyData(ringBuffer, newData);

                    elementVector->data = (void *)newData;
                    elementVector->elementCount = elementCount;
                    elementVector->elementSize = SAFU_RINGBUFFER_ELEMENT_SIZE;
                    elementVector->memorySize = ringBuffer->memorySize;

                    if (elementsWritten != NULL) {
                        *elementsWritten = ringBuffer->elementsWritten;
                    }

                    memset(ringBuffer->data, 0, ringBuffer->memorySize);
                    ringBuffer->elementsWritten = 0;
                    ringBuffer->nextIdx = 0;
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&ringBuffer->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}
