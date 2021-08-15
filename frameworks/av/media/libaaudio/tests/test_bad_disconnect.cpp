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

/**
 * Handle a DISCONNECT by only opening and starting a new stream
 * without stopping and closing the old one.
 * This caused the new stream to use the old disconnected device.
 */

#include <stdio.h>
#include <thread>
#include <unistd.h>

#include <aaudio/AAudio.h>

#define DEFAULT_TIMEOUT_NANOS  ((int64_t)1000000000)

static void s_myErrorCallbackProc(
        AAudioStream *stream,
        void *userData,
        aaudio_result_t error);

struct AudioEngine {
    AAudioStreamBuilder *builder = nullptr;
    AAudioStream *stream = nullptr;
    std::thread *thread = nullptr;
    int64_t framesRead = 0;
};

AudioEngine s_AudioEngine;

// Callback function that fills the audio output buffer.
static aaudio_data_callback_result_t s_myDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames
) {
    (void) userData;
    (void) audioData;
    (void) numFrames;
    s_AudioEngine.framesRead = AAudioStream_getFramesRead(stream);
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

static aaudio_result_t s_StartAudio() {
    int32_t framesPerBurst = 0;
    int32_t deviceId = 0;

    // Use an AAudioStreamBuilder to contain requested parameters.
    aaudio_result_t result = AAudio_createStreamBuilder(&s_AudioEngine.builder);
    if (result != AAUDIO_OK) {
        printf("AAudio_createStreamBuilder returned %s",
               AAudio_convertResultToText(result));
        return result;
    }

    // Request stream properties.
    AAudioStreamBuilder_setFormat(s_AudioEngine.builder, AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setPerformanceMode(s_AudioEngine.builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    AAudioStreamBuilder_setDataCallback(s_AudioEngine.builder, s_myDataCallbackProc, nullptr);
    AAudioStreamBuilder_setErrorCallback(s_AudioEngine.builder, s_myErrorCallbackProc, nullptr);

    // Create an AAudioStream using the Builder.
    result = AAudioStreamBuilder_openStream(s_AudioEngine.builder, &s_AudioEngine.stream);
    if (result != AAUDIO_OK) {
        printf("AAudioStreamBuilder_openStream returned %s",
               AAudio_convertResultToText(result));
        return result;
    }

    result = AAudioStream_requestStart(s_AudioEngine.stream);
    if (result != AAUDIO_OK) {
        printf("AAudioStream_requestStart returned %s",
               AAudio_convertResultToText(result));
    }

    // Check to see what kind of stream we actually got.
    deviceId = AAudioStream_getDeviceId(s_AudioEngine.stream);
    framesPerBurst = AAudioStream_getFramesPerBurst(s_AudioEngine.stream);

    printf("-------- started: deviceId = %3d, framesPerBurst = %3d\n", deviceId, framesPerBurst);

    return result;
}

static aaudio_result_t s_StopAudio() {
    aaudio_result_t result = AAUDIO_OK;
    if (s_AudioEngine.stream != nullptr) {
        result = AAudioStream_requestStop(s_AudioEngine.stream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_requestStop returned %s\n",
                   AAudio_convertResultToText(result));
        }
        result = AAudioStream_close(s_AudioEngine.stream);
        if (result != AAUDIO_OK) {
            printf("AAudioStream_close returned %s\n",
                   AAudio_convertResultToText(result));
        }
        s_AudioEngine.stream = nullptr;
        AAudioStreamBuilder_delete(s_AudioEngine.builder);
        s_AudioEngine.builder = nullptr;
    }
    return result;
}

static void s_StartThreadProc() {
    // A good app would call s_StopAudio here! This test simulates a bad app.
    s_StartAudio();
    s_AudioEngine.thread = nullptr;
}

static void s_myErrorCallbackProc(
        AAudioStream *stream __unused,
        void *userData __unused,
        aaudio_result_t error) {
    if (error == AAUDIO_ERROR_DISCONNECTED) {
        // Handle stream restart on a separate thread
        if (s_AudioEngine.thread == nullptr) {
            s_AudioEngine.thread = new std::thread(s_StartThreadProc);
        }
    }
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    aaudio_result_t result = AAUDIO_OK;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("Test Bad Disconnect V1.0\n");
    printf("\n=========== Please PLUG and UNPLUG headphones! ==============\n\n");
    printf("You should see the deviceID change on each plug event.\n");
    printf("Headphones will generally get a new deviceId each time.\n");
    printf("Speakers will have the same deviceId each time.\n");
    printf("The framesRead should reset on each plug event then increase over time.\n");
    printf("\n");

    result = s_StartAudio();

    if (result == AAUDIO_OK) {
        for (int i = 20; i > 0; i--) {
            sleep(1);
            printf("playing silence #%d, framesRead = %d\n", i, (int) s_AudioEngine.framesRead);
        }
    }

    s_StopAudio();

    printf("result = %d = %s\n", result, AAudio_convertResultToText(result));
}
