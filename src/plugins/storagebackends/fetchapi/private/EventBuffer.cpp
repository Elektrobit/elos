// SPDX-License-Identifier: MIT

#include "public/EventBuffer.h"

#include <elos/event/event.h>
#include <elos/event/event_vector.h>
#include <elos/eventfilter/eventfilter.h>
#include <elos/eventfilter/eventfilter_types.h>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/vector.h>

#include <cstdlib>
#include <new>

elosEventBuffer::elosEventBuffer(size_t len) : size(len), start(0), end(SIZE_MAX), buffer(new elosEvent_t[len]) {
    for (size_t idx = 0; idx < size; idx++) {
        safuResultE_t result = elosEventInitialize(&this->buffer[idx]);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("initializing elosEventBuffer failed!");
            throw SAFU_RESULT_FAILED;
        }
    }
}
elosEventBuffer::~elosEventBuffer() noexcept(false) {
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
size_t elosEventBuffer::elosIndexIncrement(size_t idx) const noexcept {
    return (idx + 1) % this->size;
}
safuResultE_t elosEventBuffer::elosPushEvent(const elosEvent_t &event) noexcept {
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
            this->start = this->elosIndexIncrement(this->start);
        }
        this->end = this->elosIndexIncrement(this->end);
    }
    return result;
}
safuResultE_t elosEventBuffer::elosFindEvents(const elosEventFilter_t &filter, safuVec_t &eventList) const noexcept {
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
            result = elosEventVectorPushDeepCopy(&eventList, &this->buffer[idx]);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("failed to add event for fetch result list");
            }
        } else if (filterResult == RPNFILTER_RESULT_ERROR) {
            safuLogErr("Error fetching event from in memory backend!");
            result = SAFU_RESULT_FAILED;
        }
        idx = this->elosIndexIncrement(idx);
    } while (idx != this->end && result == SAFU_RESULT_OK);

    return result;
}
