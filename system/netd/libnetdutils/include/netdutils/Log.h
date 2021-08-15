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

#ifndef NETUTILS_LOG_H
#define NETUTILS_LOG_H

#include <chrono>
#include <deque>
#include <shared_mutex>
#include <string>
#include <type_traits>
#include <vector>

#include <android-base/stringprintf.h>
#include <android-base/thread_annotations.h>

#include <netdutils/Status.h>

namespace android {
namespace netdutils {

class LogEntry {
  public:
    LogEntry() = default;
    LogEntry(const LogEntry&) = default;
    LogEntry(LogEntry&&) = default;
    ~LogEntry() = default;
    LogEntry& operator=(const LogEntry&) = default;
    LogEntry& operator=(LogEntry&&) = default;

    std::string toString() const;

    ///
    // Helper methods that make it easy to build up a LogEntry message.
    // If performance becomes a factor the implementations could be inlined.
    ///
    LogEntry& message(const std::string& message);

    // For calling with __FUNCTION__.
    LogEntry& function(const std::string& function_name);
    // For calling with __PRETTY_FUNCTION__.
    LogEntry& prettyFunction(const std::string& pretty_function);

    // Convenience methods for each of the common types of function arguments.
    LogEntry& arg(const std::string& val);
    // Intended for binary buffers, formats as hex
    LogEntry& arg(const std::vector<uint8_t>& val);
    LogEntry& arg(const std::vector<int32_t>& val);
    LogEntry& arg(const std::vector<std::string>& val);
    template <typename IntT, typename = std::enable_if_t<std::is_arithmetic_v<IntT>>>
    LogEntry& arg(IntT val) {
        mArgs.push_back(std::to_string(val));
        return *this;
    }
    // Not using a plain overload here to avoid the implicit conversion from
    // any pointer to bool, which causes string literals to print as 'true'.
    template <>
    LogEntry& arg<>(bool val);

    template <typename... Args>
    LogEntry& args(const Args&... a) {
        // Cleverness ahead: we throw away the initializer_list filled with
        // zeroes, all we care about is calling arg() for each argument.
        (void) std::initializer_list<int>{(arg(a), 0)...};
        return *this;
    }

    // Some things can return more than one value, or have multiple output
    // parameters, so each of these adds to the mReturns vector.
    LogEntry& returns(const std::string& rval);
    LogEntry& returns(const Status& status);
    LogEntry& returns(bool rval);
    template <class T>
    LogEntry& returns(T val) {
        mReturns.push_back(std::to_string(val));
        return *this;
    }

    LogEntry& withUid(uid_t uid);

    // Append the duration computed since the creation of this instance.
    LogEntry& withAutomaticDuration();
    // Append the string-ified duration computed by some other means.
    LogEntry& withDuration(const std::string& duration);

  private:
    std::chrono::steady_clock::time_point mStart = std::chrono::steady_clock::now();
    std::string mMsg{};
    std::string mFunc{};
    std::vector<std::string> mArgs{};
    std::vector<std::string> mReturns{};
    std::string mUid{};
    std::string mDuration{};
};

class Log {
  public:
    Log() = delete;
    Log(const std::string& tag) : Log(tag, MAX_ENTRIES) {}
    Log(const std::string& tag, size_t maxEntries) : mTag(tag), mMaxEntries(maxEntries) {}
    Log(const Log&) = delete;
    Log(Log&&) = delete;
    ~Log();
    Log& operator=(const Log&) = delete;
    Log& operator=(Log&&) = delete;

    LogEntry newEntry() const { return LogEntry(); }

    // Record a log entry in internal storage only.
    void log(const std::string& entry) { record(Level::LOG, entry); }
    template <size_t n>
    void log(const char entry[n]) { log(std::string(entry)); }
    void log(const LogEntry& entry) { log(entry.toString()); }
    void log(const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3))) {
        using ::android::base::StringAppendV;
        std::string result;
        va_list ap;
        va_start(ap, fmt);
        StringAppendV(&result, fmt, ap);
        va_end(ap);
        log(result);
    }

    // Record a log entry in internal storage and to ALOGI as well.
    void info(const std::string& entry) { record(Level::INFO, entry); }
    template <size_t n>
    void info(const char entry[n]) { info(std::string(entry)); }
    void info(const LogEntry& entry) { info(entry.toString()); }
    void info(const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3))) {
        using ::android::base::StringAppendV;
        std::string result;
        va_list ap;
        va_start(ap, fmt);
        StringAppendV(&result, fmt, ap);
        va_end(ap);
        info(result);
    }

    // Record a log entry in internal storage and to ALOGW as well.
    void warn(const std::string& entry) { record(Level::WARN, entry); }
    template <size_t n>
    void warn(const char entry[n]) { warn(std::string(entry)); }
    void warn(const LogEntry& entry) { warn(entry.toString()); }
    void warn(const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3))) {
        using ::android::base::StringAppendV;
        std::string result;
        va_list ap;
        va_start(ap, fmt);
        StringAppendV(&result, fmt, ap);
        va_end(ap);
        warn(result);
    }

    // Record a log entry in internal storage and to ALOGE as well.
    void error(const std::string& entry) { record(Level::ERROR, entry); }
    template <size_t n>
    void error(const char entry[n]) { error(std::string(entry)); }
    void error(const LogEntry& entry) { error(entry.toString()); }
    void error(const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3))) {
        using ::android::base::StringAppendV;
        std::string result;
        va_list ap;
        va_start(ap, fmt);
        StringAppendV(&result, fmt, ap);
        va_end(ap);
        error(result);
    }

    // Iterates over every entry in the log in chronological order. Operates
    // on a copy of the log entries, and so perEntryFn may itself call one of
    // the logging functions if needed.
    void forEachEntry(const std::function<void(const std::string&)>& perEntryFn) const;

  private:
    static constexpr const size_t MAX_ENTRIES = 750U;
    const std::string mTag;
    const size_t mMaxEntries;

    // The LOG level adds an entry to mEntries but does not output the message
    // to the system log. All other levels append to mEntries and output to the
    // the system log.
    enum class Level {
        LOG,
        INFO,
        WARN,
        ERROR,
    };

    void record(Level lvl, const std::string& entry);

    mutable std::shared_mutex mLock;
    std::deque<const std::string> mEntries;  // GUARDED_BY(mLock), when supported
};

}  // namespace netdutils
}  // namespace android

#endif /* NETUTILS_LOG_H */
