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

#include <aaudio/AAudio.h>


#define DEFAULT_TIMEOUT_NANOS  ((int64_t)1000000000)

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    aaudio_result_t result = AAUDIO_OK;

    int32_t triesLeft = 3;
    int32_t bufferCapacity;
    int32_t framesPerBurst = 0;
    float *buffer = nullptr;

    int32_t actualChannelCount = 0;
    int32_t actualSampleRate = 0;
    aaudio_format_t actualDataFormat = AAUDIO_FORMAT_PCM_FLOAT;

    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("TestRecovery:\n");

    // Use an AAudioStreamBuilder to contain requested parameters.
    result = AAudio_createStreamBuilder(&aaudioBuilder);
    if (result != AAUDIO_OK) {
        printf("AAudio_createStreamBuilder returned %s",
               AAudio_convertResultToText(result));
        goto finish;
    }

    // Request stream properties.
    AAudioStreamBuilder_setFormat(aaudioBuilder, AAUDIO_FORMAT_PCM_FLOAT);

    while (triesLeft-- > 0) {
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

        printf("-------- chans = %3d, rate = %6d format = %d\n",
                actualChannelCount, actualSampleRate, actualDataFormat);

        // This is the number of frames that are read in one chunk by a DMA controller
        // or a DSP or a mixer.
        framesPerBurst = AAudioStream_getFramesPerBurst(aaudioStream);
        bufferCapacity = AAudioStream_getBufferCapacityInFrames(aaudioStream);
        printf("         bufferCapacity = %d, framesPerBurst = %d\n",
        bufferCapacity, framesPerBurst);

        int samplesPerBurst = framesPerBurst * actualChannelCount;
        buffer = new float[samplesPerBurst];

        result = AAudioStream_requestStart(aaudioStream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_requestStart returned %s",
                   AAudio_convertResultToText(result));
            goto finish;
        }

        // Play silence for awhile.
        int32_t framesMax = actualSampleRate * 20;
        int64_t framesTotal = 0;
        int64_t printAt = actualSampleRate;
        while (result == AAUDIO_OK && framesTotal < framesMax) {
            int32_t framesWritten = AAudioStream_write(aaudioStream,
                                                       buffer, framesPerBurst,
                                                       DEFAULT_TIMEOUT_NANOS);
            if (framesWritten < 0) {
                result = framesWritten;
                printf("write() returned %s, frames = %d\n",
                       AAudio_convertResultToText(result), (int)framesTotal);
                printf("  frames = %d\n", (int)framesTotal);
            } else if (framesWritten != framesPerBurst) {
                printf("write() returned %d, frames = %d\n", framesWritten, (int)framesTotal);
                result = AAUDIO_ERROR_TIMEOUT;
            } else {
                framesTotal += framesWritten;
                if (framesTotal >= printAt) {
                    printf("frames = %d\n", (int)framesTotal);
                    printAt += actualSampleRate;
                }
            }
        }
        result = AAudioStream_requestStop(aaudioStream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_requestStop returned %s\n",
                   AAudio_convertResultToText(result));
        }
        result = AAudioStream_close(aaudioStream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_close returned %s\n",
                   AAudio_convertResultToText(result));
        }
        aaudioStream = nullptr;
    }

finish:
    if (aaudioStream != nullptr) {
        AAudioStream_close(aaudioStream);
    }
    AAudioStreamBuilder_delete(aaudioBuilder);
    delete[] buffer;
    printf("          result = %d = %s\n", result, AAudio_convertResultToText(result));
}
