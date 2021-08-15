/*
 * Copyright 2018 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_UTILS_TIMESTAMP_VERIFIER_H
#define ANDROID_AUDIO_UTILS_TIMESTAMP_VERIFIER_H

#include <audio_utils/clock.h>
#include <audio_utils/Statistics.h>

namespace android {

/** Verifies that a sequence of timestamps (a frame count, time pair)
 * is consistent based on sample rate.
 *
 * F is the type of frame counts (for example int64_t)
 * T is the type of time in Ns (for example int64_t ns)
 *
 * All methods except for toString() are safe to call from a SCHED_FIFO thread.
 */
template <typename F /* frame count */, typename T /* time units */>
class TimestampVerifier {
public:
    explicit constexpr TimestampVerifier(
            double alphaJitter = kDefaultAlphaJitter,
            double alphaEstimator = kDefaultAlphaEstimator)
        : mJitterMs{alphaJitter}
        , mTimestampEstimator{alphaEstimator}
        , mCorrectedJitterMs{alphaJitter}
    { }

    // construct from static arrays.
    template <size_t N>
    constexpr TimestampVerifier(const F (&frames)[N], const T (&timeNs)[N], uint32_t sampleRate) {
         for (size_t i = 0; i < N; ++i) {
             add(frames[i], timeNs[i], sampleRate);
         }
    }

    /** adds a timestamp, represented by a (frames, timeNs) pair and the
     * sample rate to the TimestampVerifier.
     *
     * The frames and timeNs should be monotonic increasing
     * (from the previous discontinuity, or the start of adding).
     *
     * A sample rate change triggers a discontinuity automatically.
     */
    constexpr void add(F frames, T timeNs, uint32_t sampleRate) {
        // We consider negative time as "not ready".
        // TODO: do we need to consider an explicit epoch start time?
        if (timeNs < 0) {
           ++mNotReady;
           return;
        }
        if (mDiscontinuity || mSampleRate != sampleRate) {
            // ALOGD("disc:%d frames:%lld timeNs:%lld",
            //         mDiscontinuity, (long long)frames, (long long)timeNs);
            switch (mDiscontinuityMode) {
            case DISCONTINUITY_MODE_CONTINUOUS:
                break;
            case DISCONTINUITY_MODE_ZERO:
                // frame position reset to 0 on discontinuity - detect this.
                if (mLastTimestamp.mFrames
                        > kDiscontinuityZeroStartThresholdMs * sampleRate / MILLIS_PER_SECOND
                        && frames >= mLastTimestamp.mFrames) {
                    return;
                }
                break;
            default:
                assert(false); // never here.
                break;
            }
            mDiscontinuity = false;
            mFirstTimestamp = {frames, timeNs};
            mLastTimestamp = mFirstTimestamp;
            mSampleRate = sampleRate;
        } else {
            assert(sampleRate != 0);
            const FrameTime timestamp{frames, timeNs};
            if (mCold && (timestamp.mTimeNs == mLastTimestamp.mTimeNs
                    || computeRatio(timestamp, mLastTimestamp, sampleRate)
                            < kMinimumSpeedToStartVerification)) {
                // cold is when the timestamp may take some time to start advancing at normal rate.
                ++mColds;
                mFirstTimestamp = timestamp;
                // ALOGD("colds:%lld frames:%lld timeNs:%lld",
                //         (long long)mColds, (long long)frames, (long long)timeNs);
            } else {
                const double jitterMs = computeJitterMs(timestamp, mLastTimestamp, sampleRate);
                mJitterMs.add(jitterMs);
                // ALOGD("frames:%lld  timeNs:%lld jitterMs:%lf",
                //         (long long)frames, (long long)timeNs, jitterMs);

                // Handle timestamp estimation
                if (mCold) {
                    mCold = false;
                    mTimestampEstimator.reset();
                    mTimestampEstimator.add(
                        {(double)mFirstTimestamp.mTimeNs * 1e-9, (double)mFirstTimestamp.mFrames});
                    mFirstCorrectedTimestamp = mFirstTimestamp;
                    mLastCorrectedTimestamp = mFirstCorrectedTimestamp;
                }
                mTimestampEstimator.add({(double)timeNs * 1e-9, (double)frames});

                // Find the corrected timestamp, a posteriori estimate.
                FrameTime correctedTimestamp = timestamp;

                // The estimator is valid after 2 timestamps; we choose correlation
                // of kEstimatorR2Lock to signal locked.
                if (mTimestampEstimator.getN() > 2
                        && mTimestampEstimator.getR2() >= kEstimatorR2Lock) {
#if 1
                    // We choose frame correction over time correction.
                    // TODO: analyze preference.

                    // figure out frames based on time.
                    const F newFrames = mTimestampEstimator.getYFromX((double)timeNs * 1e-9);
                    // prevent retrograde correction.
                    correctedTimestamp.mFrames = std::max(
                            newFrames, mLastCorrectedTimestamp.mFrames);
#else
                    // figure out time based on frames
                    const T newTimeNs = mTimestampEstimator.getXFromY((double)frames) * 1e9;
                    // prevent retrograde correction.
                    correctedTimestamp.mTimeNs = std::max(
                            newTimeNs, mLastCorrectedTimestamp.mTimeNs);
#endif
                }

                // Compute the jitter if the corrected timestamp is used.
                const double correctedJitterMs = computeJitterMs(
                        correctedTimestamp, mLastCorrectedTimestamp, sampleRate);
                mCorrectedJitterMs.add(correctedJitterMs);
                mLastCorrectedTimestamp = correctedTimestamp;
            }
            mLastTimestamp = timestamp;
        }
        ++mTimestamps;
    }

