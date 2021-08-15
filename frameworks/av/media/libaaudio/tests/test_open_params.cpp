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

// Play sine waves using AAudio.

#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

#include <android-base/macros.h>
#include <aaudio/AAudio.h>

#include <gtest/gtest.h>

// Callback function that fills the audio output buffer.
aaudio_data_callback_result_t MyDataCallbackProc(
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

static void testOpenOptions(aaudio_direction_t direction,
                            int32_t channelCount,
                            int32_t sampleRate,
                            aaudio_format_t format) {

    aaudio_result_t result = AAUDIO_OK;

    int32_t bufferCapacity;
    int32_t framesPerBurst = 0;

    int32_t actualChannelCount = 0;
    int32_t actualSampleRate = 0;
    aaudio_format_t actualDataFormat = AAUDIO_FORMAT_UNSPECIFIED;
    aaudio_direction_t actualDirection;

    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    printf("TestOpen: dir = %d, chans = %3d, rate = %6d format = %d\n",
           direction, channelCount, sampleRate, format);

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    // Request stream properties.
    AAudioStreamBuilder_setDirection(aaudioBuilder, direction);
    AAudioStreamBuilder_setSampleRate(aaudioBuilder, sampleRate);
    AAudioStreamBuilder_setChannelCount(aaudioBuilder, channelCount);
    AAudioStreamBuilder_setFormat(aaudioBuilder, format);
    AAudioStreamBuilder_setDataCallback(aaudioBuilder, MyDataCallbackProc, nullptr);

    //AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, AAUDIO_PERFORMANCE_MODE_NONE);
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    //AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, AAUDIO_PERFORMANCE_MODE_POWER_SAVING);

    // Create an AAudioStream using the Builder.
    result = AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream);
    if (result != AAUDIO_OK) {
        printf("Stream not opened! That may be OK.\n");
        goto finish;
    }

    // Check to see what kind of stream we actually got.
    actualSampleRate = AAudioStream_getSampleRate(aaudioStream);
    actualChannelCount = AAudioStream_getChannelCount(aaudioStream);
    actualDataFormat = AAudioStream_getFormat(aaudioStream);
    actualDirection = AAudioStream_getDirection(aaudioStream);

    printf("          dir = %d, chans = %3d, rate = %6d format = %d\n",
           direction, actualChannelCount, actualSampleRate, actualDataFormat);

    // If we ask for something specific then we should get that.
    if (channelCount != AAUDIO_UNSPECIFIED) {
        EXPECT_EQ(channelCount, actualChannelCount);
    }
    if (sampleRate != AAUDIO_UNSPECIFIED) {
        EXPECT_EQ(sampleRate, actualSampleRate);
    }
    if (format != AAUDIO_FORMAT_UNSPECIFIED) {
        EXPECT_EQ(format, actualDataFormat);
    }
    EXPECT_EQ(direction, actualDirection);

    // This is the number of frames that are read in one chunk by a DMA controller
    // or a DSP or a mixer.
    framesPerBurst = AAudioStream_getFramesPerBurst(aaudioStream);
    bufferCapacity = AAudioStream_getBufferCapacityInFrames(aaudioStream);
    printf("          bufferCapacity = %d, remainder = %d\n",
           bufferCapacity, bufferCapacity % framesPerBurst);

finish:
    AAudioStream_close(aaudioStream);
    AAudioStreamBuilder_delete(aaudioBuilder);
    printf("          result = %d = %s\n", result, AAudio_convertResultToText(result));
}

//void foo() { // for tricking the Android Studio formatter
TEST(test_open_params, aaudio_open_all) {
    aaudio_direction_t directions[] = {AAUDIO_DIRECTION_OUTPUT, AAUDIO_DIRECTION_INPUT};
    aaudio_format_t formats[] = {AAUDIO_FORMAT_UNSPECIFIED,
                                 AAUDIO_FORMAT_PCM_I16, AAUDIO_FORMAT_PCM_FLOAT};
    int32_t rates[] = {AAUDIO_UNSPECIFIED, 22050, 32000, 44100, 48000, 88200, 96000, 37913, 59132};

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    for (uint dirIndex = 0;dirIndex < arraysize(directions); dirIndex++) {
        aaudio_direction_t direction = directions[dirIndex];
        for (int32_t channelCount = 0; channelCount <= 8; channelCount++) {
            testOpenOptions(direction, channelCount,
                            AAUDIO_UNSPECIFIED, AAUDIO_FORMAT_UNSPECIFIED);
        }
        for (uint i = 0; i < arraysize(rates); i++) {
            testOpenOptions(direction, AAUDIO_UNSPECIFIED, rates[i], AAUDIO_FORMAT_UNSPECIFIED);
        }
        for (uint i = 0; i < arraysize(formats); i++) {
            testOpenOptions(direction, AAUDIO_UNSPECIFIED, AAUDIO_UNSPECIFIED, formats[i]);
        }
    }
}
