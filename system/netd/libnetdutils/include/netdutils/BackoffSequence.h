/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NETDUTILS_BACKOFFSEQUENCE_H
#define NETDUTILS_BACKOFFSEQUENCE_H

#include <stdint.h>
#include <algorithm>
#include <chrono>
#include <limits>

namespace android {
namespace netdutils {

// Encapsulate some RFC 3315 section 14 -style backoff mechanics.
//
//     https://tools.ietf.org/html/rfc3315#section-14
template<typename time_type = std::chrono::seconds, typename counter_type = uint32_t>
class BackoffSequence {
  public:
    struct Parameters {
        time_type initialRetransTime{TIME_UNITY};
        counter_type maxRetransCount{0U};
        time_type maxRetransTime{TIME_ZERO};
        time_type maxRetransDuration{TIME_ZERO};
        time_type endOfSequenceIndicator{TIME_ZERO};
    };

    BackoffSequence() {}
    BackoffSequence(const BackoffSequence &) = default;
    BackoffSequence(BackoffSequence &&) = default;
    BackoffSequence& operator=(const BackoffSequence &) = default;
    BackoffSequence& operator=(BackoffSequence &&) = default;

    bool hasNextTimeout() const noexcept {
        return !maxRetransCountExceed() && !maxRetransDurationExceeded();
    }

    // Returns 0 when the sequence is exhausted.
    time_type getNextTimeout() {
        if (!hasNextTimeout()) return getEndOfSequenceIndicator();

        mRetransTime = getNextTimeoutAfter(mRetransTime);

        mRetransCount++;
        mTotalRetransDuration += mRetransTime;
        return mRetransTime;
    }

    time_type getEndOfSequenceIndicator() const noexcept {
        return mParams.endOfSequenceIndicator;
    }

    class Builder {
      public:
        Builder() {}

        constexpr Builder& withInitialRetransmissionTime(time_type irt) {
            mParams.initialRetransTime = irt;
            return *this;
        }
        constexpr Builder& withMaximumRetransmissionCount(counter_type mrc) {
            mParams.maxRetransCount = mrc;
            return *this;
        }
        constexpr Builder& withMaximumRetransmissionTime(time_type mrt) {
            mParams.maxRetransTime = mrt;
            return *this;
        }
        constexpr Builder& withMaximumRetransmissionDuration(time_type mrd) {
            mParams.maxRetransDuration = mrd;
            return *this;
        }
        constexpr Builder& withEndOfSequenceIndicator(time_type eos) {
            mParams.endOfSequenceIndicator = eos;
            return *this;
        }

        constexpr BackoffSequence build() const {
            return BackoffSequence(mParams);
        }

      private:
        Parameters mParams;
    };

  private:
    static constexpr int PER_ITERATION_SCALING_FACTOR = 2;
    static constexpr time_type TIME_ZERO = time_type();
    static constexpr time_type TIME_UNITY = time_type(1);

    constexpr BackoffSequence(const struct Parameters &params)
            : mParams(params),
              mRetransCount(0),
              mRetransTime(TIME_ZERO),
              mTotalRetransDuration(TIME_ZERO) {}

    constexpr bool maxRetransCountExceed() const {
        return (mParams.maxRetransCount > 0) && (mRetransCount >= mParams.maxRetransCount);
    }

    constexpr bool maxRetransDurationExceeded() const {
        return (mParams.maxRetransDuration > TIME_ZERO) &&
               (mTotalRetransDuration >= mParams.maxRetransDuration);
    }

    time_type getNextTimeoutAfter(time_type lastTimeout) const {
        // TODO: Support proper random jitter. Also, consider supporting some
        // per-iteration scaling factor other than doubling.
        time_type nextTimeout = (lastTimeout > TIME_ZERO)
                ? PER_ITERATION_SCALING_FACTOR * lastTimeout
                : mParams.initialRetransTime;

        // Check if overflow occurred.
        if (nextTimeout < lastTimeout) {
            nextTimeout = std::numeric_limits<time_type>::max();
        }

        // Cap to maximum allowed, if necessary.
        if (mParams.maxRetransTime > TIME_ZERO) {
            nextTimeout = std::min(nextTimeout, mParams.maxRetransTime);
        }

        // Don't overflow the maximum total duration.
        if (mParams.maxRetransDuration > TIME_ZERO) {
            nextTimeout = std::min(nextTimeout, mParams.maxRetransDuration - lastTimeout);
        }
        return nextTimeout;
    }

    const Parameters mParams;
    counter_type mRetransCount;
    time_type mRetransTime;
    time_type mTotalRetransDuration;
};

}  // namespace netdutils
}  // namespace android

#endif /* NETDUTILS_BACKOFFSEQUENCE_H */
