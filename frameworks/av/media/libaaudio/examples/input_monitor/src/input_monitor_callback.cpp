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

#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <aaudio/AAudio.h>
#include "AAudioExampleUtils.h"
#include "AAudioSimpleRecorder.h"

int main(int argc, const char **argv)
{
    AAudioArgsParser      argParser;
    AAudioSimpleRecorder  recorder;
    PeakTrackerData_t     myData = {0.0};
    AAudioStream         *aaudioStream = nullptr;
    aaudio_result_t       result;
    aaudio_stream_state_t state;

    int       loopsNeeded = 0;
    const int displayRateHz = 20; // arbitrary

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);
    printf("%s - Display audio input using an AAudio callback, V0.1.3\n", argv[0]);

    if (argParser.parseArgs(argc, argv)) {
        return EXIT_FAILURE;
    }

    result = recorder.open(argParser,
                           SimpleRecorderDataCallbackProc,
                           SimpleRecorderErrorCallbackProc,
                           &myData);
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  recorder.open() returned %d\n", result);
        printf("IMPORTANT - Did you remember to enter:   adb root\n");
        goto error;
    }
    aaudioStream = recorder.getStream();
    argParser.compareWithStream(aaudioStream);

    printf("recorder.getFramesPerSecond() = %d\n", recorder.getFramesPerSecond());
    printf("recorder.getSamplesPerFrame() = %d\n", recorder.getSamplesPerFrame());

    result = recorder.start();
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  recorder.start() returned %d\n", result);
        goto error;
    }

    printf("Sleep for %d seconds while audio record in a callback thread.\n",
           argParser.getDurationSeconds());
    loopsNeeded = argParser.getDurationSeconds() * displayRateHz;
    for (int i = 0; i < loopsNeeded; i++)
    {
        const struct timespec request = { .tv_sec = 0,
                .tv_nsec = NANOS_PER_SECOND / displayRateHz };
        (void) clock_nanosleep(CLOCK_MONOTONIC, 0 /*flags*/, &request, NULL /*remain*/);
        printf("%08d: ", (int)recorder.getFramesRead());
        displayPeakLevel(myData.peakLevel);

        result = AAudioStream_waitForStateChange(aaudioStream,
                                                 AAUDIO_STREAM_STATE_CLOSED,
                                                 &state,
                                                 0);
        if (result != AAUDIO_OK) {
            fprintf(stderr, "ERROR - AAudioStream_waitForStateChange() returned %d\n", result);
            goto error;
        }
        if (state != AAUDIO_STREAM_STATE_STARTING && state != AAUDIO_STREAM_STATE_STARTED) {
            printf("Stream state is %d %s!\n", state, AAudio_convertStreamStateToText(state));
            break;
        }
    }
    printf("Woke up. Stop for a moment.\n");

    result = recorder.stop();
    if (result != AAUDIO_OK) {
        goto error;
    }
    usleep(2000 * 1000);
    result = recorder.start();
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  recorder.start() returned %d\n", result);
        goto error;
    }

    printf("Sleep for %d seconds while audio records in a callback thread.\n",
           argParser.getDurationSeconds());
    for (int i = 0; i < loopsNeeded; i++)
    {
        const struct timespec request = { .tv_sec = 0,
                .tv_nsec = NANOS_PER_SECOND / displayRateHz };
        (void) clock_nanosleep(CLOCK_MONOTONIC, 0 /*flags*/, &request, NULL /*remain*/);
        printf("%08d: ", (int)recorder.getFramesRead());
        displayPeakLevel(myData.peakLevel);

        state = AAudioStream_getState(aaudioStream);
        if (state != AAUDIO_STREAM_STATE_STARTING && state != AAUDIO_STREAM_STATE_STARTED) {
            printf("Stream state is %d %s!\n", state, AAudio_convertStreamStateToText(state));
            break;
        }
    }
    printf("Woke up now.\n");
    argParser.compareWithStream(aaudioStream);

    result = recorder.stop();
    if (result != AAUDIO_OK) {
        goto error;
    }
    result = recorder.close();
    if (result != AAUDIO_OK) {
        goto error;
    }

    printf("SUCCESS\n");
    return EXIT_SUCCESS;
error:
    recorder.close();
    printf("exiting - AAudio result = %d = %s\n", result, AAudio_convertResultToText(result));
    return EXIT_FAILURE;
}

