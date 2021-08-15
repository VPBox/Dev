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

// Try to create a resource leak in the server by opening a stream and then not closing it.
// Return 0 if the stream opened, 1 if it failed.

#include <stdio.h>
#include <stdlib.h>
#include <aaudio/AAudio.h>

int main(int argc, char **argv)
{
    (void)argc; // unused
    (void)argv; // unused

    aaudio_result_t result = AAUDIO_OK;
    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    result = AAudio_createStreamBuilder(&aaudioBuilder);
    if (result != AAUDIO_OK) {
        goto finish;
    }

    // Create an AAudioStream using the Builder.
    result = AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream);
    if (result != AAUDIO_OK) {
        printf("ERROR could not open AAudio stream, %d\n", result);
        goto finish;
    } else {
        printf("AAudio stream opened successfully.\n");
    }

    printf("Exit without closing the stream!\n");

finish:
    return (result != AAUDIO_OK) ? EXIT_FAILURE : EXIT_SUCCESS;
}

