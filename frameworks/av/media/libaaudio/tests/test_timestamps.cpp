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

// Play silence and recover from dead servers or disconnected devices.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <aaudio/AAudio.h>
#include <aaudio/AAudioTesting.h>
#include "AAudioExampleUtils.h"

// Arbitrary period for glitches, once per second at 48000 Hz.
#define FORCED_UNDERRUN_PERIOD_FRAMES    48000
// How long to sleep in a callback to cause an intentional glitch. For testing.
#define FORCED_UNDERRUN_SLEEP_MICROS     (10 * 1000)

#define MAX_TIMESTAMPS          1000

#define DEFAULT_TIMEOUT_NANOS   ((int64_t)1000000000)

#define NUM_SECONDS             1
#define NUM_LOOPS               4
#define MAX_TESTS               20

typedef struct TimestampInfo {
    int64_t         framesTotal;
    int64_t         appPosition; // frames
    int64_t         appNanoseconds;
    int64_t         timestampPosition;  // frames
    int64_t         timestampNanos;
    aaudio_result_t result;
} TimestampInfo;

typedef struct TimestampCallbackData_s {
    TimestampInfo  timestamps[MAX_TIMESTAMPS];
    int64_t        framesTotal = 0;
    int64_t        nextFrameToGlitch = FORCED_UNDERRUN_PERIOD_FRAMES;
    int32_t        timestampCount = 0; // in timestamps
    bool           forceUnderruns = false;
} TimestampCallbackData_t;

struct TimeStampTestLog {
    aaudio_policy_t           isMmap;
    aaudio_sharing_mode_t     sharingMode;
    aaudio_performance_mode_t performanceMode;
    aaudio_direction_t        direction;
    aaudio_result_t           result;
};

static int s_numTests = 0;
// Use a plain old array because we reference this from the callback and do not want any
// automatic memory allocation.
static TimeStampTestLog s_testLogs[MAX_TESTS]{};

static void logTestResult(bool isMmap,
                          aaudio_sharing_mode_t sharingMode,
                          aaudio_performance_mode_t performanceMode,
                          aaudio_direction_t direction,
                          aaudio_result_t result) {
    if(s_numTests >= MAX_TESTS) {
        printf("ERROR - MAX_TESTS too small = %d\n", MAX_TESTS);
        return;
    }
    s_testLogs[s_numTests].isMmap = isMmap;
    s_testLogs[s_numTests].sharingMode = sharingMode;
    s_testLogs[s_numTests].performanceMode = performanceMode;
    s_testLogs[s_numTests].direction = direction;
    s_testLogs[s_numTests].result = result;
    s_numTests++;
}

static void printTestResults() {
    for (int i = 0; i < s_numTests; i++) {
        TimeStampTestLog *log = &s_testLogs[i];
        printf("%2d: mmap = %3s, sharing = %9s, perf = %11s, dir = %6s ---- %4s\n",
               i,
               log->isMmap ? "yes" : "no",
               getSharingModeText(log->sharingMode),
               getPerformanceModeText(log->performanceMode),
               getDirectionText(log->direction),
               log->result ? "FAIL" : "pass");
    }
}