    /** registers a discontinuity.
     *
     * The next timestamp added does not participate in any statistics with the last
     * timestamp, but rather anchors following timestamp sequence verification.
     *
     * Consecutive discontinuities are treated as one for the purposes of counting.
     */
    constexpr void discontinuity() {
        if (!mDiscontinuity) {
            // ALOGD("discontinuity");
            mDiscontinuity = true;
            mCold = true;
            ++mDiscontinuities;
        }
    }

    /** registers an error.
     *
     * The timestamp sequence is still assumed continuous after error. Use discontinuity()
     * if it is not.
     */
    constexpr void error() {
        ++mErrors;
    }

    // How a discontinuity affects frame position.
    enum DiscontinuityMode : int32_t {
        DISCONTINUITY_MODE_CONTINUOUS, // frame position is unaffected.
        DISCONTINUITY_MODE_ZERO,       // frame position resets to zero.
    };

    constexpr void setDiscontinuityMode(DiscontinuityMode mode) {
        assert(mode == DISCONTINUITY_MODE_CONTINUOUS
                || mode == DISCONTINUITY_MODE_ZERO);
        mDiscontinuityMode = mode;
    }

    constexpr DiscontinuityMode getDiscontinuityMode() const {
        return mDiscontinuityMode;
    }

    /** returns a string with relevant statistics.
     *
     * Should not be called from a SCHED_FIFO thread since it uses std::string.
     */
    std::string toString() const {
        std::stringstream ss;

        ss << "n=" << mTimestamps;          // number of timestamps added with valid times.
        ss << " disc=" << mDiscontinuities; // discontinuities encountered (dups ignored).
        ss << " cold=" << mColds;           // timestamps not progressing after discontinuity.
        ss << " nRdy=" << mNotReady;        // timestamps not ready (time negative).
        ss << " err=" << mErrors;           // errors encountered.
        if (mSampleRate != 0) {             // ratio of time-by-frames / time
            ss << " rate=" << computeRatio( // (since last discontinuity).
                    mLastTimestamp, mFirstTimestamp, mSampleRate);
        }
        ss << " jitterMs(" << mJitterMs.toString() << ")";  // timestamp jitter statistics.

        double a, b, r2; // sample rate is the slope b.
        estimateSampleRate(a, b, r2);

        // r2 is correlation coefficient (where 1 is best and 0 is worst),
        // so for better printed resolution, we print from 1 - r2.
        ss << " localSR(" << b << ", " << (1. - r2) << ")";
        ss << " correctedJitterMs(" << mCorrectedJitterMs.toString() << ")";
        return ss.str();
    }

