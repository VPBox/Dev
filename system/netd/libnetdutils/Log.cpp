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

#include "netdutils/Log.h"
#include "netdutils/Slice.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <sstream>

#include <android-base/strings.h>
#include <log/log.h>

using ::android::base::Join;
using ::android::base::StringPrintf;

namespace android {
namespace netdutils {

namespace {

std::string makeTimestampedEntry(const std::string& entry) {
    using ::std::chrono::duration_cast;
    using ::std::chrono::milliseconds;
    using ::std::chrono::system_clock;

    std::stringstream tsEntry;
    const auto now = system_clock::now();
    const auto time_sec = system_clock::to_time_t(now);
    tsEntry << std::put_time(std::localtime(&time_sec), "%m-%d %H:%M:%S.") << std::setw(3)
            << std::setfill('0')
            << duration_cast<milliseconds>(now - system_clock::from_time_t(time_sec)).count() << " "
            << entry;

    return tsEntry.str();
}

}  // namespace

std::string LogEntry::toString() const {
    std::vector<std::string> text;

    if (!mMsg.empty()) text.push_back(mMsg);
    if (!mFunc.empty()) {
        text.push_back(StringPrintf("%s(%s)", mFunc.c_str(), Join(mArgs, ", ").c_str()));
    }
    if (!mReturns.empty()) {
        text.push_back("->");
        text.push_back(StringPrintf("(%s)", Join(mReturns, ", ").c_str()));
    }
    if (!mUid.empty()) text.push_back(mUid);
    if (!mDuration.empty()) text.push_back(StringPrintf("(%s)", mDuration.c_str()));

    return Join(text, " ");
}

LogEntry& LogEntry::message(const std::string& message) {
    mMsg = message;
    return *this;
}

LogEntry& LogEntry::function(const std::string& function_name) {
    mFunc = function_name;
    return *this;
}

LogEntry& LogEntry::prettyFunction(const std::string& pretty_function) {
    // __PRETTY_FUNCTION__ generally seems to be of the form:
    //
    //     qualifed::returnType qualified::function(args...)
    //
    // where the qualified forms include "(anonymous namespace)" in the
    // "::"-delimited list and keywords like "virtual" (where applicable).
    //
    // Here we try to convert strings like:
    //
    //     virtual binder::Status android::net::NetdNativeService::isAlive(bool *)
    //     netdutils::LogEntry android::netd::(anonymous namespace)::AAA::BBB::function()
    //
    // into just "NetdNativeService::isAlive" or "BBB::function". Note that
    // without imposing convention, how to easily identify any namespace/class
    // name boundary is not obvious.
    const size_t endFuncName = pretty_function.rfind('(');
    const size_t precedingSpace = pretty_function.rfind(' ', endFuncName);
    size_t substrStart = (precedingSpace != std::string::npos) ? precedingSpace + 1 : 0;

    const size_t beginFuncName = pretty_function.rfind("::", endFuncName);
    if (beginFuncName != std::string::npos && substrStart < beginFuncName) {
        const size_t previousNameBoundary = pretty_function.rfind("::", beginFuncName - 1);
        if (previousNameBoundary < beginFuncName && substrStart < previousNameBoundary) {
            substrStart = previousNameBoundary + 2;
        } else {
            substrStart = beginFuncName + 2;
        }
    }

    mFunc = pretty_function.substr(substrStart, endFuncName - substrStart);
    return *this;
}

LogEntry& LogEntry::arg(const std::string& val) {
    mArgs.push_back(val.empty() ? "\"\"" : val);
    return *this;
}

template <>
LogEntry& LogEntry::arg<>(bool val) {
    mArgs.push_back(val ? "true" : "false");
    return *this;
}

LogEntry& LogEntry::arg(const std::vector<int32_t>& val) {
    mArgs.push_back(StringPrintf("[%s]", Join(val, ", ").c_str()));
    return *this;
}

LogEntry& LogEntry::arg(const std::vector<uint8_t>& val) {
    mArgs.push_back('{' + toHex(makeSlice(val)) + '}');
    return *this;
}

LogEntry& LogEntry::arg(const std::vector<std::string>& val) {
    mArgs.push_back(StringPrintf("[%s]", Join(val, ", ").c_str()));
    return *this;
}

LogEntry& LogEntry::returns(const std::string& rval) {
    mReturns.push_back(rval);
    return *this;
}

LogEntry& LogEntry::returns(bool rval) {
    mReturns.push_back(rval ? "true" : "false");
    return *this;
}

LogEntry& LogEntry::returns(const Status& status) {
    mReturns.push_back(status.msg());
    return *this;
}

LogEntry& LogEntry::withUid(uid_t uid) {
    mUid = StringPrintf("(uid=%d)", uid);
    return *this;
}

LogEntry& LogEntry::withAutomaticDuration() {
    using ms = std::chrono::duration<float, std::ratio<1, 1000>>;

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::stringstream duration;
    duration << std::setprecision(1) << std::chrono::duration_cast<ms>(end - mStart).count()
             << "ms";
    mDuration = duration.str();
    return *this;
}

LogEntry& LogEntry::withDuration(const std::string& duration) {
    mDuration = duration;
    return *this;
}

Log::~Log() {
    // TODO: dump the last N entries to the android log for possible posterity.
    info(LogEntry().function(__FUNCTION__));
}

void Log::forEachEntry(const std::function<void(const std::string&)>& perEntryFn) const {
    // We make a (potentially expensive) copy of the log buffer (including
    // all strings), in case the |perEntryFn| takes its sweet time.
    std::deque<std::string> entries;
    {
        std::shared_lock<std::shared_mutex> guard(mLock);
        entries.assign(mEntries.cbegin(), mEntries.cend());
    }

    for (const std::string& entry : entries) perEntryFn(entry);
}

void Log::record(Log::Level lvl, const std::string& entry) {
    switch (lvl) {
        case Level::LOG:
            break;
        case Level::INFO:
            ALOG(LOG_INFO, mTag.c_str(), "%s", entry.c_str());
            break;
        case Level::WARN:
            ALOG(LOG_WARN, mTag.c_str(), "%s", entry.c_str());
            break;
        case Level::ERROR:
            ALOG(LOG_ERROR, mTag.c_str(), "%s", entry.c_str());
            break;
    }

    std::lock_guard guard(mLock);
    mEntries.push_back(makeTimestampedEntry(entry));
    while (mEntries.size() > mMaxEntries) mEntries.pop_front();
}

}  // namespace netdutils
}  // namespace android
