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

// Play a shared stream that might use MMAP.
// Then play a second stream at a different sample rate.
// Make sure the first stream is still running.
// See: b/73369112 | AAudio disconnects shared stream if second MMAP open fails

#include <memory.h>
#include <stdio.h>
#include <unistd.h>

#include <android-base/macros.h>
#include <aaudio/AAudio.h>

#include <gtest/gtest.h>

// Callback function that fills the audio output buffer.
aaudio_data_callback_result_t MyDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {
    (void) userData;
    int32_t numSamples = AAudioStream_getChannelCount(stream) * numFrames;
    aaudio_format_t format = AAudioStream_getFormat(stream);
    if (format == AAUDIO_FORMAT_PCM_I16) {
        memset(audioData, 0, numSamples * sizeof(int16_t));
    } else if (format == AAUDIO_FORMAT_PCM_FLOAT) {
        memset(audioData, 0, numSamples * sizeof(float));
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

//void foo() { // for tricking the Android Studio formatter
TEST(test_interference, aaudio_mmap_interference) {

    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream1 = nullptr;
    AAudioStream *aaudioStream2 = nullptr;

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    // Request stream properties.
    AAudioStreamBuilder_setSampleRate(aaudioBuilder, 48000);
    AAudioStreamBuilder_setDataCallback(aaudioBuilder, MyDataCallbackProc, nullptr);
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);

    // Create an AAudioStream using the Builder.
    ASSERT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream1));
    // Start it running.
    ASSERT_EQ(AAUDIO_OK, AAudioStream_requestStart(aaudioStream1));

    // Verify that the stream is running.
    sleep(1);
    EXPECT_LT(0, AAudioStream_getFramesRead(aaudioStream1));
    ASSERT_EQ(AAUDIO_STREAM_STATE_STARTED, AAudioStream_getState(aaudioStream1));

    // Now try to open a second stream with a different rate.
    AAudioStreamBuilder_setSampleRate(aaudioBuilder, 44100);
    ASSERT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream2));
    ASSERT_EQ(AAUDIO_OK, AAudioStream_requestStart(aaudioStream2));

    // Verify that the second stream is running.
    sleep(1);
    EXPECT_LT(0, AAudioStream_getFramesRead(aaudioStream2));

    EXPECT_EQ(AAUDIO_STREAM_STATE_STARTED, AAudioStream_getState(aaudioStream2));

    // Now verify that the first stream is still running.
    EXPECT_EQ(AAUDIO_STREAM_STATE_STARTED, AAudioStream_getState(aaudioStream1));

    int32_t framesRead1_1 = AAudioStream_getFramesRead(aaudioStream1);
    EXPECT_LT(0, framesRead1_1);
    sleep(1);
    int32_t framesRead1_2 = AAudioStream_getFramesRead(aaudioStream1);
    EXPECT_LT(0, framesRead1_2);
    EXPECT_LT(framesRead1_1, framesRead1_2); // advancing?

    AAudioStream_close(aaudioStream2);
    AAudioStream_close(aaudioStream1);
    AAudioStreamBuilder_delete(aaudioBuilder);
}
