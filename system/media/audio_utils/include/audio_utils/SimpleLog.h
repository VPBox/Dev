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

#ifndef ANDROID_AUDIO_SIMPLE_LOG_H
#define ANDROID_AUDIO_SIMPLE_LOG_H

#include <deque>
#include <mutex>
#include <sstream>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include <utils/Errors.h>

#include <audio_utils/clock.h>

namespace android {

/**
 * SimpleLog provides a private logcat-style logging to avoid cluttering
 * the device logcat.
 *
 * The public methods are internally protected by a mutex to be thread-safe.
 * Do not call from a sched_fifo thread as it can use a system time call
 * and obtains a local mutex.
 *
 * Formatted logs by log() and logv() will be truncated at kMaxStringLength - 1
 * due to null termination. logs() does not have a string length limitation.
 */

class SimpleLog {
public:
    /**
     * \brief Creates a SimpleLog object.
     *
     * \param maxLogLines the maximum number of log lines.
     */
    explicit SimpleLog(size_t maxLogLines = kDefaultMaxLogLines)
        : mMaxLogLines(maxLogLines)
    {
    }

    /**
     * \brief Adds a formatted string into the log.
     *
     * Time is automatically associated with the string by audio_utils_get_real_time_ns().
     *
     * \param format            the format string, similar to printf().
     *
     * and optional arguments.
     */
    // using C++11 unified attribute syntax; index is offset by 1 for implicit "this".
    [[gnu::format(printf, 2 /* string-index */, 3 /* first-to-check */)]]
    void log(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        // use -1 to trigger the clock fetch within the mutex lock.
        logv(-1 /* nowNs */, format, args);
        va_end(args);
    }

    /**
     * \brief Adds a formatted string into the log with time.
     *
     * \param nowNs             the time to use for logging. Assumed to be monotonically
     *                          increasing for sequential calls.  If -1, then
     *                          audio_utils_get_real_time_ns() is called.
     * \param format            the format string, similar to printf().
     *
     * and optional arguments.
     */
    // using C++11 unified attribute syntax; index is offset by 1 for implicit "this".
    [[gnu::format(printf, 3 /* string-index */, 4 /* first-to-check */)]]
    void log(int64_t nowNs, const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        logv(nowNs, format, args);
        va_end(args);
    }

    /**
     * \brief Adds a formatted string by va_list with time.  Not intended for typical use.
     *
     * \param nowNs             the time to use for logging. Assumed to be monotonically
     *                          increasing for sequential calls.  If -1, then
     *                          audio_utils_get_real_time_ns() is called.
     * \param format            the format string, similar to printf().
     * \param args              va_list args.
     */
    void logv(int64_t nowNs, const char *format, va_list args)
    {
        // format to buffer
        char buffer[kMaxStringLength];
        int length = vsnprintf(buffer, sizeof(buffer), format, args);
        if (length < 0) { // encoding error
            logs(nowNs, "invalid format");
            return;
        } else if (length >= (signed)sizeof(buffer)) {
            length = sizeof(buffer) - 1;
        }

        // strip out trailing newlines
        while (length > 0 && buffer[length - 1] == '\n') {
            buffer[--length] = '\0';
        }
        logs(nowNs, buffer);
    }

    /**
     * \brief Logs a string to the buffer with time.
     * \param nowNs             the time to use for logging. Assumed to be monotonically
     *                          increasing for sequential calls.  If -1, then
     *                          audio_utils_get_real_time_ns() is called.
     * \param buffer            contains a null terminated string, which may have
     *                          special characters such as % and \ that are
     *                          not interpreted.
     *                          This could be a char * or a std::string.
     */
    template <typename U>
    void logs(int64_t nowNs, U&& buffer)
    {
        // store in circular array
        std::lock_guard<std::mutex> guard(mLock);
        if (nowNs == -1) {
            nowNs = audio_utils_get_real_time_ns();
        }
        mLog.emplace_back(nowNs, std::forward<U>(buffer));
        if (mLog.size() > mMaxLogLines) {
            mLog.pop_front();
        }
    }

    /**
     * \brief Dumps the log to a string.
     *
     * \param prefix            the prefix to use for each line
     *                          (generally a null terminated string of spaces).
     * \param lines             maximum number of lines to output (0 disables).
     * \param limitNs           limit dump to data more recent than limitNs (0 disables).
     * \return a string object for the log.
     */
    std::string dumpToString(const char *prefix = "", size_t lines = 0, int64_t limitNs = 0) const
    {
        if (lines == 0) {
            lines = mLog.size();
        }

        std::stringstream ss;
        std::lock_guard<std::mutex> guard(mLock);
        auto it = mLog.begin();

        // Note: this restricts the lines before checking the time constraint.
        if (mLog.size() > lines) {
            it += (mLog.size() - lines);
        }
        for (; it != mLog.end(); ++it) {
            const int64_t time = it->first;
            if (time < limitNs) continue;  // too old
            ss << prefix << audio_utils_time_string_from_ns(time).time
                    << " " << it->second.c_str() << "\n";
        }
        return ss.str();
    }

    /**
     * \brief Dumps the log to a raw file descriptor.
     *
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
        // dumpToString() and write() are individually thread-safe, but concurrent threads
        // using dump() to the same file descriptor may write out of order.
        const std::string s = dumpToString(prefix, lines, limitNs);
        if (s.size() > 0 && write(fd, s.c_str(), s.size()) < 0) {
            return -errno;
        }
        return NO_ERROR;
    }

private:
    mutable std::mutex mLock;
    static const size_t kMaxStringLength = 1024;  // maximum formatted string length
    static const size_t kDefaultMaxLogLines = 80; // default maximum log history

    const size_t mMaxLogLines;                    // maximum log history
    std::deque<std::pair<int64_t, std::string>> mLog; // circular buffer is backed by deque.
};

} // namespace android

#endif // !ANDROID_AUDIO_SIMPLE_LOG_H
