/*
 * Copyright (C) 2017 The Android Open Source Project
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

// Test various AAudio features including AAudioStream_setBufferSizeInFrames().

#include <condition_variable>
#include <mutex>
#include <stdio.h>

#include <android-base/macros.h>
#include <aaudio/AAudio.h>

#include <gtest/gtest.h>
#include <unistd.h>

// Callback function that does nothing.
aaudio_data_callback_result_t NoopDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames
) {
    (void) stream;
    (void) userData;
    (void) audioData;
    (void) numFrames;
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

// Test AAudioStream_setBufferSizeInFrames()

constexpr int64_t NANOS_PER_MILLISECOND = 1000 * 1000;

enum FunctionToCall {
    CALL_START, CALL_STOP, CALL_PAUSE, CALL_FLUSH
};

void checkStateTransition(aaudio_performance_mode_t perfMode,
                          aaudio_stream_state_t originalState,
                          FunctionToCall functionToCall,
                          aaudio_result_t expectedResult,
                          aaudio_stream_state_t expectedState) {
    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    // Request stream properties.
    AAudioStreamBuilder_setDataCallback(aaudioBuilder, NoopDataCallbackProc, nullptr);
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, perfMode);

    // Create an AAudioStream using the Builder.
    ASSERT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream));

    // Verify Open State
    aaudio_stream_state_t state = AAUDIO_STREAM_STATE_UNKNOWN;
    EXPECT_EQ(AAUDIO_OK, AAudioStream_waitForStateChange(aaudioStream,
                                                         AAUDIO_STREAM_STATE_UNKNOWN, &state,
                                                         1000 * NANOS_PER_MILLISECOND));
    EXPECT_EQ(AAUDIO_STREAM_STATE_OPEN, state);

    // Put stream into desired state.
    aaudio_stream_state_t inputState = AAUDIO_STREAM_STATE_UNINITIALIZED;
    if (originalState != AAUDIO_STREAM_STATE_OPEN) {

        ASSERT_EQ(AAUDIO_OK, AAudioStream_requestStart(aaudioStream));

        if (originalState != AAUDIO_STREAM_STATE_STARTING) {

            ASSERT_EQ(AAUDIO_OK, AAudioStream_waitForStateChange(aaudioStream,
                                                                 AAUDIO_STREAM_STATE_STARTING,
                                                                 &state,
                                                                 1000 * NANOS_PER_MILLISECOND));
            ASSERT_EQ(AAUDIO_STREAM_STATE_STARTED, state);

            if (originalState == AAUDIO_STREAM_STATE_STOPPING) {
                ASSERT_EQ(AAUDIO_OK, AAudioStream_requestStop(aaudioStream));
            } else if (originalState == AAUDIO_STREAM_STATE_STOPPED) {
                ASSERT_EQ(AAUDIO_OK, AAudioStream_requestStop(aaudioStream));
                inputState = AAUDIO_STREAM_STATE_STOPPING;
            } else if (originalState == AAUDIO_STREAM_STATE_PAUSING) {
                ASSERT_EQ(AAUDIO_OK, AAudioStream_requestPause(aaudioStream));
            } else if (originalState == AAUDIO_STREAM_STATE_PAUSED) {
                ASSERT_EQ(AAUDIO_OK, AAudioStream_requestPause(aaudioStream));
                inputState = AAUDIO_STREAM_STATE_PAUSING;
            }
        }
    }

    // Wait until past transitional state.
    if (inputState != AAUDIO_STREAM_STATE_UNINITIALIZED) {
        ASSERT_EQ(AAUDIO_OK, AAudioStream_waitForStateChange(aaudioStream,
                                                             inputState,
                                                             &state,
                                                             1000 * NANOS_PER_MILLISECOND));
        ASSERT_EQ(originalState, state);
    }

    aaudio_stream_state_t transitionalState = originalState;
    switch(functionToCall) {
        case FunctionToCall::CALL_START:
            EXPECT_EQ(expectedResult, AAudioStream_requestStart(aaudioStream));
            transitionalState = AAUDIO_STREAM_STATE_STARTING;
            break;
        case FunctionToCall::CALL_STOP:
            EXPECT_EQ(expectedResult, AAudioStream_requestStop(aaudioStream));
            transitionalState = AAUDIO_STREAM_STATE_STOPPING;
            break;
        case FunctionToCall::CALL_PAUSE:
            EXPECT_EQ(expectedResult, AAudioStream_requestPause(aaudioStream));
            transitionalState = AAUDIO_STREAM_STATE_PAUSING;
            break;
        case FunctionToCall::CALL_FLUSH:
            EXPECT_EQ(expectedResult, AAudioStream_requestFlush(aaudioStream));
            transitionalState = AAUDIO_STREAM_STATE_FLUSHING;
            break;
    }

    EXPECT_EQ(AAUDIO_OK, AAudioStream_waitForStateChange(aaudioStream,
                                                         transitionalState,
                                                         &state,
                                                         1000 * NANOS_PER_MILLISECOND));
    // We should not change state when a function fails.
    if (expectedResult != AAUDIO_OK) {
        ASSERT_EQ(originalState, expectedState);
    }
    EXPECT_EQ(expectedState, state);
    if (state != expectedState) {
        printf("ERROR - expected %s, actual = %s\n",
                AAudio_convertStreamStateToText(expectedState),
                AAudio_convertStreamStateToText(state));
        fflush(stdout);
    }

    AAudioStream_close(aaudioStream);
    AAudioStreamBuilder_delete(aaudioBuilder);
}

// TODO Use parameterized tests instead of these individual specific tests.

// OPEN =================================================================
TEST(test_various, aaudio_state_lowlat_open_start) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_START,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_none_open_start) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_START,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_lowlat_open_stop) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_STOP,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_none_open_stop) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_STOP,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_lowlat_open_pause) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_PAUSE,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_none_open_pause) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_PAUSE,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_lowlat_open_flush) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_FLUSH,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_FLUSHED);
}

TEST(test_various, aaudio_state_none_open_flush) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_OPEN,
            FunctionToCall::CALL_FLUSH,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_FLUSHED);
}


// STARTED =================================================================
TEST(test_various, aaudio_state_lowlat_started_start) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_START,
            AAUDIO_ERROR_INVALID_STATE,
            AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_none_started_start) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_START,
            AAUDIO_ERROR_INVALID_STATE,
            AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_lowlat_started_stop) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_STOP,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_none_started_stop) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_STOP,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_lowlat_started_pause) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_PAUSE,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_none_started_pause) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_PAUSE,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_lowlat_started_flush) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_FLUSH,
            AAUDIO_ERROR_INVALID_STATE,
            AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_none_started_flush) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STARTED,
            FunctionToCall::CALL_FLUSH,
            AAUDIO_ERROR_INVALID_STATE,
            AAUDIO_STREAM_STATE_STARTED);
}

// STOPPED =================================================================
TEST(test_various, aaudio_state_lowlat_stopped_start) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_START,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_none_stopped_start) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_START,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_lowlat_stopped_stop) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_STOP,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_none_stopped_stop) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_STOP,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_lowlat_stopped_pause) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_PAUSE,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_none_stopped_pause) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_PAUSE,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_lowlat_stopped_flush) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_FLUSH,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_FLUSHED);
}

TEST(test_various, aaudio_state_none_stopped_flush) {
    checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
            AAUDIO_STREAM_STATE_STOPPED,
            FunctionToCall::CALL_FLUSH,
            AAUDIO_OK,
            AAUDIO_STREAM_STATE_FLUSHED);
}

// PAUSED =================================================================
TEST(test_various, aaudio_state_lowlat_paused_start) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_START,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_none_paused_start) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_START,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_STARTED);
}

TEST(test_various, aaudio_state_lowlat_paused_stop) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_STOP,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_none_paused_stop) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_STOP,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_STOPPED);
}

TEST(test_various, aaudio_state_lowlat_paused_pause) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_PAUSE,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_none_paused_pause) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_PAUSE,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_PAUSED);
}

TEST(test_various, aaudio_state_lowlat_paused_flush) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_FLUSH,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_FLUSHED);
}

TEST(test_various, aaudio_state_none_paused_flush) {
checkStateTransition(AAUDIO_PERFORMANCE_MODE_NONE,
        AAUDIO_STREAM_STATE_PAUSED,
        FunctionToCall::CALL_FLUSH,
        AAUDIO_OK,
        AAUDIO_STREAM_STATE_FLUSHED);
}

// ==========================================================================
TEST(test_various, aaudio_set_buffer_size) {

    int32_t bufferCapacity;
    int32_t framesPerBurst = 0;
    int32_t actualSize = 0;

    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    // Request stream properties.
    AAudioStreamBuilder_setDataCallback(aaudioBuilder, NoopDataCallbackProc, nullptr);
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);

    // Create an AAudioStream using the Builder.
    EXPECT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream));

    // This is the number of frames that are read in one chunk by a DMA controller
    // or a DSP or a mixer.
    framesPerBurst = AAudioStream_getFramesPerBurst(aaudioStream);
    bufferCapacity = AAudioStream_getBufferCapacityInFrames(aaudioStream);
    printf("          bufferCapacity = %d, remainder = %d\n",
           bufferCapacity, bufferCapacity % framesPerBurst);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, 0);
    EXPECT_GT(actualSize, 0);
    EXPECT_LE(actualSize, bufferCapacity);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, 2 * framesPerBurst);
    EXPECT_GT(actualSize, framesPerBurst);
    EXPECT_LE(actualSize, bufferCapacity);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, bufferCapacity - 1);
    EXPECT_GT(actualSize, framesPerBurst);
    EXPECT_LE(actualSize, bufferCapacity);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, bufferCapacity);
    EXPECT_GT(actualSize, framesPerBurst);
    EXPECT_LE(actualSize, bufferCapacity);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, bufferCapacity + 1);
    EXPECT_GT(actualSize, framesPerBurst);
    EXPECT_LE(actualSize, bufferCapacity);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, 1234567);
    EXPECT_GT(actualSize, framesPerBurst);
    EXPECT_LE(actualSize, bufferCapacity);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, INT32_MAX);
    EXPECT_GT(actualSize, framesPerBurst);
    EXPECT_LE(actualSize, bufferCapacity);

    actualSize = AAudioStream_setBufferSizeInFrames(aaudioStream, INT32_MIN);
    EXPECT_GT(actualSize, 0);
    EXPECT_LE(actualSize, bufferCapacity);

    AAudioStream_close(aaudioStream);
    AAudioStreamBuilder_delete(aaudioBuilder);
}

// ************************************************************
// Test to make sure that AAUDIO_CALLBACK_RESULT_STOP works.

// Callback function that counts calls.
aaudio_data_callback_result_t CallbackOnceProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames
) {
    (void) stream;
    (void) audioData;
    (void) numFrames;

    std::atomic<int32_t> *callbackCountPtr = (std::atomic<int32_t> *)userData;
    (*callbackCountPtr)++;

    return AAUDIO_CALLBACK_RESULT_STOP;
}

void checkCallbackOnce(aaudio_performance_mode_t perfMode) {

    std::atomic<int32_t>   callbackCount{0};

    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    // Request stream properties.
    AAudioStreamBuilder_setDataCallback(aaudioBuilder, CallbackOnceProc, &callbackCount);
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, perfMode);

    // Create an AAudioStream using the Builder.
    ASSERT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream));
    AAudioStreamBuilder_delete(aaudioBuilder);

    ASSERT_EQ(AAUDIO_OK, AAudioStream_requestStart(aaudioStream));

    sleep(1); // Give callback a chance to run many times.

    EXPECT_EQ(AAUDIO_OK, AAudioStream_requestStop(aaudioStream));

    EXPECT_EQ(1, callbackCount.load()); // should stop after first call

    EXPECT_EQ(AAUDIO_OK, AAudioStream_close(aaudioStream));
}

TEST(test_various, aaudio_callback_once_none) {
    checkCallbackOnce(AAUDIO_PERFORMANCE_MODE_NONE);
}

TEST(test_various, aaudio_callback_once_lowlat) {
    checkCallbackOnce(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
}

// ************************************************************
struct WakeUpCallbackData {
    void wakeOther() {
        // signal waiting test to wake up
        {
            std::lock_guard <std::mutex> lock(mutex);
            finished = true;
        }
        conditionVariable.notify_one();
    }

    void waitForFinished() {
        std::unique_lock <std::mutex> aLock(mutex);
        conditionVariable.wait(aLock, [=] { return finished; });
    }

    // For signalling foreground test when callback finished
    std::mutex              mutex;
    std::condition_variable conditionVariable;
    bool                    finished = false;
};

// Test to make sure we cannot call recursively into the system from a callback.
struct DangerousData : public WakeUpCallbackData {
    aaudio_result_t resultStart = AAUDIO_OK;
    aaudio_result_t resultStop = AAUDIO_OK;
    aaudio_result_t resultPause = AAUDIO_OK;
    aaudio_result_t resultFlush = AAUDIO_OK;
    aaudio_result_t resultClose = AAUDIO_OK;
};

// Callback function that tries to call back into the stream.
aaudio_data_callback_result_t DangerousDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {
    (void) audioData;
    (void) numFrames;

    DangerousData *data = (DangerousData *)userData;
    data->resultStart = AAudioStream_requestStart(stream);
    data->resultStop = AAudioStream_requestStop(stream);
    data->resultPause = AAudioStream_requestPause(stream);
    data->resultFlush = AAudioStream_requestFlush(stream);
    data->resultClose = AAudioStream_close(stream);

    data->wakeOther();

    return AAUDIO_CALLBACK_RESULT_STOP;
}

//int main() { // To fix Android Studio formatting when editing.
void checkDangerousCallback(aaudio_performance_mode_t perfMode) {
    DangerousData        dangerousData;
    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream        *aaudioStream = nullptr;

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    // Request stream properties.
    AAudioStreamBuilder_setDataCallback(aaudioBuilder, DangerousDataCallbackProc, &dangerousData);
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, perfMode);

    // Create an AAudioStream using the Builder.
    ASSERT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream));
    AAudioStreamBuilder_delete(aaudioBuilder);

    ASSERT_EQ(AAUDIO_OK, AAudioStream_requestStart(aaudioStream));

    dangerousData.waitForFinished();

    EXPECT_EQ(AAUDIO_OK, AAudioStream_requestStop(aaudioStream));

    EXPECT_EQ(AAUDIO_ERROR_INVALID_STATE, dangerousData.resultStart);
    EXPECT_EQ(AAUDIO_ERROR_INVALID_STATE, dangerousData.resultStop);
    EXPECT_EQ(AAUDIO_ERROR_INVALID_STATE, dangerousData.resultPause);
    EXPECT_EQ(AAUDIO_ERROR_INVALID_STATE, dangerousData.resultFlush);
    EXPECT_EQ(AAUDIO_ERROR_INVALID_STATE, dangerousData.resultClose);

    EXPECT_EQ(AAUDIO_OK, AAudioStream_close(aaudioStream));
}

//int main() { // To fix Android Studio formatting when editing.

TEST(test_various, aaudio_callback_blockers_none) {
    checkDangerousCallback(AAUDIO_PERFORMANCE_MODE_NONE);
}

TEST(test_various, aaudio_callback_blockers_lowlat) {
    checkDangerousCallback(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
}
