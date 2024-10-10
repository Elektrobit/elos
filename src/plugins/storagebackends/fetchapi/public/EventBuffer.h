// SPDX-License-Identifier: MIT
#pragma once

#include <elos/event/event_types.h>
#include <elos/eventfilter/eventfilter.h>
#include <safu/result.h>
#include <safu/vector_types.h>

#include <memory>

namespace elos {
namespace fetchapi {

class EventBuffer {
   private:
    size_t size;
    size_t start;
    size_t end;
    std::unique_ptr<elosEvent_t[]> buffer;

    size_t indexIncrement(size_t idx) const noexcept;

   public:
    EventBuffer(size_t size) noexcept(false);
    ~EventBuffer() noexcept(false);
    EventBuffer(EventBuffer &buff) = delete;
    EventBuffer(EventBuffer &&buff) = delete;
    safuResultE_t pushEvent(const elosEvent_t &event) noexcept;
    safuResultE_t findEvents(const elosRpnFilter_t &filter, const timespec &newest, const timespec &oldest,
                             safuVec_t &eventList) const noexcept;
};
}  // namespace fetchapi
}  // namespace elos
