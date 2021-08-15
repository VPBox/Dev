/*
**
** Copyright 2018, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef KEYSTORE_CONFIRMATIONUI_RATE_LIMITING_H_
#define KEYSTORE_CONFIRMATIONUI_RATE_LIMITING_H_

#include <android/hardware/confirmationui/1.0/types.h>
#include <chrono>
#include <stdint.h>
#include <sys/types.h>
#include <tuple>
#include <unordered_map>

namespace keystore {

using ConfirmationResponseCode = android::hardware::confirmationui::V1_0::ResponseCode;

using std::chrono::time_point;
using std::chrono::duration;

template <typename Clock = std::chrono::steady_clock> class RateLimiting {
  private:
    struct Slot {
        Slot() : previous_start{}, prompt_start{}, counter(0) {}
        typename Clock::time_point previous_start;
        typename Clock::time_point prompt_start;
        uint32_t counter;
    };

    std::unordered_map<uid_t, Slot> slots_;

    uint_t latest_requester_;

    static std::chrono::seconds getBackoff(uint32_t counter) {
        using namespace std::chrono_literals;
        switch (counter) {
        case 0:
        case 1:
        case 2:
            return 0s;
        case 3:
        case 4:
        case 5:
            return 30s;
        default:
            return 60s * (1ULL << (counter - 6));
        }
    }

  public:
    // Exposes the number of used slots. This is only used by the test to verify the assumption
    // about used counter slots.
    size_t usedSlots() const { return slots_.size(); }
    void doGC() {
        using namespace std::chrono_literals;
        using std::chrono::system_clock;
        using std::chrono::time_point_cast;
        auto then = Clock::now() - 24h;
        auto iter = slots_.begin();
        while (iter != slots_.end()) {
            if (iter->second.prompt_start <= then) {
                iter = slots_.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    bool tryPrompt(uid_t id) {
        using namespace std::chrono_literals;
        // remove slots that have not been touched in 24 hours
        doGC();
        auto& slot = slots_[id];
        auto now = Clock::now();
        if (!slot.counter || slot.prompt_start <= now - getBackoff(slot.counter)) {
            latest_requester_ = id;
            slot.counter += 1;
            slot.previous_start = slot.prompt_start;
            slot.prompt_start = now;
            return true;
        }
        return false;
    }

    void processResult(ConfirmationResponseCode rc) {
        switch (rc) {
        case ConfirmationResponseCode::OK:
            // reset the counter slot
            slots_.erase(latest_requester_);
            return;
        case ConfirmationResponseCode::Canceled:
            // nothing to do here
            return;
        default:;
        }

        // roll back latest request
        auto& slot = slots_[latest_requester_];
        if (slot.counter <= 1) {
            slots_.erase(latest_requester_);
            return;
        }
        slot.counter -= 1;
        slot.prompt_start = slot.previous_start;
    }
};

}  // namespace keystore

#endif  // KEYSTORE_CONFIRMATIONUI_RATE_LIMITING_H_