    // general counters
    constexpr int64_t getN() const { return mTimestamps; }
    constexpr int64_t getDiscontinuities() const { return mDiscontinuities; }
    constexpr int64_t getNotReady() const { return mNotReady; }
    constexpr int64_t getColds() const { return mColds; }
    constexpr int64_t getErrors() const { return mErrors; }
    constexpr const audio_utils::Statistics<double> & getJitterMs() const {
        return mJitterMs;
    }
    // estimate local sample rate (dframes / dtime) which is the slope b from:
    // y = a + bx
    constexpr void estimateSampleRate(double &a, double &b, double &r2) const {
        mTimestampEstimator.computeYLine(a, b, r2);
    }

    // timestamp anchor info
    using FrameTime = struct { F mFrames; T mTimeNs; }; // a "constexpr" pair
    constexpr FrameTime getFirstTimestamp() const { return mFirstTimestamp; }
    constexpr FrameTime getLastTimestamp() const { return mLastTimestamp; }
    constexpr uint32_t getSampleRate() const { return mSampleRate; }

    constexpr FrameTime getLastCorrectedTimestamp() const { return mLastCorrectedTimestamp; }

    // Inf+-, NaN is possible only if sampleRate is 0 (should not happen)
    static constexpr double computeJitterMs(
            const FrameTime &current, const FrameTime &last, uint32_t sampleRate) {
        const auto diff = sub(current, last);
        const double frameDifferenceNs = diff.first * 1e9 / sampleRate;
        const double jitterNs = diff.second - frameDifferenceNs;  // actual - expected
        return jitterNs * 1e-6;
    }

private:
    // our statistics have exponentially weighted history.
    // the defaults are here.
    static constexpr double kDefaultAlphaJitter = 0.999;
    static constexpr double kDefaultAlphaEstimator = 0.99;
    static constexpr double kEstimatorR2Lock = 0.95;

    // general counters
    int64_t mTimestamps = 0;
    int64_t mDiscontinuities = 0;
    int64_t mNotReady = 0;
    int64_t mColds = 0;
    int64_t mErrors = 0;
    audio_utils::Statistics<double> mJitterMs{kDefaultAlphaJitter};

    // timestamp anchor info
    bool mDiscontinuity = true;
    bool mCold = true;
    FrameTime mFirstTimestamp{};
    FrameTime mLastTimestamp{};
    uint32_t mSampleRate = 0;

    // timestamp estimation and correction
    audio_utils::LinearLeastSquaresFit<double> mTimestampEstimator{kDefaultAlphaEstimator};
    FrameTime mFirstCorrectedTimestamp{};
    FrameTime mLastCorrectedTimestamp{};
    audio_utils::Statistics<double> mCorrectedJitterMs{kDefaultAlphaJitter};

    // configuration
    DiscontinuityMode mDiscontinuityMode = DISCONTINUITY_MODE_CONTINUOUS;

    static constexpr double kMinimumSpeedToStartVerification = 0.1;
     // Number of ms so small that initial jitter is OK for DISCONTINUITY_MODE_ZERO.
    static constexpr int64_t kDiscontinuityZeroStartThresholdMs = 5;

    // sub returns the signed type of the difference between left and right.
    // This is only important if F or T are unsigned int types.
    __attribute__((no_sanitize("integer")))
    static constexpr auto sub(const FrameTime &left, const FrameTime &right) {
        return std::make_pair<
                typename std::make_signed<F>::type, typename std::make_signed<T>::type>(
                        left.mFrames - right.mFrames, left.mTimeNs - right.mTimeNs);
    }

    // Inf+-, Nan possible depending on differences between current and last.
    static constexpr double computeRatio(
            const FrameTime &current, const FrameTime &last, uint32_t sampleRate) {
        const auto diff = sub(current, last);
        const double frameDifferenceNs = diff.first * 1e9 / sampleRate;
        return frameDifferenceNs / diff.second;
    }
};

} // namespace android

#endif // !ANDROID_AUDIO_UTILS_TIMESTAMP_VERIFIER_H
