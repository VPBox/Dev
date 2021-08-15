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

#ifndef ANDROID_AUDIO_ERROR_LOG_H
#define ANDROID_AUDIO_ERROR_LOG_H

#ifdef __cplusplus

#include <iomanip>
#include <mutex>
#include <sstream>
#include <unistd.h>
#include <vector>

#include <audio_utils/clock.h>
#include <utils/Errors.h>

namespace android {

/**
 * ErrorLog captures audio errors codes, combining consecutive identical error codes
 * (within a specified time) into a single entry (to reduce log spamming).
 *
 * The entry thus contains the number of consecutive error codes,
 * together with the first time the error code occurs and the last time the error code occurs.
 *
 * The type T represents the error code type and is an int32_t for the C API.
 */
template <typename T>
class ErrorLog {
public:
    /**
     * \brief Creates an ErrorLog object
     *
     * \param entries           the length of error history.
     * \param aggregateNs       the maximum time in nanoseconds between identical error codes
     *                          to be aggregated into a single entry.
     */
    explicit ErrorLog(size_t entries, int64_t aggregateNs = 1000000000 /* one second */)
        : mErrors(0)
        , mIdx(0)
        , mAggregateNs(aggregateNs)
        , mEntries(entries)
    {
    }

    /**
     * \brief Adds new error code to the error log.
     *
     * Consecutive errors with the same code will be aggregated
     * if they occur within aggregateNs.
     *
     * \param code              error code of type T.
     * \param nowNs             current time in nanoseconds.
     */
    void log(const T &code, int64_t nowNs)
    {
        std::lock_guard<std::mutex> guard(mLock);

        ++mErrors;

        // Within mAggregateNs (1 second by default), aggregate error codes together.
        if (code == mEntries[mIdx].mCode
                && nowNs - mEntries[mIdx].mLastTime < mAggregateNs) {
            mEntries[mIdx].mCount++;
            mEntries[mIdx].mLastTime = nowNs;
            return;
        }

        // Add new error entry.
        if (++mIdx >= mEntries.size()) {
            mIdx = 0;
        }
        mEntries[mIdx].setFirstError(code, nowNs);
    }

    /**
     * \brief Dumps the log to a std::string.
     * \param prefix            the prefix to use for each line
     *                          (generally a null terminated string of spaces).
     * \param lines             maximum number of lines to output (0 disables).
     * \param limitNs           limit dump to data more recent than limitNs (0 disables).
     * \return std::string of the dump.
     */
    std::string dumpToString(const char *prefix = "", size_t lines = 0, int64_t limitNs = 0) const
    {
        std::lock_guard<std::mutex> guard(mLock);

        std::stringstream ss;
        const size_t numberOfEntries = mEntries.size();
        const size_t headerLines = 2;

        if (lines == 0) {
            lines = SIZE_MAX;
        }
        ss << prefix << "Errors: " << mErrors << "\n";

        if (mErrors == 0 || lines <= headerLines) {
            return ss.str();
        }

        lines = std::min(lines - headerLines, numberOfEntries);
        // compute where to start dump log
        ssize_t offset;
        for (offset = 0; offset < (ssize_t)lines; ++offset) {
            const auto &entry =
                    mEntries[(mIdx + numberOfEntries - offset) % numberOfEntries];
            if (entry.mCount == 0 || entry.mLastTime < limitNs) {
                break;
            }
        }
        if (offset > 0) {
            offset--;
            ss << prefix << " Code  Freq          First time           Last time\n";
            for (; offset >= 0; --offset) {
                const auto &entry =
                        mEntries[(mIdx + numberOfEntries - offset) % numberOfEntries];

                ss << prefix << std::setw(5) <<  entry.mCode
                        << " " << std::setw(5) << entry.mCount
                        << "  " << audio_utils_time_string_from_ns(entry.mFirstTime).time
                        << "  " << audio_utils_time_string_from_ns(entry.mLastTime).time << "\n";
            }
        }
        return ss.str();
    }

    /**
     * \brief Dumps the log to a raw file descriptor.
     * \param fd                file descriptor to use.
     * \param prefix            the prefix to use for each line
     *                          (generally a null terminated string of spaces).
     * \param lines             maximum number of lines to output (0 disables).
     * \param limitNs           limit dump to data more recent than limitNs (0 disables).
     * \return
     *   NO_ERROR on success or a negative number (-errno) on failure of write().
     */
    status_t dump(int fd, const char *prefix = "", size_t lines = 0, int64_t limitNs = 0) const
    {
        // thread safe but not necessarily serial with respect to concurrent dumps to the same fd.
        const std::string s = dumpToString(prefix, lines, limitNs);
        if (s.size() > 0 && write(fd, s.c_str(), s.size()) < 0) {
            return -errno;
        }
        return NO_ERROR;
    }

    struct Entry {
        Entry()
            : mCode(0)
            , mCount(0)
            , mFirstTime(0)
            , mLastTime(0)
        {
        }

        // Initialize entry with code as the first error at the given time.
        void setFirstError(T code, int64_t time) {
            mCode = code;
            mCount = 1;
            mFirstTime = time;
            mLastTime = time;
        }

        T mCode;            // error code
        uint32_t mCount;    // number of consecutive errors of the same code.
        int64_t mFirstTime; // first time of the error code.
        int64_t mLastTime;  // last time of the error code.
    };

private:
    mutable std::mutex mLock;     // monitor mutex
    int64_t mErrors;              // total number of errors registered
    size_t mIdx;                  // current index into mEntries (active)
    const int64_t mAggregateNs;   // number of nanoseconds to aggregate consecutive error codes.
    std::vector<Entry> mEntries;  // circular buffer of error entries.
};

} // namespace android

#endif // __cplusplus

// C API (see C++ API above for details)

/** \cond */
__BEGIN_DECLS
/** \endcond */

typedef struct error_log_t error_log_t;

/**
 * \brief Creates an error log object
 *
 * \param entries           the length of error history.
 * \param aggregate_ns      the maximum time in nanoseconds between identical error codes
 *                          to be aggregated into a single entry.
 * \return the error log object or NULL on failure.
 */
error_log_t *error_log_create(size_t entries, int64_t aggregate_ns);

/**
 * \brief Adds new error code to the error log.
 *
 * Consecutive errors with the same code will be aggregated if
 * they occur within aggregate_ns.
 *
 * \param error_log         object returned by create, if NULL nothing happens.
 * \param code              error code of type T.
 * \param now_ns            current time in nanoseconds.
 */
void error_log_log(error_log_t *error_log, int32_t code, int64_t now_ns);

/**
 * \brief Dumps the log to a raw file descriptor.
 * \param error_log         object returned by create, if NULL nothing happens.
 * \param prefix            the prefix to use for each line
 *                          (generally a null terminated string of spaces).
 * \param fd                file descriptor to use.
 * \param lines             maximum number of lines to output (0 disables).
 * \param limit_ns          limit dump to data more recent than limit_ns (0 disables).
 * \return
 *   NO_ERROR on success or a negative number (-errno) on failure of write().
 *   if power_log is NULL, BAD_VALUE is returned.
 */
int error_log_dump(
        error_log_t *error_log, int fd, const char *prefix, size_t lines, int64_t limit_ns);

/**
 * \brief Destroys the error log object.
 *
 * \param error_log         object returned by create, if NULL nothing happens.
 */
void error_log_destroy(error_log_t *error_log);

/** \cond */
__END_DECLS
/** \endcond */

#endif // !ANDROID_AUDIO_ERROR_LOG_H
