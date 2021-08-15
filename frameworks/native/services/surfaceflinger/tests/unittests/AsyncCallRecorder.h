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

#pragma once

#include <chrono>
#include <deque>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include <android-base/thread_annotations.h>

namespace android {

// This class helps record calls made by another thread when they are made
// asynchronously, with no other way for the tests to verify that the calls have
// been made.
//
// A normal Google Mock recorder, while thread safe, does not allow you to wait
// for asynchronous calls to be made.
//
// Usage:
//
// In the test, use a Google Mock expectation to invoke an instance of the
// recorder:
//
//     AsyncCallRecorder<void(int)> recorder;
//
//     EXPECT_CALL(someMock, someFunction(_)).
//             .WillRepeatedly(Invoke(recorder.getInvocable()));
//
// Then you can invoke the functionality being tested:
//
//     threadUnderTest.doSomethingAsync()
//
// And afterwards make a number of assertions using the recorder:
//
//     // Wait for one call (with reasonable default timeout), and get the args
//     // as a std::tuple inside a std::optional.
//     auto args = recorder.waitForCall();
//     // The returned std::optional will have a value if the recorder function
//     // was called.
//     ASSERT_TRUE(args.has_value());
//     // The arguments can be checked if needed using standard tuple
//     // operations.
//     EXPECT_EQ(123, std::get<0>(args.value()));
//
// Alternatively maybe you want to assert that a call was not made.
//
//     EXPECT_FALSE(recorder.waitForUnexpectedCall().has_value());
//
// However this check uses a really short timeout so as not to block the test
// unnecessarily. And it could be possible for the check to return false and
// then the recorder could observe a call being made after.
template <typename Func>
class AsyncCallRecorder;

template <typename... Args>
class AsyncCallRecorder<void (*)(Args...)> {
public:
    // This wait value needs to be large enough to avoid flakes caused by delays
    // scheduling threads, but small enough that tests don't take forever if
    // something really is wrong. Based on some empirical evidence, 100ms should
    // be enough to avoid the former.
    static constexpr std::chrono::milliseconds DEFAULT_CALL_EXPECTED_TIMEOUT{100};

    // The wait here is tricky. It's for when We don't expect to record a call,
    // but we don't want to wait forever (or for longer than the typical test
    // function runtime). As even the simplest Google Test can take 1ms (1000us)
    // to run, we wait for half that time.
    static constexpr std::chrono::microseconds UNEXPECTED_CALL_TIMEOUT{500};

    using ArgTuple = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;

    void recordCall(Args... args) {
        std::lock_guard<std::mutex> lock(mMutex);
        mCalls.emplace_back(std::make_tuple(args...));
        mCondition.notify_all();
    }

    // Returns a functor which can be used with the Google Mock Invoke()
    // function, or as a std::function to record calls.
    auto getInvocable() {
        return [this](Args... args) { recordCall(args...); };
    }

    // Returns a set of arguments as a std::optional<std::tuple<...>> for the
    // oldest call, waiting for the given timeout if necessary if there are no
    // arguments in the FIFO.
    std::optional<ArgTuple> waitForCall(
            std::chrono::microseconds timeout = DEFAULT_CALL_EXPECTED_TIMEOUT)
            NO_THREAD_SAFETY_ANALYSIS {
        std::unique_lock<std::mutex> lock(mMutex);

        // Wait if necessary for us to have a record from a call.
        mCondition.wait_for(lock, timeout,
                            [this]() NO_THREAD_SAFETY_ANALYSIS { return !mCalls.empty(); });

        // Return the arguments from the oldest call, if one was made
        bool called = !mCalls.empty();
        std::optional<ArgTuple> result;
        if (called) {
            result.emplace(std::move(mCalls.front()));
            mCalls.pop_front();
        }
        return result;
    }

    // Waits using a small default timeout for when a call is not expected to be
    // made. The returned std::optional<std:tuple<...>> should not have a value
    // except if a set of arguments was unexpectedly received because a call was
    // actually made.
    //
    // Note this function uses a small timeout to not block test execution, and
    // it is possible the code under test could make the call AFTER the timeout
    // expires.
    std::optional<ArgTuple> waitForUnexpectedCall() { return waitForCall(UNEXPECTED_CALL_TIMEOUT); }

private:
    std::mutex mMutex;
    std::condition_variable mCondition;
    std::deque<ArgTuple> mCalls GUARDED_BY(mMutex);
};

// Like AsyncCallRecorder, but for when the function being invoked
// asynchronously is expected to return a value.
//
// This helper allows a single constant return value to be set to be returned by
// all calls that were made.
template <typename Func>
class AsyncCallRecorderWithCannedReturn;

template <typename Ret, typename... Args>
class AsyncCallRecorderWithCannedReturn<Ret (*)(Args...)>
      : public AsyncCallRecorder<void (*)(Args...)> {
public:
    explicit AsyncCallRecorderWithCannedReturn(Ret returnvalue) : mReturnValue(returnvalue) {}

    auto getInvocable() {
        return [this](Args... args) {
            this->recordCall(args...);
            return mReturnValue;
        };
    }

private:
    const Ret mReturnValue;
};

} // namespace android
