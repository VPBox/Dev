/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_UTILS_BALANCE_H
#define ANDROID_AUDIO_UTILS_BALANCE_H

#include <math.h>       /* expf */
#include <sstream>
#include <system/audio.h>
#include <vector>

namespace android::audio_utils {

class Balance {
public:
   /**
     * \brief Balance processing of left-right volume on audio data.
     *
     * Allows processing of audio data with a single balance parameter from [-1, 1].
     * For efficiency, the class caches balance and channel mask data between calls;
     * hence, use by multiple threads will require caller locking.
     *
     * \param ramp whether to ramp volume or not.
     * \param curve a monotonic increasing function f: [0, 1] -> [a, b]
     *        which represents the volume steps from an input domain of [0, 1] to
     *        an output range [a, b] (ostensibly also from 0 to 1).
     *        If [a, b] is not [0, 1], it is normalized to [0, 1].
     *        Curve is typically a convex function, some possible examples:
     *        [](float x) { return expf(2.f * x); }
     *        or
     *        [](float x) { return x * (x + 0.2f); }
     */
    explicit Balance(
            bool ramp = true,
            std::function<float(float)> curve = [](float x) { return x * (x + 0.2f); })
        : mRamp(ramp)
        , mCurve(normalize(std::move(curve))) { }

    /**
     * \brief Sets whether the process ramps left-right volume changes.
     *
     * The default value is true.
     * The ramp will take place, if needed, on the following process()
     * using the current balance and volume as the starting point.
     *
     * Toggling ramp off and then back on will reset the ramp starting point.
     *
     * \param ramp whether ramping is used to smooth volume changes.
     */
    void setRamp(bool ramp) {
        if (ramp == mRamp) return; // no change
        mRamp = ramp;
        if (mRamp) { // use current volume and balance as starting point.
           mRampVolumes = mVolumes;
           mRampBalance = mBalance;
        }
    }

    /**
     * \brief Sets the channel mask for data passed in.
     *
     * setChannelMask() must called before process() to set
     * a valid output audio channel mask.
     *
     * \param channelMask the audio output channel mask to use.
     *                    Invalid channel masks are ignored.
     *
     */
    void setChannelMask(audio_channel_mask_t channelMask);


    /**
     * \brief Sets the left-right balance parameter.
     *
     * setBalance() should be called before process() to set
     * the balance.  The initial value is 0.f (no action).
     *
     * \param balance   from -1.f (left) to 0.f (center) to 1.f (right).
     *
     */
    void setBalance(float balance);

    /**
     * \brief Processes balance for audio data.
     *
     * setChannelMask() should be called at least once before calling process()
     * to set the channel mask.  A balance of 0.f or a channel mask of
     * less than 2 channels will return with the buffer untouched.
     *
     * \param buffer    pointer to the audio data to be modified in-place.
     * \param frames    number of frames of audio data to convert.
     *
     */
    void process(float *buffer, size_t frames);

    /**
     * \brief Computes the stereo gains for left and right channels.
     *
     * Implementation detail (may change):
     * This is not an energy preserving balance (e.g. using sin/cos cross fade or some such).
     * Rather balance preserves full gain on left and right when balance is 0.f,
     * and decreases the right or left as one changes the balance parameter.
     *
     * \param balance   from -1.f (left) to 0.f (center) to 1.f (right).
     * \param left      pointer to the float where the left gain will be stored.
     * \param right     pointer to the float where the right gain will be stored.
     */
    void computeStereoBalance(float balance, float *left, float *right) const;

    /**
     * \brief Creates a std::string representation of Balance object for logging.
     *
     * \return string representation of Balance object
     */
    std::string toString() const;

private:

    /**
     * \brief Normalizes f: [0, 1] -> [a, b] to g: [0, 1] -> [0, 1].
     *
     * A helper function to normalize a float volume function.
     * g(0) is exactly zero, but g(1) may not necessarily be 1 since we
     * use reciprocal multiplication instead of division to scale.
     *
     * \param f a function from [0, 1] -> [a, b]
     * \return g a function from [0, 1] -> [0, 1] as a linear function of f.
     */
    template<typename T>
    static std::function<T(T)> normalize(std::function<T(T)> f) {
        const T f0 = f(0);
        const T r = T(1) / (f(1) - f0); // reciprocal multiplication

        if (f0 != T(0) ||  // must be exactly 0 at 0, since we promise g(0) == 0
            fabs(r - T(1)) > std::numeric_limits<T>::epsilon() * 3) { // some fudge allowed on r.
            return [f, f0, r](T x) { return r * (f(x) - f0); };
        }
        // no translation required.
        return f;
    }

    // setBalance() changes mBalance and mVolumes based on the channel geometry information.
    float mBalance = 0.f;              // balance: -1.f (left), 0.f (center), 1.f (right)
    std::vector<float> mVolumes;       // per channel, the volume adjustment due to balance.

    // setChannelMask() updates mChannelMask, mChannelCount, and mSides to cache the geometry
    // and then calls setBalance() to update mVolumes.

    audio_channel_mask_t mChannelMask = AUDIO_CHANNEL_INVALID;
    size_t mChannelCount = 0;          // from mChannelMask, 0 means no processing done.

    std::vector<int> mSides;           // per channel, the side (0 = left, 1 = right, 2 = center)
                                       // only used for channel position masks.

    // Ramping variables
    bool mRamp;                       // whether ramp is enabled.
    float mRampBalance = 0.f;         // last (starting) balance to begin ramp.
    std::vector<float> mRampVolumes;  // last (starting) volumes to begin ramp, clear for no ramp.

    const std::function<float(float)> mCurve; // monotone volume transfer func [0, 1] -> [0, 1]
};

} // namespace android::audio_utils

#endif // !ANDROID_AUDIO_UTILS_BALANCE_H
