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

// Try to create as many streams as possible and report the maximum.

#include <stdio.h>
#include <stdlib.h>
#include <aaudio/AAudio.h>
#include <aaudio/AAudioTesting.h>

//#define MMAP_POLICY              AAUDIO_UNSPECIFIED
//#define MMAP_POLICY              AAUDIO_POLICY_NEVER
#define MMAP_POLICY              AAUDIO_POLICY_AUTO
//#define MMAP_POLICY              AAUDIO_POLICY_ALWAYS

#define MAX_STREAMS   200

aaudio_result_t testMaxStreams(aaudio_direction_t direction) {
    aaudio_result_t result = AAUDIO_OK;
    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStreams[MAX_STREAMS];
    int32_t numStreams = 0;

    result = AAudio_createStreamBuilder(&aaudioBuilder);
    if (result != AAUDIO_OK) {
        return 1;
    }

    AAudioStreamBuilder_setDirection(aaudioBuilder, direction);

    for (int i = 0; i < MAX_STREAMS; i++) {
        // Create an AAudioStream using the Builder.
        result = AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStreams[i]);
        if (result != AAUDIO_OK) {
            printf("ERROR could not open AAudio stream, %d %s\n",
                   result, AAudio_convertResultToText(result));
            break;
        } else {
            printf("AAudio stream[%2d] opened successfully. MMAP = %s\n",
                   i, AAudioStream_isMMapUsed(aaudioStreams[i]) ? "YES" : "NO");
            numStreams++;
        }
    }

    printf("Created %d streams!\n", numStreams);

    // Close all the streams.
    for (int i = 0; i < numStreams; i++) {
        result = AAudioStream_close(aaudioStreams[i]);
        if (result != AAUDIO_OK) {
            printf("ERROR could not close AAudio stream, %d %s\n",
                   result, AAudio_convertResultToText(result));
            break;
        } else {
            printf("AAudio stream[%2d] closed successfully.\n", i);
        }
    }

    AAudioStreamBuilder_delete(aaudioBuilder);

    return result;
}

int main(int argc, char **argv) {
    (void)argc; // unused
    (void)argv; // unused

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, NULL, _IONBF, (size_t) 0);

    printf("Try to open a maximum of %d streams.\n", MAX_STREAMS);

    AAudio_setMMapPolicy(MMAP_POLICY);
    printf("requested MMapPolicy = %d\n", AAudio_getMMapPolicy());

    printf("Test AAUDIO_DIRECTION_OUTPUT ---------\n");
    aaudio_result_t result = testMaxStreams(AAUDIO_DIRECTION_OUTPUT);
    if (result == AAUDIO_OK) {
        printf("Test AAUDIO_DIRECTION_INPUT ---------\n");
        result = testMaxStreams(AAUDIO_DIRECTION_INPUT);
    }

    return (result != AAUDIO_OK) ? EXIT_FAILURE : EXIT_SUCCESS;
}
