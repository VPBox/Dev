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

#include <audio_utils/Balance.h>

namespace android::audio_utils {

void Balance::setChannelMask(audio_channel_mask_t channelMask)
{
    channelMask &= ~ AUDIO_CHANNEL_HAPTIC_ALL;
    if (!audio_is_output_channel(channelMask) // invalid mask
            || mChannelMask == channelMask) { // no need to do anything
        return;
    }

    mChannelMask = channelMask;
    mChannelCount = audio_channel_count_from_out_mask(channelMask);

    // save mBalance into balance for later restoring, then reset
    const float balance = mBalance;
    mBalance = 0.f;

    // reset mVolumes
    mVolumes.resize(mChannelCount);
    std::fill(mVolumes.begin(), mVolumes.end(), 1.f);

    // reset ramping variables
    mRampBalance = 0.f;
    mRampVolumes.clear();

    if (audio_channel_mask_get_representation(mChannelMask)
            == AUDIO_CHANNEL_REPRESENTATION_INDEX) {
        mSides.clear();       // mSides unused for channel index masks.
        setBalance(balance);  // recompute balance
        return;
    }

    // Implementation detail (may change):
    // For implementation speed, we precompute the side (left, right, center),
    // which is a fixed geometrical constant for a given channel mask.
    // This assumes that the channel mask does not change frequently.
    //
    // For the channel mask spec, see system/media/audio/include/system/audio-base.h.
    //
    // The side is: 0 = left, 1 = right, 2 = center.
    static constexpr int sideFromChannel[] = {
        0, // AUDIO_CHANNEL_OUT_FRONT_LEFT            = 0x1u,
        1, // AUDIO_CHANNEL_OUT_FRONT_RIGHT           = 0x2u,
        2, // AUDIO_CHANNEL_OUT_FRONT_CENTER          = 0x4u,
        2, // AUDIO_CHANNEL_OUT_LOW_FREQUENCY         = 0x8u,
        0, // AUDIO_CHANNEL_OUT_BACK_LEFT             = 0x10u,
        1, // AUDIO_CHANNEL_OUT_BACK_RIGHT            = 0x20u,
        0, // AUDIO_CHANNEL_OUT_FRONT_LEFT_OF_CENTER  = 0x40u,
        1, // AUDIO_CHANNEL_OUT_FRONT_RIGHT_OF_CENTER = 0x80u,
        2, // AUDIO_CHANNEL_OUT_BACK_CENTER           = 0x100u,
        0, // AUDIO_CHANNEL_OUT_SIDE_LEFT             = 0x200u,
        1, // AUDIO_CHANNEL_OUT_SIDE_RIGHT            = 0x400u,
        2, // AUDIO_CHANNEL_OUT_TOP_CENTER            = 0x800u,
        0, // AUDIO_CHANNEL_OUT_TOP_FRONT_LEFT        = 0x1000u,
        2, // AUDIO_CHANNEL_OUT_TOP_FRONT_CENTER      = 0x2000u,
        1, // AUDIO_CHANNEL_OUT_TOP_FRONT_RIGHT       = 0x4000u,
        0, // AUDIO_CHANNEL_OUT_TOP_BACK_LEFT         = 0x8000u,
        2, // AUDIO_CHANNEL_OUT_TOP_BACK_CENTER       = 0x10000u,
        1, // AUDIO_CHANNEL_OUT_TOP_BACK_RIGHT        = 0x20000u,
        0, // AUDIO_CHANNEL_OUT_TOP_SIDE_LEFT         = 0x40000u,
        1, // AUDIO_CHANNEL_OUT_TOP_SIDE_RIGHT        = 0x80000u,
     };

    mSides.resize(mChannelCount);
    for (unsigned i = 0, channel = channelMask; channel != 0; ++i) {
        const int index = __builtin_ctz(channel);
        if (index < std::size(sideFromChannel)) {
            mSides[i] = sideFromChannel[index];
        } else {
            mSides[i] = 2; // consider center
        }
        channel &= ~(1 << index);
    }
    setBalance(balance); // recompute balance
}

void Balance::process(float *buffer, size_t frames)
{
    if (mBalance == 0.f || mChannelCount < 2) {
        return;
    }

    if (mRamp) {
        if (mRampVolumes.size() != mVolumes.size()) {
            // If mRampVolumes is empty, we do not ramp in this process() but directly
            // apply the existing mVolumes. We save the balance and volume state here
            // and fall through to non-ramping code below. The next process() will ramp if needed.
            mRampBalance = mBalance;
            mRampVolumes = mVolumes;
        } else if (mRampBalance != mBalance) {
            if (frames > 0) {
                std::vector<float> mDeltas(mVolumes.size());
                const float r = 1.f / frames;
                for (size_t j = 0; j < mChannelCount; ++j) {
                    mDeltas[j] = (mVolumes[j] - mRampVolumes[j]) * r;
                }

                // ramped balance
                for (size_t i = 0; i < frames; ++i) {
                    const float findex = i;
                    for (size_t j = 0; j < mChannelCount; ++j) { // better precision: delta * i
                        *buffer++ *= mRampVolumes[j] + mDeltas[j] * findex;
                    }
                }
            }
            mRampBalance = mBalance;
            mRampVolumes = mVolumes;
            return;
        }
        // fall through
    }

    // non-ramped balance
    for (size_t i = 0; i < frames; ++i) {
        for (size_t j = 0; j < mChannelCount; ++j) {
            *buffer++ *= mVolumes[j];
        }
    }
}

void Balance::computeStereoBalance(float balance, float *left, float *right) const
{
    if (balance > 0.f) {
        *left = mCurve(1.f - balance);
        *right = 1.f;
    } else if (balance < 0.f) {
        *left = 1.f;
        *right = mCurve(1.f + balance);
    } else {
        *left = 1.f;
        *right = 1.f;
    }

    // Functionally:
    // *left = balance > 0.f ? mCurve(1.f - balance) : 1.f;
    // *right = balance < 0.f ? mCurve(1.f + balance) : 1.f;
}

std::string Balance::toString() const
{
    std::stringstream ss;
    ss << "balance " << mBalance << " channelCount " << mChannelCount << " volumes:";
    for (float volume : mVolumes) {
        ss << " " << volume;
    }
    // we do not show mSides, which is only valid for channel position masks.
    return ss.str();
}

void Balance::setBalance(float balance)
{
    if (mBalance == balance                         // no change
        || isnan(balance) || fabs(balance) > 1.f) { // balance out of range
        return;
    }

    mBalance = balance;

    if (mChannelCount < 2) { // if channel count is 1, mVolumes[0] is already set to 1.f
        return;              // and if channel count < 2, we don't do anything in process().
    }

    // Handle the common cases:
    // stereo and channel index masks only affect the first two channels as left and right.
    if (mChannelMask == AUDIO_CHANNEL_OUT_STEREO
            || audio_channel_mask_get_representation(mChannelMask)
                    == AUDIO_CHANNEL_REPRESENTATION_INDEX) {
        computeStereoBalance(balance, &mVolumes[0], &mVolumes[1]);
        return;
    }

    // For position masks with more than 2 channels, we consider which side the
    // speaker position is on to figure the volume used.
    float balanceVolumes[3]; // left, right, center
    computeStereoBalance(balance, &balanceVolumes[0], &balanceVolumes[1]);
    balanceVolumes[2] = 1.f; // center  TODO: consider center scaling.

    for (size_t i = 0; i < mVolumes.size(); ++i) {
        mVolumes[i] = balanceVolumes[mSides[i]];
    }
}

} // namespace android::audio_utils
