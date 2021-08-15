/*
 * Copyright (C) 2019 The Android Open Source Project
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
 * Return stop from the callback
 * and then close the stream immediately.
 */

#include <atomic>
#include <mutex>
#include <stdio.h>
#include <thread>
#include <unistd.h>

#include <aaudio/AAudio.h>

#define DURATION_SECONDS   5

struct AudioEngine {
    AAudioStreamBuilder *builder = nullptr;
    AAudioStream        *stream = nullptr;
    std::thread         *thread = nullptr;

    std::atomic<bool>   started{false};
    std::mutex          doneLock; // Use a mutex so we can sleep on it while join()ing.
    std::atomic<bool>   done{false};

    aaudio_result_t join() {
        aaudio_result_t result = AAUDIO_ERROR_INVALID_STATE;
        if (stream != nullptr) {
            while (true) {
                {
                    // Will block if the thread is running.
                    // This mutex is used to close() immediately after the callback returns
                    // and before the requestStop() is called.
                    std::lock_guard<std::mutex> lock(doneLock);
                    if (done) break;
                }
                printf("join() got mutex but stream not done!");
                usleep(10 * 1000); // sleep then check again
            }
            result = AAudioStream_close(stream);
            stream = nullptr;
        }
        return result;
    }
};

// Callback function that fills the audio output buffer.
static aaudio_data_callback_result_t s_myDataCallbackProc(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames
) {
    (void) stream;
    (void) audioData;
    (void) numFrames;
    AudioEngine *engine = (struct AudioEngine *)userData;
    std::lock_guard<std::mutex> lock(engine->doneLock);
    engine->started = true;
    usleep(DURATION_SECONDS * 1000 * 1000); // Mimic SynthMark procedure.
    engine->done = true;
    return AAUDIO_CALLBACK_RESULT_STOP;
}

static void s_myErrorCallbackProc(
    AAudioStream *stream __unused,
    void *userData __unused,
    aaudio_result_t error) {
    printf("%s() - error = %d\n", __func__, error);
}

static aaudio_result_t s_OpenAudioStream(struct AudioEngine *engine) {
    // Use an AAudioStreamBuilder to contain requested parameters.
    aaudio_result_t result = AAudio_createStreamBuilder(&engine->builder);
    if (result != AAUDIO_OK) {
        printf("AAudio_createStreamBuilder returned %s",
               AAudio_convertResultToText(result));
        return result;
    }

    // Request stream properties.
    AAudioStreamBuilder_setPerformanceMode(engine->builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    AAudioStreamBuilder_setDataCallback(engine->builder, s_myDataCallbackProc, engine);
    AAudioStreamBuilder_setErrorCallback(engine->builder, s_myErrorCallbackProc, engine);

    // Create an AAudioStream using the Builder.
    result = AAudioStreamBuilder_openStream(engine->builder, &engine->stream);
    if (result != AAUDIO_OK) {
        printf("AAudioStreamBuilder_openStream returned %s",
               AAudio_convertResultToText(result));
        return result;
    }

    return result;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    struct AudioEngine engine;
    aaudio_result_t result = AAUDIO_OK;
    int errorCount = 0;

    // Make printf print immediately so that debug info is not stuck
    // in a buffer if we hang or crash.
    setvbuf(stdout, nullptr, _IONBF, (size_t) 0);

    printf("Test Return Stop Hang V1.0\n");

    result = s_OpenAudioStream(&engine);
    if (result != AAUDIO_OK) {
        printf("s_OpenAudioStream returned %s\n",
               AAudio_convertResultToText(result));
        errorCount++;
    }

    // Check to see what kind of stream we actually got.
    int32_t deviceId = AAudioStream_getDeviceId(engine.stream);
    aaudio_performance_mode_t actualPerfMode = AAudioStream_getPerformanceMode(engine.stream);
    printf("-------- opened: deviceId = %3d, perfMode = %d\n", deviceId, actualPerfMode);

    // Start stream.
    result = AAudioStream_requestStart(engine.stream);
    printf("AAudioStream_requestStart() returned %d >>>>>>>>>>>>>>>>>>>>>>\n", result);
    if (result != AAUDIO_OK) {
        errorCount++;
    } else {
        int counter = 0;
        while (!engine.started) {
            printf("Waiting for stream to start, %d\n", counter++);
            usleep(5 * 1000);
        }
        printf("You should see more messages %d seconds after this. If not then the test failed!\n",
               DURATION_SECONDS);
        result = engine.join(); // This might hang!
        AAudioStreamBuilder_delete(engine.builder);
        engine.builder = nullptr;
    }

    printf("aaudio result = %d = %s\n", result, AAudio_convertResultToText(result));
    printf("test %s\n", errorCount ? "FAILED" : "PASSED");

    return errorCount ? EXIT_FAILURE : EXIT_SUCCESS;
}
