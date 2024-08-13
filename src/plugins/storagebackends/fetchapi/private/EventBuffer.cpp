// SPDX-License-Identifier: MIT

#include "public/EventBuffer.h"

#include <elos/event/event.h>
#include <elos/eventfilter/eventfilter.h>
#include <elos/eventfilter/eventfilter_types.h>
#include <new>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/vector.h>

EventBuffer::EventBuffer(size_t len)
    : size(len), start(0), end(SIZE_MAX), buffer(new elosEvent_t[len]) {
        for (size_t idx = 0; idx < size; idx++) {
            safuResultE_t result = elosEventInitialize(&this->buffer[idx]);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("initializing EventBuffer failed!");
                throw SAFU_RESULT_FAILED;
            }
        }
    }
EventBuffer::~EventBuffer() noexcept(false) {
    safuResultE_t result = SAFU_RESULT_OK;
    for (size_t idx = 0; idx < this->size; idx++) {
        if (elosEventDeleteMembers(&this->buffer[idx]) != SAFU_RESULT_OK) {
            safuLogErr("Error freeing events from event buffer!");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result != SAFU_RESULT_OK) {
        throw SAFU_RESULT_FAILED;
    }
}
size_t EventBuffer::indexIncrement(size_t idx) const noexcept {
    return (idx + 1) % this->size;
}
safuResultE_t EventBuffer::pushEvent(const elosEvent_t &event) noexcept {
    safuResultE_t result = SAFU_RESULT_OK;

    if (SIZE_MAX == this->end) {
        result = elosEventDeepCopy(&this->buffer[this->start], &event);
        this->end = 1;
        return result;
    }
    result = elosEventDeleteMembers(&this->buffer[this->end]);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("Failed to free event from buffer");
    } else {
        result = elosEventDeepCopy(&this->buffer[this->end], &event);

        if (this->start == this->end) {
            this->start = this->indexIncrement(this->start);
        }
        this->end = this->indexIncrement(this->end);
    }
    return result;
}
safuResultE_t EventBuffer::findEvents(const elosEventFilter_t &filter,
        safuVec_t &eventList) const noexcept {
    safuResultE_t result = SAFU_RESULT_OK;
    if (SIZE_MAX == this->end) {
        safuLogDebug("EventRingBuffer is empty!");
        return result;
    }
    size_t idx = this->start;
    do {
        elosRpnFilterResultE_t filterResult;
        filterResult = elosEventFilterExecute(&filter, nullptr, &this->buffer[idx]);
        if (filterResult == RPNFILTER_RESULT_MATCH) {
            elosEvent_t *event = NULL;
            result = elosEventNew(&event);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("failed to allocate event for fetch api call");
            } else {
                result = elosEventDeepCopy(event, &this->buffer[idx]);
                if (result != SAFU_RESULT_OK) {
                    elosEventDelete(event);
                    safuLogErr("failed to copy event for fetch api call");
                } else if (safuVecPush(&eventList, event) != 0) {
                    result = SAFU_RESULT_FAILED;
                    elosEventDelete(event);
                    safuLogErr("failed to add requested event to retun buffer");
                }
            }
        } else if (filterResult == RPNFILTER_RESULT_ERROR) {
            safuLogErr("Error fetching event from in memory backend!");
            result = SAFU_RESULT_FAILED;
        }
        idx = this->indexIncrement(idx);
    } while (idx != this->end && result == SAFU_RESULT_OK);

    return result;
}
