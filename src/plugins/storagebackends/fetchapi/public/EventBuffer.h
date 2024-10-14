// SPDX-License-Identifier: MIT
#pragma once

#include <elos/event/event_types.h>
#include <elos/eventfilter/eventfilter.h>
#include <safu/result.h>
#include <safu/vector_types.h>

#include <memory>

class elosEventBuffer {
   private:
    size_t size;
    size_t start;
    size_t end;
    std::unique_ptr<elosEvent_t[]> buffer;

    size_t elosIndexIncrement(size_t idx) const noexcept;

   public:
    elosEventBuffer(size_t size) noexcept(false);
    ~elosEventBuffer() noexcept(false);
    elosEventBuffer(elosEventBuffer &buff) = delete;
    elosEventBuffer(elosEventBuffer &&buff) = delete;
    safuResultE_t elosPushEvent(const elosEvent_t &event) noexcept;
    safuResultE_t elosFindEvents(const elosRpnFilter_t &filter, const timespec &newest, const timespec &oldest,
                                 safuVec_t &eventList) const noexcept;
};
