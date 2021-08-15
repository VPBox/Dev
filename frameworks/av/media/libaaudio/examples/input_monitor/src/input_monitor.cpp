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

// Record input using AAudio and display the peak amplitudes.

#include <new>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <aaudio/AAudio.h>
#include <aaudio/AAudioTesting.h>
#include "AAudioExampleUtils.h"
#include "AAudioSimpleRecorder.h"

#define MIN_FRAMES_TO_READ 48  /* arbitrary, 1 msec at 48000 Hz */

static const int FRAMES_PER_LINE = 20000;

int main(int argc, const char **argv)
{
    AAudioArgsParser      argParser;
    AAudioSimpleRecorder  recorder;
    AAudioStream         *aaudioStream = nullptr;

    aaudio_result_t       result;
    aaudio_format_t       actualDataFormat;
    aaudio_stream_state_t state;

    int32_t actualSamplesPerFrame;
    int32_t actualSampleRate;
    int32_t framesPerBurst = 0;
    int32_t framesPerRead = 0;
    int32_t framesToRecord = 0;
    int32_t framesLeft = 0;
    int32_t nextFrameCount = 0;
    int32_t frameCount = 0;
    int32_t xRunCount = 0;
    int32_t deviceId;

    int16_t *shortData = nullptr;
    float   *floatData = nullptr;
    float    peakLevel = 0.0;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("%s - Monitor input level using AAudio read, V0.1.3\n", argv[0]);

    if (argParser.parseArgs(argc, argv)) {
        return EXIT_FAILURE;
    }

    result = recorder.open(argParser);
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  recorder.open() returned %d\n", result);
        printf("IMPORTANT - Did you remember to enter:   adb root\n");
        goto finish;
    }
    aaudioStream = recorder.getStream();
    argParser.compareWithStream(aaudioStream);

    deviceId = AAudioStream_getDeviceId(aaudioStream);
    printf("deviceId = %d\n", deviceId);

    actualSamplesPerFrame = AAudioStream_getChannelCount(aaudioStream);
    printf("SamplesPerFrame = %d\n", actualSamplesPerFrame);
    actualSampleRate = AAudioStream_getSampleRate(aaudioStream);
    printf("SamplesPerFrame = %d\n", actualSampleRate);

    // This is the number of frames that are written in one chunk by a DMA controller
    // or a DSP.
    framesPerBurst = AAudioStream_getFramesPerBurst(aaudioStream);
    printf("DataFormat: framesPerBurst = %d\n",framesPerBurst);

    // Some DMA might use very short bursts of 16 frames. We don't need to read such small
    // buffers. But it helps to use a multiple of the burst size for predictable scheduling.
    framesPerRead = framesPerBurst;
    while (framesPerRead < MIN_FRAMES_TO_READ) {
        framesPerRead *= 2;
    }
    printf("DataFormat: framesPerRead  = %d\n",framesPerRead);

    actualDataFormat = AAudioStream_getFormat(aaudioStream);

    // Allocate a buffer for the PCM_16 audio data.
    switch (actualDataFormat) {
        case AAUDIO_FORMAT_PCM_I16:
            shortData = new int16_t[framesPerRead * actualSamplesPerFrame];
            break;
        case AAUDIO_FORMAT_PCM_FLOAT:
            floatData = new float[framesPerRead * actualSamplesPerFrame];
            break;
        default:
            fprintf(stderr, "UNEXPECTED FORMAT! %d", actualDataFormat);
            goto finish;
    }

    // Start the stream.
    result = recorder.start();
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  recorder.start() returned %d\n", result);
        goto finish;
    }

    state = AAudioStream_getState(aaudioStream);
    printf("after start, state = %s\n", AAudio_convertStreamStateToText(state));

    // Record for a while.
    framesToRecord = actualSampleRate * argParser.getDurationSeconds();
    framesLeft = framesToRecord;
    while (framesLeft > 0) {
        // Read audio data from the stream.
        const int64_t timeoutNanos = 1000 * NANOS_PER_MILLISECOND;
        int minFrames = (framesToRecord < framesPerRead) ? framesToRecord : framesPerRead;
        int actual = 0;
        if (actualDataFormat == AAUDIO_FORMAT_PCM_I16) {
            actual = AAudioStream_read(aaudioStream, shortData, minFrames, timeoutNanos);
        } else if (actualDataFormat == AAUDIO_FORMAT_PCM_FLOAT) {
            actual = AAudioStream_read(aaudioStream, floatData, minFrames, timeoutNanos);
        }
        if (actual < 0) {
            fprintf(stderr, "ERROR - AAudioStream_read() returned %d\n", actual);
            result = actual;
            goto finish;
        } else if (actual == 0) {
            fprintf(stderr, "WARNING - AAudioStream_read() returned %d\n", actual);
            goto finish;
        }
        framesLeft -= actual;
        frameCount += actual;

        // Peak finder.
        for (int frameIndex = 0; frameIndex < actual; frameIndex++) {
            float sample = 0.0f;
            if (actualDataFormat == AAUDIO_FORMAT_PCM_I16) {
                sample = shortData[frameIndex * actualSamplesPerFrame] * (1.0/32768);
            } else if (actualDataFormat == AAUDIO_FORMAT_PCM_FLOAT) {
                sample = floatData[frameIndex * actualSamplesPerFrame];
            }
            if (sample > peakLevel) {
                peakLevel = sample;
            }
        }

        // Display level as stars, eg. "******".
        if (frameCount > nextFrameCount) {
            displayPeakLevel(peakLevel);
            peakLevel = 0.0;
            nextFrameCount += FRAMES_PER_LINE;

            // Print timestamps.
            int64_t framePosition = 0;
            int64_t frameTime = 0;
            aaudio_result_t timeResult;
            timeResult = AAudioStream_getTimestamp(aaudioStream, CLOCK_MONOTONIC,
                                                   &framePosition, &frameTime);

            if (timeResult == AAUDIO_OK) {
                int64_t realTime = getNanoseconds();
                int64_t framesRead = AAudioStream_getFramesRead(aaudioStream);

                double latencyMillis = calculateLatencyMillis(framesRead, realTime,
                                                              framePosition, frameTime,
                                                              actualSampleRate);

                printf("--- timestamp: result = %4d, position = %lld, at %lld nanos"
                               ", latency = %7.2f msec\n",
                       timeResult,
                       (long long) framePosition,
                       (long long) frameTime,
                       latencyMillis);
            } else {
                printf("WARNING - AAudioStream_getTimestamp() returned %d\n", timeResult);
            }
        }
    }

    state = AAudioStream_getState(aaudioStream);
    printf("after loop, state = %s\n", AAudio_convertStreamStateToText(state));

    xRunCount = AAudioStream_getXRunCount(aaudioStream);
    printf("AAudioStream_getXRunCount %d\n", xRunCount);

    result = recorder.stop();
    if (result != AAUDIO_OK) {
        goto finish;
    }

    argParser.compareWithStream(aaudioStream);

finish:
    recorder.close();
    delete[] shortData;
    delete[] floatData;
    printf("exiting - AAudio result = %d = %s\n", result, AAudio_convertResultToText(result));
    return (result != AAUDIO_OK) ? EXIT_FAILURE : EXIT_SUCCESS;
}