// Callback function that fills the audio output buffer.
aaudio_data_callback_result_t timestampDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData __unused,
        int32_t numFrames
) {

    // should not happen but just in case...
    if (userData == nullptr) {
        printf("ERROR - SimplePlayerDataCallbackProc needs userData\n");
        return AAUDIO_CALLBACK_RESULT_STOP;
    }
    TimestampCallbackData_t *timestampData = (TimestampCallbackData_t *) userData;

    aaudio_direction_t direction = AAudioStream_getDirection(stream);
    if (direction == AAUDIO_DIRECTION_INPUT) {
        timestampData->framesTotal += numFrames;
    }

    if (timestampData->forceUnderruns) {
        if (timestampData->framesTotal > timestampData->nextFrameToGlitch) {
            usleep(FORCED_UNDERRUN_SLEEP_MICROS);
            printf("Simulate glitch at %lld\n", (long long) timestampData->framesTotal);
            timestampData->nextFrameToGlitch += FORCED_UNDERRUN_PERIOD_FRAMES;
        }
    }

    if (timestampData->timestampCount < MAX_TIMESTAMPS) {
        TimestampInfo *timestamp = &timestampData->timestamps[timestampData->timestampCount];
        timestamp->result = AAudioStream_getTimestamp(stream,
                                                      CLOCK_MONOTONIC,
                                                      &timestamp->timestampPosition,
                                                      &timestamp->timestampNanos);
        timestamp->framesTotal = timestampData->framesTotal;
        timestamp->appPosition = (direction == AAUDIO_DIRECTION_OUTPUT)
                ? AAudioStream_getFramesWritten(stream)
                : AAudioStream_getFramesRead(stream);
        timestamp->appNanoseconds = getNanoseconds();
        timestampData->timestampCount++;
    }

    if (direction == AAUDIO_DIRECTION_OUTPUT) {
        timestampData->framesTotal += numFrames;
    }
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

static TimestampCallbackData_t sTimestampData;

static aaudio_result_t testTimeStamps(aaudio_policy_t mmapPolicy,
                           aaudio_sharing_mode_t sharingMode,
                           aaudio_performance_mode_t performanceMode,
                           aaudio_direction_t direction) {
    aaudio_result_t result = AAUDIO_OK;

    int32_t framesPerBurst = 0;
    int32_t actualChannelCount = 0;
    int32_t actualSampleRate = 0;
    int32_t originalBufferSize = 0;
    int32_t requestedBufferSize = 0;
    int32_t finalBufferSize = 0;
    bool    isMmap = false;
    aaudio_format_t actualDataFormat = AAUDIO_FORMAT_PCM_FLOAT;
    aaudio_sharing_mode_t actualSharingMode = AAUDIO_SHARING_MODE_SHARED;
    aaudio_sharing_mode_t actualPerformanceMode = AAUDIO_PERFORMANCE_MODE_NONE;

    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    memset(&sTimestampData, 0, sizeof(sTimestampData));

    printf("\n=================================================================================\n");
    printf("--------- testTimeStamps(policy = %d, sharing = %s, perf = %s, dir = %s) --------\n",
           mmapPolicy,
           getSharingModeText(sharingMode),
           getPerformanceModeText(performanceMode),
           getDirectionText(direction));

    AAudio_setMMapPolicy(mmapPolicy);

    // Use an AAudioStreamBuilder to contain requested parameters.
    result = AAudio_createStreamBuilder(&aaudioBuilder);
    if (result != AAUDIO_OK) {
        printf("AAudio_createStreamBuilder returned %s",
               AAudio_convertResultToText(result));
        goto finish;
    }

    // Request stream properties.
    AAudioStreamBuilder_setFormat(aaudioBuilder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setSharingMode(aaudioBuilder, sharingMode);
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, performanceMode);
    AAudioStreamBuilder_setDirection(aaudioBuilder, direction);
    AAudioStreamBuilder_setDataCallback(aaudioBuilder, timestampDataCallbackProc, &sTimestampData);

    // Create an AAudioStream using the Builder.
    result = AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream);
    if (result != AAUDIO_OK) {
        printf("AAudioStreamBuilder_openStream returned %s",
               AAudio_convertResultToText(result));
        goto finish;
    }

    // Check to see what kind of stream we actually got.
    actualSampleRate = AAudioStream_getSampleRate(aaudioStream);
    actualChannelCount = AAudioStream_getChannelCount(aaudioStream);
    actualDataFormat = AAudioStream_getFormat(aaudioStream);

    actualSharingMode = AAudioStream_getSharingMode(aaudioStream);
    if (actualSharingMode != sharingMode) {
        printf("did not get expected sharingMode, got %3d, skipping test\n",
               actualSharingMode);
        result = AAUDIO_OK;
        goto finish;
    }
    actualPerformanceMode = AAudioStream_getPerformanceMode(aaudioStream);
    if (actualPerformanceMode != performanceMode) {
        printf("did not get expected performanceMode, got %3d, skipping test\n",
               actualPerformanceMode);
        result = AAUDIO_OK;
        goto finish;
    }

    printf("    chans = %3d, rate = %6d format = %d\n",
           actualChannelCount, actualSampleRate, actualDataFormat);
    isMmap = AAudioStream_isMMapUsed(aaudioStream);
    printf("    Is MMAP used? %s\n", isMmap ? "yes" : "no");

    // This is the number of frames that are read in one chunk by a DMA controller
    // or a DSP or a mixer.
    framesPerBurst = AAudioStream_getFramesPerBurst(aaudioStream);
    printf("    framesPerBurst = %3d\n", framesPerBurst);

    originalBufferSize = AAudioStream_getBufferSizeInFrames(aaudioStream);
    requestedBufferSize = 4 * framesPerBurst;
    finalBufferSize = AAudioStream_setBufferSizeInFrames(aaudioStream, requestedBufferSize);

    printf("    BufferSize: original = %4d, requested = %4d, final = %4d\n",
           originalBufferSize, requestedBufferSize, finalBufferSize);

    {
        int64_t position;
        int64_t nanoseconds;
        result = AAudioStream_getTimestamp(aaudioStream, CLOCK_MONOTONIC, &position, &nanoseconds);
        printf("before start, AAudioStream_getTimestamp() returns %s\n",
               AAudio_convertResultToText(result));
    }

    for (int runs = 0; runs < NUM_LOOPS; runs++) {
        printf("------------------ loop #%d\n", runs);

        int64_t temp = sTimestampData.framesTotal;
        memset(&sTimestampData, 0, sizeof(sTimestampData));
        sTimestampData.framesTotal = temp;

        sTimestampData.forceUnderruns = false;

        result = AAudioStream_requestStart(aaudioStream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_requestStart returned %s",
                   AAudio_convertResultToText(result));
            goto finish;
        }

        for (int second = 0; second < NUM_SECONDS; second++) {
            // Give AAudio callback time to run in the background.
            usleep(200 * 1000);

            // Periodically print the progress so we know it hasn't died.
            printf("framesWritten = %d, XRuns = %d\n",
                   (int) AAudioStream_getFramesWritten(aaudioStream),
                   (int) AAudioStream_getXRunCount(aaudioStream)
            );
        }

        result = AAudioStream_requestStop(aaudioStream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_requestStop returned %s\n",
                   AAudio_convertResultToText(result));
        }

        printf("timestampCount = %d\n", sTimestampData.timestampCount);
        int printedGood = 0;
        int printedBad = 0;
        for (int i = 1; i < sTimestampData.timestampCount; i++) {
            TimestampInfo *timestamp = &sTimestampData.timestamps[i];
            if (timestamp->result != AAUDIO_OK) {
                if (printedBad < 5) {
                    printf("  %3d : frames %8lld, xferd %8lld, result = %s\n",
                           i,
                           (long long) timestamp->framesTotal,
                           (long long) timestamp->appPosition,
                           AAudio_convertResultToText(timestamp->result));
                    printedBad++;
                }
            } else {
                const bool posChanged = (timestamp->timestampPosition !=
                                   (timestamp - 1)->timestampPosition);
                const bool timeChanged = (timestamp->timestampNanos
                        != (timestamp - 1)->timestampNanos);
                if ((printedGood < 20) && (posChanged || timeChanged)) {
                    bool negative = timestamp->timestampPosition < 0;
                    bool retro = (i > 0 && (timestamp->timestampPosition <
                                            (timestamp - 1)->timestampPosition));
                    const char *message = negative ? " <=NEGATIVE!"
                                                   : (retro ? "  <= RETROGRADE!" : "");

                    double latency = calculateLatencyMillis(timestamp->timestampPosition,
                                                            timestamp->timestampNanos,
                                                            timestamp->appPosition,
                                                            timestamp->appNanoseconds,
                                                            actualSampleRate);
                    printf("  %3d : frames %8lld, xferd %8lld",
                           i,
                           (long long) timestamp->framesTotal,
                           (long long) timestamp->appPosition);
                    printf(" STAMP: pos = %8lld, nanos = %8lld, lat = %7.1f msec %s\n",
                           (long long) timestamp->timestampPosition,
                           (long long) timestamp->timestampNanos,
                           latency,
                           message);
                    printedGood++;
                }
            }
        }

        if (printedGood == 0) {
            printf("ERROR - AAudioStream_getTimestamp() never gave us a valid timestamp\n");
            result = AAUDIO_ERROR_INTERNAL;
        } else {
            // Make sure we do not get timestamps when stopped.
            int64_t position;
            int64_t time;
            aaudio_result_t tempResult = AAudioStream_getTimestamp(aaudioStream,
                                                                   CLOCK_MONOTONIC,
                                                                   &position, &time);
            if (tempResult != AAUDIO_ERROR_INVALID_STATE) {
                printf("ERROR - AAudioStream_getTimestamp() should return"
                       " INVALID_STATE when stopped! %s\n",
                       AAudio_convertResultToText(tempResult));
                result = AAUDIO_ERROR_INTERNAL;
            }
        }

        // Avoid race conditions in AudioFlinger.
        // There is normally a delay between a real user stopping and restarting a stream.
        sleep(1);
    }

