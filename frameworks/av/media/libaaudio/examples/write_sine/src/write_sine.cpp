/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <aaudio/AAudio.h>
#include <aaudio/AAudioTesting.h>
#include <asm/fcntl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "AAudioExampleUtils.h"
#include "AAudioSimplePlayer.h"
#include "AAudioArgsParser.h"

#define NUM_SECONDS           4

int main(int argc, const char **argv)
{
    AAudioArgsParser   argParser;
    AAudioSimplePlayer player;
    SineThreadedData_t myData;
    aaudio_result_t    result = AAUDIO_OK;

    int32_t         actualChannelCount = 0;
    int32_t         actualSampleRate = 0;
    aaudio_format_t actualDataFormat = AAUDIO_FORMAT_UNSPECIFIED;

    AAudioStream *aaudioStream = nullptr;
    int32_t  framesPerBurst = 0;
    int32_t  framesPerWrite = 0;
    int32_t  framesToPlay = 0;
    int32_t  framesLeft = 0;
    int32_t  xRunCount = 0;
    int      numActiveOscilators = 0;
    float   *floatData = nullptr;
    int16_t *shortData = nullptr;

    int      testFd = -1;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("%s - Play a sine wave using AAudio V0.1.3\n", argv[0]);

    if (argParser.parseArgs(argc, argv)) {
        return EXIT_FAILURE;
    }

    result = player.open(argParser);
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR -  player.open() returned %d\n", result);
        goto finish;
    }

    aaudioStream = player.getStream();

    argParser.compareWithStream(aaudioStream);

    actualChannelCount = AAudioStream_getChannelCount(aaudioStream);
    actualSampleRate = AAudioStream_getSampleRate(aaudioStream);
    actualDataFormat = AAudioStream_getFormat(aaudioStream);

    myData.sampleRate = actualSampleRate;
    myData.setupSineSweeps();

    // Some DMA might use very short bursts of 16 frames. We don't need to write such small
    // buffers. But it helps to use a multiple of the burst size for predictable scheduling.
    framesPerBurst = AAudioStream_getFramesPerBurst(aaudioStream);
    framesPerWrite = framesPerBurst;
    while (framesPerWrite < 48) {
        framesPerWrite *= 2;
    }
    printf("Buffer: framesPerBurst = %d\n",framesPerBurst);
    printf("Buffer: framesPerWrite = %d\n",framesPerWrite);

    // Allocate a buffer for the audio data.
    if (actualDataFormat == AAUDIO_FORMAT_PCM_FLOAT) {
        floatData = new float[framesPerWrite * actualChannelCount];
    } else if (actualDataFormat == AAUDIO_FORMAT_PCM_I16) {
        shortData = new int16_t[framesPerWrite * actualChannelCount];
    } else {
        printf("ERROR Unsupported data format!\n");
        goto finish;
    }

    testFd = open("/data/aaudio_temp.raw", O_CREAT | O_RDWR, S_IRWXU);
    printf("testFd = %d, pid = %d\n", testFd, getpid());

    // Start the stream.
    printf("call player.start()\n");
    result = player.start();
    if (result != AAUDIO_OK) {
        fprintf(stderr, "ERROR - AAudioStream_requestStart() returned %d\n", result);
        goto finish;
    }

    printf("after start, state = %s\n",
            AAudio_convertStreamStateToText(AAudioStream_getState(aaudioStream)));

    // Play for a while.
    framesToPlay = actualSampleRate * argParser.getDurationSeconds();
    framesLeft = framesToPlay;
    numActiveOscilators = (actualChannelCount > MAX_CHANNELS) ? MAX_CHANNELS : actualChannelCount;
    while (framesLeft > 0) {
        // Render as FLOAT or PCM
        if (actualDataFormat == AAUDIO_FORMAT_PCM_FLOAT) {
            for (int i = 0; i < numActiveOscilators; ++i) {
                myData.sineOscillators[i].render(&floatData[i], actualChannelCount,
                                                  framesPerWrite);
            }
        } else if (actualDataFormat == AAUDIO_FORMAT_PCM_I16) {
            for (int i = 0; i < numActiveOscilators; ++i) {
                myData.sineOscillators[i].render(&shortData[i], actualChannelCount,
                                                  framesPerWrite);
            }
        }

        // Write audio data to the stream.
        int64_t timeoutNanos = 1000 * NANOS_PER_MILLISECOND;
        int32_t minFrames = (framesToPlay < framesPerWrite) ? framesToPlay : framesPerWrite;
        int32_t actual = 0;
        if (actualDataFormat == AAUDIO_FORMAT_PCM_FLOAT) {
            actual = AAudioStream_write(aaudioStream, floatData, minFrames, timeoutNanos);
        } else if (actualDataFormat == AAUDIO_FORMAT_PCM_I16) {
            actual = AAudioStream_write(aaudioStream, shortData, minFrames, timeoutNanos);
        }
        if (actual < 0) {
            fprintf(stderr, "ERROR - AAudioStream_write() returned %d\n", actual);
            goto finish;
        } else if (actual == 0) {
            fprintf(stderr, "WARNING - AAudioStream_write() returned %d\n", actual);
            goto finish;
        }
        framesLeft -= actual;

        // Use timestamp to estimate latency.
        /*
        {
            int64_t presentationFrame;
            int64_t presentationTime;
            result = AAudioStream_getTimestamp(aaudioStream,
                                               CLOCK_MONOTONIC,
                                               &presentationFrame,
                                               &presentationTime
                                               );
            if (result == AAUDIO_OK) {
                int64_t elapsedNanos = getNanoseconds() - presentationTime;
                int64_t elapsedFrames = actualSampleRate * elapsedNanos / NANOS_PER_SECOND;
                int64_t currentFrame = presentationFrame + elapsedFrames;
                int64_t framesWritten = AAudioStream_getFramesWritten(aaudioStream);
                int64_t estimatedLatencyFrames = framesWritten - currentFrame;
                int64_t estimatedLatencyMillis = estimatedLatencyFrames * 1000 / actualSampleRate;
                printf("estimatedLatencyMillis %d\n", (int)estimatedLatencyMillis);
            }
        }
         */
    }

    xRunCount = AAudioStream_getXRunCount(aaudioStream);
    printf("AAudioStream_getXRunCount %d\n", xRunCount);

    printf("call stop()\n");
    result = player.stop();
    if (result != AAUDIO_OK) {
        goto finish;
    }

finish:
    printf("testFd = %d, fcntl before aaudio close returns 0x%08X\n",
           testFd, fcntl(testFd, F_GETFD));
    player.close();
    printf("testFd = %d, fcntl after aaudio close returns 0x%08X\n",
           testFd, fcntl(testFd, F_GETFD));
    if (::close(testFd) != 0) {
        printf("ERROR SharedMemoryParcelable::close() of testFd = %d, errno = %s\n",
               testFd, strerror(errno));
    }
    printf("testFd = %d, fcntl after close() returns 0x%08X\n", testFd, fcntl(testFd, F_GETFD));

    delete[] floatData;
    delete[] shortData;
    printf("exiting - AAudio result = %d = %s\n", result, AAudio_convertResultToText(result));
    return (result != AAUDIO_OK) ? EXIT_FAILURE : EXIT_SUCCESS;
}

