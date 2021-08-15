/*
 * Copyright 2017 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_POWER_LOG_H
#define ANDROID_AUDIO_POWER_LOG_H

#ifdef __cplusplus

#include <mutex>
#include <vector>
#include <system/audio.h>
#include <utils/Errors.h>

namespace android {

/**
 * PowerLog captures the audio data power (measured in dBFS) over time.
 *
 * For the purposes of power evaluation, the audio data is divided into "bins",
 * and grouped by signals consisting of consecutive non-zero energy bins.
 * The sum energy in dB of each signal is computed for comparison purposes.
 *
 * No distinction is made between channels in an audio frame; they are all
 * summed together for energy purposes.
 *
 * The public methods are internally protected by a mutex to be thread-safe.
 */
class PowerLog {
public:
    /**
     * \brief Creates a PowerLog object.
     *
     * \param sampleRate        sample rate of the audio data.
     * \param channelCount      channel count of the audio data.
     * \param format            format of the audio data. It must be allowed by
     *                          audio_utils_is_compute_power_format_supported()
     *                          else the constructor will abort.
     * \param entries           total number of energy entries "bins" to use.
     * \param framesPerEntry    total number of audio frames used in each entry.
     */
    PowerLog(uint32_t sampleRate,
            uint32_t channelCount,
            audio_format_t format,
            size_t entries,
            size_t framesPerEntry);

    /**
     * \brief Adds new audio data to the power log.
     *
     * \param buffer            pointer to the audio data buffer.
     * \param frames            buffer size in audio frames.
     * \param nowNs             current time in nanoseconds.
     */
    void log(const void *buffer, size_t frames, int64_t nowNs);

    /**
     * \brief Dumps the log to a std::string.
     *
     * \param lines             maximum number of lines to output (0 disables).
     * \param limitNs           limit dump to data more recent than limitNs (0 disables).
     * \return the std::string for the log.
     */
    std::string dumpToString(
            const char *prefix = "", size_t lines = 0, int64_t limitNs = 0) const;

    /**
     * \brief Dumps the log to a raw file descriptor.
     *
     * \param fd                file descriptor to use.
     * \param lines             maximum number of lines to output (0 disables).
     * \param limitNs           limit dump to data more recent than limitNs (0 disables).
     * \return
     *   NO_ERROR on success or a negative number (-errno) on failure of write().
     */
    status_t dump(int fd, const char *prefix = "", size_t lines = 0, int64_t limitNs = 0) const;

private:
    mutable std::mutex mLock;     // monitor mutex
    int64_t mCurrentTime;         // time of first frame in buffer
    float mCurrentEnergy;         // local energy accumulation
    size_t mCurrentFrames;        // number of frames in the energy
    size_t mIdx;                  // next usable index in mEntries
    size_t mConsecutiveZeroes;    // current run of consecutive zero entries
    const uint32_t mSampleRate;   // audio data sample rate
    const uint32_t mChannelCount; // audio data channel count
    const audio_format_t mFormat; // audio data format
    const size_t mFramesPerEntry; // number of audio frames per entry
    std::vector<std::pair<int64_t /* real time ns */, float /* energy */>> mEntries;
};

} // namespace android

#endif // __cplusplus

/** \cond */
__BEGIN_DECLS
/** \endcond */

// C API (see C++ api above for details)

typedef struct power_log_t power_log_t;

/**
 * \brief Creates a power log object.
 *
 * \param sample_rate       sample rate of the audio data.
 * \param channel_count     channel count of the audio data.
 * \param format            format of the audio data. It must be allowed by
 *                          audio_utils_is_compute_power_format_supported().
 * \param entries           total number of energy entries "bins" to use.
 * \param frames_per_entry  total number of audio frames used in each entry.
 *
 * \return power log object or NULL on failure.
 */
power_log_t *power_log_create(uint32_t sample_rate,
        uint32_t channel_count, audio_format_t format, size_t entries, size_t frames_per_entry);

/**
 * \brief Adds new audio data to the power log.
 *
 * \param power_log         object returned by create, if NULL nothing happens.
 * \param buffer            pointer to the audio data buffer.
 * \param frames            buffer size in audio frames.
 * \param now_ns            current time in nanoseconds.
 */
void power_log_log(power_log_t *power_log, const void *buffer, size_t frames, int64_t now_ns);

/**
 * \brief Dumps the log to a raw file descriptor.
 *
 * \param power_log         object returned by create, if NULL nothing happens.
 * \param fd                file descriptor to use.
 * \param prefix            displayed at start of each line.
 * \param lines             maximum number of lines to output (0 disables).
 * \param limit_ns          limit dump to data more recent than limit_ns (0 disables).
 * \return
 *   NO_ERROR on success or a negative number (-errno) on failure of write().
 *   if power_log is NULL, BAD_VALUE is returned.
 */
int power_log_dump(
        power_log_t *power_log, int fd, const char *prefix,  size_t lines, int64_t limit_ns);

/**
 * \brief Destroys the power log object.
 *
 * \param power_log         object returned by create, if NULL nothing happens.
 */
void power_log_destroy(power_log_t *power_log);

/** \cond */
__END_DECLS
/** \endcond */

#endif // !ANDROID_AUDIO_POWER_LOG_H