finish:

    logTestResult(isMmap, sharingMode, performanceMode, direction, result);

    if (aaudioStream != nullptr) {
        AAudioStream_close(aaudioStream);
    }
    AAudioStreamBuilder_delete(aaudioBuilder);
    printf("result = %d = %s\n", result, AAudio_convertResultToText(result));
    return result;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    aaudio_result_t result = AAUDIO_OK;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("Test Timestamps V0.1.4\n");

    // Legacy
    aaudio_policy_t policy = AAUDIO_POLICY_NEVER;
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_SHARED,
                            AAUDIO_PERFORMANCE_MODE_NONE,
                            AAUDIO_DIRECTION_INPUT);
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_SHARED,
                            AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
                            AAUDIO_DIRECTION_INPUT);
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_SHARED,
                            AAUDIO_PERFORMANCE_MODE_NONE,
                            AAUDIO_DIRECTION_OUTPUT);
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_SHARED,
                            AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
                            AAUDIO_DIRECTION_OUTPUT);

    // MMAP
    policy = AAUDIO_POLICY_ALWAYS;
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_EXCLUSIVE,
                            AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
                            AAUDIO_DIRECTION_INPUT);
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_EXCLUSIVE,
                            AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
                            AAUDIO_DIRECTION_OUTPUT);
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_SHARED,
                            AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
                            AAUDIO_DIRECTION_INPUT);
    result = testTimeStamps(policy,
                            AAUDIO_SHARING_MODE_SHARED,
                            AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
                            AAUDIO_DIRECTION_OUTPUT);

    printTestResults();

    return (result == AAUDIO_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}
